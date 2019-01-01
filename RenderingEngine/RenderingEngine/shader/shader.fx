float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;
float4 gWorldLightPosition;
float4 gWorldCameraPosition : ViewPosition;

texture DiffuseMap_Tex;

sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMap_Tex);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};


struct VS_INPUT_VNT
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
    float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT_VNT
{
    float4 mPosition : POSITION;
    float3 mDiffuse : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 mReflection : TEXCOORD3;
    float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT_VNT vs_main_vnt(VS_INPUT_VNT Input)
{
    VS_OUTPUT_VNT Output;

    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    float3 worldNormal = mul(Input.mNormal, (float3x3) gWorldMatrix);
    worldNormal = normalize(worldNormal); // moved normal vectors

    float3 lightDir = normalize(Output.mPosition.xyz - gWorldLightPosition.xyz); // Light -> mPosition
    float3 viewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);
    Output.mViewDir = viewDir; //

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
    float3 diffuse = saturate(Input.mDiffuse);
    float3 reflection = normalize(Input.mReflection);
    float3 viewDir = normalize(Input.mViewDir);
    float3 specular = 0;
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, 20.0f);
    }
    float3 ambient = float3(0.2f, 0.2f, 0.2f);
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
    float4 factor = float4(ambient + diffuse + specular, 1);
    return albedo * factor;
}




float4 Ambient;
float4 Diffuse;
float4 Specular;
float4 Emissive;

struct VS_INPUT_RGBA
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
};

struct VS_OUTPUT_RGBA
{
    float4 mPosition : POSITION;
    float3 mDiffuse : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 mReflection : TEXCOORD3;
};

VS_OUTPUT_RGBA vs_main_RGBA(VS_INPUT_RGBA input)
{
    VS_OUTPUT_RGBA output;
    output.mPosition = mul(input.mPosition, gWorldMatrix);
    float3 lightDir = normalize(output.mPosition.xyz - gWorldLightPosition.xyz); // Light -> mPosition
    float3 worldNormal = mul(input.mNormal, (float3x3) gWorldMatrix);
    worldNormal = normalize(worldNormal); // moved normal vectors
    float3 viewDir = normalize(output.mPosition.xyz - gWorldCameraPosition.xyz);

    output.mViewDir = viewDir; //
    output.mDiffuse = dot(-lightDir, worldNormal);
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
 //   return Diffuse;
    float4 diffuse = float4(saturate(input.mDiffuse), 1);
    float3 reflection = normalize(input.mReflection);
    float3 viewDir = normalize(input.mViewDir);
    float specular = 0;
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, 20.0f);
    }
    float4 ambient_factor = float4(0.9f, 0.9f, 0.9f, 0.9f);
//    return float4(1.0f, 0.0f, 0.0f, 1.0f);
//    return Diffuse;
    return Ambient * ambient_factor + Diffuse * diffuse + Specular * specular;
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