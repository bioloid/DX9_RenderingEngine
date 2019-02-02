// shadowmap 만들고 -> Render 해서 흑백 처리하고 -> Vert, hori -> 합성해서 출력

struct VInput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VOutput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
    float4 Depth : TEXCOORD1;
};

struct PInput
{
    float2 UV : TEXCOORD0;
    float4 Depth : TEXCOORD1;
};


float4x4 WorldMatrix;
float4x4 ViewProjectionMatrix;
float4x4 LightViewProjectionMatrix;

texture ShadowTexture;
sampler2D ShadowSampler = sampler_state
{
    texture = ShadowTexture;
};



VOutput mainVertex(VInput Input)
{
    VOutput Output;
    Output.UV = Input.UV;
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Depth = mul(Output.Position, LightViewProjectionMatrix);
    Output.Position = mul(Output.Position, ViewProjectionMatrix);

    return Output;
}

float4 mainPixel(PInput Input) : COLOR
{
    float2 UV = Input.Depth.xy / Input.Depth.w;
    float currentDepth = Input.Depth.z / Input.Depth.w;
    UV.y = -UV.y;
    UV = UV * 0.5f + 0.5f;
    float4 shadowDepth = tex2D(ShadowSampler, UV);
    if (UV.y < 0 || UV.x < 0 || UV.y > 1 || UV.x > 1)
    {
        return float4(1, 1, 1, 1);
    }
    if (currentDepth > shadowDepth.r + 0.0005f)
        return float4(0.5, 0.5, 0.5, 1);
    return float4(1, 1, 1, 1);
}
// 3 5 6 7 6 5 3 = 35
// 0.085714f
// 0.142857f
// 0.171428f
// 0.2f
technique SoftShadow
{
    pass PASS0
    {
        VertexShader = compile vs_2_0 mainVertex();
        PixelShader = compile vs_2_0 mainPixel();
    }
}