/*!
 * @brief	輪郭線用のモデルシェーダー
 */
////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
    float4x4 mPrevWorld;
    float4x4 mPrevView;
    float4x4 mPrevProj;
};
////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0; //UV座標。
    SSkinVSIn skinVert; //スキン用のデータ。
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;

};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL; //法線。
    float2 uv : TEXCOORD0; //uv座標。
    float3 tangent : TANGENT; // 接ベクトル
    float3 biNormal : BINORMAL; // 従ベクトル
    float depthInView : TEXCOORD1;
    
};

struct SPSOut
{
    float4 depth : SV_Target0;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_normalMap : register(t1); //法線マップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    psIn.normal = normalize(mul(m, vsIn.normal)); //法線を回転させる。
    float4 defaultpos = vsIn.pos;
    vsIn.pos += float4(vsIn.normal*0.1f, 0); //頂点座標を法線方向に押し出すやつ
    
    float4 worldpos = mul(m, defaultpos);
    psIn.depthInView = mul(mView, worldpos).z;
    // step-4 接ベクトルと従ベクトルをワールド空間に変換する
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.depthInView = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    //頂点法線をピクセルシェーダーに渡す。
    psIn.normal = normalize(mul(m, vsIn.normal)); //法線を回転させる。
    psIn.uv = vsIn.uv;
    return psIn;
}
/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}
SPSOut PSMain(SPSIn psIn)
{
    SPSOut psOut;
    
    psOut.depth = psIn.depthInView;
  
   
    return psOut;
}