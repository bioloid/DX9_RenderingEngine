struct LIGHT
{
    float4 position;
    float4 lookTo;
};
struct VInput
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};

struct VOutput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
    float3 Diffuse : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
    float3 Reflection : TEXCOORD3;
    float4 ShadowUV : TEXCOORD4;
};

struct PInput
{
    float2 UV : TEXCOORD0;
    float3 Diffuse : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
    float3 Reflection : TEXCOORD3;
    float4 ShadowUV : TEXCOORD4;
};



float4x4 WorldMatrix;
float4x4 ViewProjectionMatrix;
float4x4 LightViewProjectionMatrix;
float4 CameraPosition;

LIGHT light_0;
texture MainTexture;
texture BlurTexture;

sampler2D MainSampler = sampler_state
{
    Texture = (MainTexture);
};

sampler2D BlurSampler = sampler_state
{
    Texture = (BlurTexture);
};

VOutput mainVertex(VInput Input)
{
    VOutput Output;
    float3 worldNormal, lightDir;
    Output.UV = Input.UV;
    Output.Position = mul(Input.Position, WorldMatrix);
    worldNormal = normalize(mul(Input.Normal, (float3x3) WorldMatrix));
    lightDir = normalize(light_0.lookTo - light_0.position);
    Output.ViewDir = normalize(Output.Position.xyz - CameraPosition.xyz);

    Output.Position = mul(Output.Position, ViewProjectionMatrix);
    Output.Diffuse = dot(-lightDir, worldNormal);
    Output.Reflection = reflect(lightDir, worldNormal);
    Output.ShadowUV = Output.Position;
    return Output;
}

float4 mainPixel(PInput Input) : COLOR
{
    float4 mainTextureColor = tex2D(MainSampler, Input.UV);
    float3 ambient = float3(0.2f, 0.2f, 0.2f);
    float3 diffuse = saturate(Input.Diffuse);
    float3 reflection = normalize(Input.Reflection);
    float3 specular = 0;
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -normalize(Input.ViewDir)));
        specular = pow(specular, 20.0f);
    }
    specular *= 0.5;
    float2 UV = Input.ShadowUV.xy / Input.ShadowUV.w;
    UV.y = -UV.y;
    UV = UV * 0.5f + 0.5f;
    if (UV.y < 0 || UV.x < 0 || UV.y > 1 || UV.x > 1)
    {
//        return float4(1, 0, 1, 1);
        return float4((ambient + diffuse + specular)
    * mainTextureColor.rgb, mainTextureColor.w);
    }
    float4 shadowDepth = tex2D(BlurSampler, UV);
//    return shadowDepth;
    return float4((ambient + diffuse + specular)
    * mainTextureColor.rgb * shadowDepth.r, mainTextureColor.w);
}



technique Main
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 mainVertex();
        PixelShader = compile ps_2_0 mainPixel();
    }
}