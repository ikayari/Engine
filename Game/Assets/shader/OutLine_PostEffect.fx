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
Texture2D<float4> g_scenedepthTextuere : register(t0);//シーンの深度テクスチャ

Texture2D<float4> g_depthTexture : register(t1); //深度値テクスチャ

sampler g_sampler : register(s0); //サンプラー

/*!
 *@brief    ピクセルシェーダーへの入力。
 */

float4 PSMain(PSInput psIn) : SV_Target0
{
   

    // 近傍8テクセルへのUVオフセット
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 900.0f), //上
        float2(0.0f, -1.0f / 900.0f), //下
        float2(1.0f / 1600.0f, 0.0f), //右
        float2(-1.0f / 1600.0f, 0.0f), //左
        float2(1.0f / 1600.0f, 1.0f / 900.0f), //右上
        float2(-1.0f / 1600.0f, 1.0f / 900.0f), //左上
        float2(1.0f / 1600.0f, -1.0f / 900.0f), //右下
        float2(-1.0f / 1600.0f, -1.0f / 900.0f) //左下
    };

    // このピクセルの深度値を取得
    float depth = g_depthTexture.Sample(g_sampler, psIn.uv).x;

    // 近傍8テクセルの深度値の平均値を計算する
    float depth2 = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_depthTexture.Sample(g_sampler, psIn.uv + uvOffset[i]).x;
    }
    depth2 /= 8.0f;
    float scenedepth = g_scenedepthTextuere.Sample(g_sampler, psIn.uv).r;
    // 自身の深度値と近傍8テクセルの深度値の差を調べる
    if (abs(depth - depth2) > 0.015f&& psIn.pos.z > depth-0.01f)
    {
        
        // 深度値が結構違う場合はピクセルカラーを黒にする
        // ->これがエッジカラーとなる
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
   

    return float4(1.0f, 1.0f, 1.0f, 0.0f);

}
