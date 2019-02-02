struct VS_INPUT
{
    float4 mPosition : POSITION;
    float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
    float2 mTexCoord : TEXCOORD0;
    float4 mZCoord : TEXCOORD1;
};

float4x4 gMatWorld;
float4x4 gMatLightViewProjection;
texture gMainTexture;

sampler2D DiffuseSampler = sampler_state
{
    Texture = (gMainTexture);
};
VS_OUTPUT shadow_vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.mTexCoord = Input.mTexCoord;
    Output.mPosition = mul(Input.mPosition, gMatWorld);
    Output.mPosition = mul(Output.mPosition, gMatLightViewProjection);
    Output.mZCoord = Output.mPosition;
    return Output;
}

struct PS_INPUT
{
    float2 mTexCoord : TEXCOORD0;
    float4 mZCoord : TEXCOORD1;
};
float4 shadow_ps_main(PS_INPUT Input) :COLOR
{
    float depth = Input.mZCoord.z / Input.mZCoord.w;
    return float4(depth, 0.0f, 0.0f, tex2D(DiffuseSampler, Input.mTexCoord).w);
}
technique CreateShadowShader
{
    pass CreateShadow
    {
        VertexShader = compile vs_2_0 shadow_vs_main();
        PixelShader = compile ps_2_0 shadow_ps_main();
    }
}