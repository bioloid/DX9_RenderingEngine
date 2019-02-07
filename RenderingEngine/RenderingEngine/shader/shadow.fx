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

float4x4 WorldMatrix;
float4x4 LightViewProjectionMatrix;
texture MainTexture;

sampler2D MainSampler = sampler_state
{
    Texture = (MainTexture);
};
VS_OUTPUT shadow_vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.mTexCoord = Input.mTexCoord;
    Output.mPosition = mul(Input.mPosition, WorldMatrix);
    Output.mPosition = mul(Output.mPosition, LightViewProjectionMatrix);
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
    return float4(depth.xxx, tex2D(MainSampler, Input.mTexCoord).w);
}
technique CreateShadowShader
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 shadow_vs_main();
        PixelShader = compile ps_2_0 shadow_ps_main();
    }
}