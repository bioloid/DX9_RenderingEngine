struct VInput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VOutput
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct PInput
{
    float2 UV : TEXCOORD0;
};

float4x4 OrthoMatrix;
texture BlurTexture;
float2 Screen;
sampler2D BlurSampler = sampler_state
{
    texture = BlurTexture;
};

VOutput mainVertex(VInput Input)
{
    VOutput Output;
    Output.Position = mul(Input.Position, OrthoMatrix);
    Output.UV = Input.UV;
    return Output;
}

float4 mainPixel(PInput Input) : COLOR
{
    float2 UV[8];
    UV[0] = Input.UV + float2(Screen.x, 0);
    UV[1] = Input.UV + float2(Screen.x * 2, 0);
    UV[2] = Input.UV + float2(Screen.x * 3, 0);
    UV[3] = Input.UV + float2(Screen.x * 4, 0);

    UV[4] = Input.UV - float2(Screen.x,     0);
    UV[5] = Input.UV - float2(Screen.x * 2, 0);
    UV[6] = Input.UV - float2(Screen.x * 3, 0);
    UV[7] = Input.UV - float2(Screen.x * 4, 0);
    return tex2D(BlurSampler, Input.UV);

    float4 Output = float4(0, 0, 0, 1);
    Output += tex2D(BlurSampler, Input.UV) * 0.189189f;
    Output += tex2D(BlurSampler, UV[0]) * 0.162162f;
    Output += tex2D(BlurSampler, UV[1]) * 0.135135f;
    Output += tex2D(BlurSampler, UV[2]) * 0.081081f;
    Output += tex2D(BlurSampler, UV[3]) * 0.027027f;
    Output += tex2D(BlurSampler, UV[4]) * 0.162162f;
    Output += tex2D(BlurSampler, UV[5]) * 0.135135f;
    Output += tex2D(BlurSampler, UV[6]) * 0.081081f;
    Output += tex2D(BlurSampler, UV[7]) * 0.027027f;
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
        PixelShader = compile ps_2_0 mainPixel();
    }
}