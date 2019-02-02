float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

float4x4 gLightViewMatrix;
float4x4 gLightProjectionMatrix : Projection;
struct LIGHT
{
    float4 position;
    float4 lookTo;
    float4 color;
};

LIGHT light_0;
float4 gWorldCameraPosition : ViewPosition;

float4 Ambient;
float4 Diffuse;
float4 Specular;
float4 Emissive;

texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state { Texture = (DiffuseMap_Tex); };

texture ShadowMap_Tex;
sampler2D ShadowSampler = sampler_state { Texture = (ShadowMap_Tex); };

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

    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    Output.mClipPosition = mul(Output.mPosition, gLightViewMatrix);
    Output.mClipPosition = mul(Output.mClipPosition, gLightProjectionMatrix);
    float3 worldNormal = mul(Input.mNormal, (float3x3) gWorldMatrix);
    worldNormal = normalize(worldNormal); // moved normal vectors

    float3 lightDir = normalize(light_0.lookTo - light_0.position);
    float3 viewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);
    Output.mViewDir = viewDir; 

    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjectionMatrix);


    Output.mTexCoord = Input.mTexCoord;
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
};

float4 ps_main_vnt(PS_INPUT_VNT Input) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
    float3 diffuse = saturate(Input.mDiffuse) * albedo.rgb * light_0.color.xyz;
    float3 reflection = normalize(Input.mReflection);
    float3 viewDir = normalize(Input.mViewDir);
    float3 specular = 0;
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, 20.0f);
    }
    float3 ambient = Ambient.xyz * float3(0.2f, 0.2f, 0.2f) * albedo.rgb * light_0.color.xyz;
    return float4(ambient + diffuse + Specular * specular * light_0.color.xyz, albedo.w);
}




struct VS_INPUT_RGBA
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
};

struct VS_OUTPUT_RGBA
{
    float4 mPosition : POSITION;
    float3 mDiffuse : TEXCOORD0;
    float3 mViewDir : TEXCOORD1;
    float3 mReflection : TEXCOORD2;
};

VS_OUTPUT_RGBA vs_main_RGBA(VS_INPUT_RGBA input)
{
    VS_OUTPUT_RGBA output;
    output.mPosition = mul(input.mPosition, gWorldMatrix);
    float3 worldNormal = mul(input.mNormal, (float3x3) gWorldMatrix);
    worldNormal = normalize(worldNormal); // moved normal vectors

    float3 lightDir = normalize(output.mPosition.xyz - light_0.position); // Light -> mPosition
    float3 viewDir = normalize(output.mPosition.xyz - gWorldCameraPosition.xyz);
    output.mViewDir = viewDir; //

//    output.mDiffuse = dot(-lightDir, worldNormal);
    output.mDiffuse = 1.0f;
    output.mReflection = reflect(lightDir, worldNormal);
    output.mPosition = mul(output.mPosition, gViewMatrix);
    output.mPosition = mul(output.mPosition, gProjectionMatrix);
    
    return output;
}

struct PS_INPUT_RGBA
{
    float3 mDiffuse : TEXCOORD0;
    float3 mViewDir : TEXCOORD1;
    float3 mReflection : TEXCOORD2;
};

float4 ps_main_RGBA(PS_INPUT_RGBA input) : COLOR
{
    float3 diffuse = saturate(input.mDiffuse);
    float3 reflection = normalize(input.mReflection);
    float3 viewDir = normalize(input.mViewDir);
    float3 specular = 0;
    if (input.mDiffuse.x > 0)
    {
        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, 20.0f);
    }
    return float4(Ambient + Diffuse * diffuse + Specular * specular, 1);
}


//--------------------------------------------------------------//
// Technique Section
//--------------------------------------------------------------//
technique VTN
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 vs_main_vnt();
        PixelShader = compile ps_2_0 ps_main_vnt();
    }
}

technique RGBA
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 vs_main_RGBA();
        PixelShader = compile ps_2_0 ps_main_RGBA();
    }
}