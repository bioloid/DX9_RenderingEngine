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
    UV[0] = Input.UV + float2(0, Screen.y);
    UV[1] = Input.UV + float2(0, Screen.y * 2);
    UV[2] = Input.UV + float2(0, Screen.y * 3);
    UV[3] = Input.UV + float2(0, Screen.y * 4);

    UV[4] = Input.UV - float2(0, Screen.y);
    UV[5] = Input.UV - float2(0, Screen.y * 2);
    UV[6] = Input.UV - float2(0, Screen.y * 3);
    UV[7] = Input.UV - float2(0, Screen.y * 4);
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
// 1 3 5 6 7 6 5 3 1 = 37
// 0.027027f
// 0.081081f
// 0.135135f
// 0.162162f
// 0.189189f
technique Vertical
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 mainVertex();
        PixelShader = compile ps_2_0 mainPixel();
    }
}