float4x4 WorldMatrix;
float4x4 ViewProjectionMatrix;
texture BlurTexture;
float2 Screen;
sampler2D BlurSampler = sampler_state
{
    texture = BlurTexture;
};

struct VInput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VOutput
{
    float4 Position : POSITION;
    float2 UV0 : TEXCOORD0;
    float2 UV1 : TEXCOORD1;
    float2 UV2 : TEXCOORD2;
    float2 UV3 : TEXCOORD3;
    float2 UV4 : TEXCOORD4;
    float2 UV5 : TEXCOORD5;
    float2 UV6 : TEXCOORD6;
};

struct PInput
{
    float2 UV0 : TEXCOORD0;
    float2 UV1 : TEXCOORD1;
    float2 UV2 : TEXCOORD2;
    float2 UV3 : TEXCOORD3;
    float2 UV4 : TEXCOORD4;
    float2 UV5 : TEXCOORD5;
    float2 UV6 : TEXCOORD6;
};

VOutput mainVertex(VInput Input)
{
    VOutput Output;
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewProjectionMatrix);
    Output.UV0 = Input.UV;
    Output.UV1 = Input.UV + float2(Screen.x,     0);
    Output.UV2 = Input.UV + float2(Screen.x * 2, 0);
    Output.UV3 = Input.UV + float2(Screen.x * 3, 0);
    Output.UV4 = Input.UV - float2(Screen.x,     0);
    Output.UV5 = Input.UV - float2(Screen.x * 2, 0);
    Output.UV6 = Input.UV - float2(Screen.x * 3, 0);
    return Output;
}

float4 mainPixel(PInput Input) : COLOR
{
    float4 Output = float4(0, 0, 0, 0);
    Output += tex2D(BlurSampler, Input.UV0) * 0.2f;
    Output += tex2D(BlurSampler, Input.UV1) * 0.171428f;
    Output += tex2D(BlurSampler, Input.UV2) * 0.142857f;
    Output += tex2D(BlurSampler, Input.UV3) * 0.085714f;
    Output += tex2D(BlurSampler, Input.UV4) * 0.171428f;
    Output += tex2D(BlurSampler, Input.UV5) * 0.142857f;
    Output += tex2D(BlurSampler, Input.UV6) * 0.085714f;
    return Output;
}
// 3 5 6 7 6 5 3 = 35
// 0.085714f
// 0.142857f
// 0.171428f
// 0.2f
technique Horizontal
{
    pass PASS0
    {
        VertexShader = compile vs_2_0 mainVertex();
        PixelShader = compile vs_2_0 mainPixel();
    }
}