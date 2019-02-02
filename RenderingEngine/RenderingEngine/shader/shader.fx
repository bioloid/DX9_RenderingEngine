float4x4 gMatWorld;
float4x4 gMatViewProjection;
float4x4 gMatLightViewProjection;

struct LIGHT
{
    float4 position;
    float4 lookTo;
    float4 color;
};

LIGHT light_0;
float4 gWorldCameraPosition : ViewPosition;

texture DiffuseTexture;
sampler2D DiffuseSampler = sampler_state { Texture = (DiffuseTexture); };

texture ShadowTexture;
sampler2D ShadowSampler = sampler_state { Texture = (ShadowTexture); };

struct VS_INPUT_VNT
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
    float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT_VNT
{
    float4 mPosition : POSITION;
    float2 mTexCoord : TEXCOORD0;
    float3 mDiffuse : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 mReflection : TEXCOORD3;
    float4 mClipPosition : TEXCOORD4;
};

VS_OUTPUT_VNT vs_main_vnt(VS_INPUT_VNT Input)
{
    VS_OUTPUT_VNT Output;
    Output.mTexCoord = Input.mTexCoord;

    Output.mPosition = mul(Input.mPosition, gMatWorld);
    Output.mClipPosition = mul(Output.mPosition, gMatLightViewProjection);
    float3 worldNormal = normalize(mul(Input.mNormal, (float3x3) gMatWorld));

    float3 lightDir = normalize(light_0.lookTo - light_0.position);
    Output.mViewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);

    Output.mPosition = mul(Output.mPosition, gMatViewProjection);
    Output.mDiffuse = dot(-lightDir, worldNormal);
    Output.mReflection = reflect(lightDir, worldNormal);

    return Output;
}

struct PS_INPUT_VNT
{
    float2 mTexCoord : TEXCOORD0;
    float3 mDiffuse : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 mReflection : TEXCOORD3;
    float4 mClipPosition : TEXCOORD4;
};

float4 ps_main_vnt(PS_INPUT_VNT Input) : COLOR
{
    float2 uv = Input.mClipPosition.xy / Input.mClipPosition.w;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;

    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
    float3 ambient = float3(0.2f, 0.2f, 0.2f);
    
    float3 diffuse = saturate(Input.mDiffuse);
    float3 reflection = normalize(Input.mReflection);

    float3 specular = 0;
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -normalize(Input.mViewDir)));
        specular = pow(specular, 20.0f);
    }
    specular *= 0.5;
    if (uv.y < 0 || uv.x < 0 || uv.y > 1 || uv.x > 1)
    {
        return float4((ambient + diffuse + specular) * albedo.rgb, albedo.w);
    }
    
    
    float currentDepth = Input.mClipPosition.z / Input.mClipPosition.w;
    float4 shadowDepth = tex2D(ShadowSampler, uv);

    if (currentDepth > shadowDepth.r + 0.0005f)
    {
        return float4((ambient + diffuse + specular) * albedo.rgb * 0.5, albedo.w);
    }

    return float4((ambient + diffuse + specular) * albedo.rgb, albedo.w);

}


struct INPUT_V
{
    float4 mPosition : POSITION;
    float2 mTexCoord : TEXCOORD0;
};

struct OUTPUT_V
{
    float4 mPosition : POSITION;
    float2 mTexCoord0 : TEXCOORD0;
    float2 mTexCoord1 : TEXCOORD1;
    float2 mTexCoord2 : TEXCOORD2;
    float2 mTexCoord3 : TEXCOORD3;
    float2 mTexCoord4 : TEXCOORD4;
    float2 mTexCoord5 : TEXCOORD5;
    float2 mTexCoord6 : TEXCOORD6;
    float2 mTexCoord7 : TEXCOORD7;
};

struct INPUT_P
{
    float2 mTexCoord0 : TEXCOORD0;
    float2 mTexCoord1 : TEXCOORD1;
    float2 mTexCoord2 : TEXCOORD2;
    float2 mTexCoord3 : TEXCOORD3;
    float2 mTexCoord4 : TEXCOORD4;
    float2 mTexCoord5 : TEXCOORD5;
    float2 mTexCoord6 : TEXCOORD6;
    float2 mTexCoord7 : TEXCOORD7;
};


texture gBlurTexture;
sampler2D BlurSampler = sampler_state
{
    Texture = (gBlurTexture);
};
OUTPUT_V vs_main(INPUT_V Input)
{
    float2 step = { 1.0f / 1280.0f, 1.0f / 720.0f };
    OUTPUT_V Output;
    Output.mTexCoord0 = Input.mTexCoord;
    Output.mTexCoord1 = Input.mTexCoord - float2(step.x, 0);
    Output.mTexCoord2 = Input.mTexCoord + float2(step.x, 0);
    Output.mTexCoord3 = Input.mTexCoord - float2(0, step.y);
    Output.mTexCoord4 = Input.mTexCoord + float2(0, step.y);
    Output.mTexCoord5 = Input.mTexCoord - float2(step.x, step.y);
    Output.mTexCoord6 = Input.mTexCoord + float2(step.x, step.y);
    Output.mTexCoord7 = Input.mTexCoord - float2(step.x, -step.y);

    Output.mPosition = mul(Input.mPosition, gMatWorld);
    Output.mPosition = mul(Output.mPosition, gMatViewProjection);

    return Output;
}
float4 ps_main(INPUT_P Input) : COLOR
{
    float4 tmp[8];
    tmp[0] = tex2D(BlurSampler, Input.mTexCoord0);
//    return tmp[0];
    tmp[1] = tex2D(BlurSampler, Input.mTexCoord1);
    tmp[2] = tex2D(BlurSampler, Input.mTexCoord2);
    tmp[3] = tex2D(BlurSampler, Input.mTexCoord3);
    tmp[4] = tex2D(BlurSampler, Input.mTexCoord4);
    tmp[5] = tex2D(BlurSampler, Input.mTexCoord5);
    tmp[6] = tex2D(BlurSampler, Input.mTexCoord6);
    tmp[7] = tex2D(BlurSampler, Input.mTexCoord7);
    float4 color;
    color = (tmp[0] + tmp[1] + tmp[2] + tmp[3] + tmp[4] + tmp[5] + tmp[6] + tmp[7]) / 8;
    return color;
}






//--------------------------------------------------------------//
// Technique Section
//--------------------------------------------------------------//
technique VTN
{
    pass Pass_0
    {
        VertexShader = compile vs_3_0 vs_main_vnt();
        PixelShader = compile ps_3_0 ps_main_vnt();
    }
}
technique Blur
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 vs_main();
        PixelShader = compile ps_2_0 ps_main();
    }
}