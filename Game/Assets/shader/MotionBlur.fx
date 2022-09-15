/*!
 * @brief ブルーム
 */

cbuffer cb : register(b0)
{
    float4x4 mvp; // MVP行列
    float4 mulColor; // 乗算カラー
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D<float4> mainRenderTexture : register(t0);
Texture2D<float4> velocityMapTexture : register(t1);
sampler g_sampler : register(s0);

/*!
 * @brief 頂点シェーダー
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}



float4 PSMain(PSInput In) : SV_Target0
{
    
    float2 velocity = velocityMapTexture.Sample(g_sampler, In.uv).rg * 2.0f;
    float2 velocitylength = velocity;
    float1 vellength =length(velocitylength);
    
    clip(vellength - 0.01f);
    //if (vellength <= 5.0f && vellength >= -5.0f)
    //{
    //    clip(-1);

    //}
    float uvStepAdd = 0.1f;
    float uvStep = 0.0f;
    int blurStep = 4;
    float4 Out = 0.0f;

    Out += mainRenderTexture.Sample(g_sampler, In.uv - (velocity * uvStep)) * 0.7f;
    uvStep += uvStepAdd;
    
    Out += mainRenderTexture.Sample(g_sampler, In.uv - (velocity * uvStep)) * 0.2f;
    uvStep += uvStepAdd;
    
    Out += mainRenderTexture.Sample(g_sampler, In.uv - (velocity * uvStep)) * 0.075f;
    uvStep += uvStepAdd;
    
    Out += mainRenderTexture.Sample(g_sampler, In.uv - (velocity * uvStep)) * 0.025f;
    
    // Out.xyz /= blurStep;
   // Out.xyz = float3(1, 0, 0);
    Out.a = 1.0f;
    return Out;
    //float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;
    //return color;
}