/*!
 * @brief	シンプルなモデルシェーダー。
 */

// ディレクションライト
struct DirectionLight
{
    float3 direction; // ライトの方向
    float3  color; // ライトのカラー
    
};

// ポイントライト
struct PointLight
{
    float3  position; //位置
    float3  color; //カラー
    float   range; //影響範囲
};

//スポットライト
struct SpotLight
{
    float3  position;   //位置
    float3  color;      //カラー
    float   range;      //影響範囲
    float3  direction;  //方向
    float   angle;      //角度
};

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

cbuffer LightCb : register(b1)
{
    DirectionLight directionLight;
    PointLight pointLight;
    SpotLight spotLight;
    //アンビエントライト。
    float3 ambientLight; //環境光。
    
    float3 eyePos; //視点の位置。
    
    float4x4 mLVP;
    
    float3 targetPosition;
    
    float clipratio;
};


////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
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
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2; //カメラ空間の法線
    float4 posInLVP : TEXCOORD3; // ライトビュースクリーン空間でのピクセルの座標1
    float depthInView : TEXCOORD4;
    //float4 velocityMap : TEXCOORD5;
    float4 prevPosInProj : TEXCOORD5;
    float4 posInProj : TEXCOORD6;
    float3 toEye : TEXCOORD7; //    
};

struct SPSOut
{
    float4 color : SV_Target0;
    float depth : SV_Target1;
    float4 velocity : SV_Target2;
};
///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromDirectionLight(SPSIn psIn);
float3 CalcLigFromSpotLight(SPSIn psIn);
float3 CalcRimLight(SPSIn psIn,float3 lightdirection,float3 lightcolor);
float3 CalcToonRatio(float3 lightDirection,float3 normal);
float4 DrawVelocityMap(SVSIn vsIn, SPSIn psIn);

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_normalMap : register(t1); //法線マップ
Texture2D<float4> g_specularMap : register(t2); //スペキュラーマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。
Texture2D<float4> g_shadowMap : register(t10); // シャドウマップ
sampler g_sampler : register(s0); //サンプラステート。

//ディザパターンの定義
static const int pattern[4][4] =
{
    { 0, 32, 8, 40 },
    { 48, 16, 56, 24 },
    { 12, 44, 4, 36 },
    { 60, 28, 52, 20 },
};

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}
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
   // psIn.velocityMap = DrawVelocityMap(vsIn, psIn);
    // step-4 接ベクトルと従ベクトルをワールド空間に変換する
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.depthInView = psIn.pos.z; //Z値を入れる
    psIn.pos = mul(mProj, psIn.pos);
    psIn.posInProj = psIn.pos;
    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    psIn.toEye = eyePos - psIn.worldPos;
    psIn.toEye = normalize(psIn.toEye);
    
    psIn.prevPosInProj = mul(mPrevWorld, vsIn.pos);
    psIn.prevPosInProj = mul(mPrevView, psIn.prevPosInProj);
    psIn.prevPosInProj = mul(mPrevProj, psIn.prevPosInProj);

	//頂点法線をピクセルシェーダーに渡す。
    psIn.normal = normalize(mul(m, vsIn.normal)); //法線を回転させる。

    psIn.uv = vsIn.uv;
    
    //カメラ空間の法線を求める。
    psIn.normalInView = normalize(mul(mView, psIn.normal));
    
    psIn.posInLVP = mul(mLVP, float4(psIn.worldPos, 1.0f));

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
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);

    // 鏡面反射光を求める
    return lightColor * t;
}
/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
    // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(directionLight.direction, directionLight.color, psIn.normal);

    // ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(directionLight.direction, directionLight.color, psIn.worldPos, psIn.normal);
    
    //ディレクションライトによるリムライトを計算する。
    float3 rimDirection = CalcRimLight(psIn, directionLight.direction, directionLight.color);
    
    return diffDirection + specDirection +rimDirection;
}
/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn)
{
    // このサーフェイスに入射しているポイントライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - pointLight.position;

    // 正規化して大きさ1のベクトルにする
    ligDir = normalize(ligDir);

    // 減衰なしのLambert拡散反射光を計算する
    float3 diffPoint = CalcLambertDiffuse(
        ligDir, // ライトの方向
        pointLight.color, // ライトのカラー
        psIn.normal // サーフェイスの法線
    );

    // 減衰なしのPhong鏡面反射光を計算する
    float3 specPoint = CalcPhongSpecular(
        ligDir, // ライトの方向
        pointLight.color, // ライトのカラー
        psIn.worldPos, // サーフェイズのワールド座標
        psIn.normal     // サーフェイズの法線
    );
    //減衰なしのリムライトを計算する。
    float3 rimPoint = CalcRimLight(psIn, ligDir, pointLight.color);

    // 距離による影響率を計算する
    // ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - pointLight.position);

    // 影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / pointLight.range * distance;

    // 影響力がマイナスにならないように補正をかける
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
    affect = pow(affect, 3.0f);

    // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
    diffPoint *= affect;
    specPoint *= affect;
    rimPoint *= affect;

    return diffPoint + specPoint + rimPoint;
}
//スポットライトによる反射光を計算。
float3 CalcLigFromSpotLight(SPSIn psIn)
{
     // このサーフェイスに入射しているポイントライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - spotLight.position;

    // 正規化して大きさ1のベクトルにする
    ligDir = normalize(ligDir);

    // 減衰なしのLambert拡散反射光を計算する
    float3 diffPoint = CalcLambertDiffuse(
        ligDir, // ライトの方向
        spotLight.color, // ライトのカラー
        psIn.normal // サーフェイスの法線
    );

    // 減衰なしのPhong鏡面反射光を計算する
    float3 specPoint = CalcPhongSpecular(
        ligDir, // ライトの方向
        spotLight.color, // ライトのカラー
        psIn.worldPos, // サーフェイズのワールド座標
        psIn.normal     // サーフェイズの法線
    );
    //減衰なしのリムライトを計算する。
    float3 rimPoint = CalcRimLight(psIn, spotLight.direction, spotLight.color);

    // 距離による影響率を計算する
    // ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - spotLight.position);

    // 影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / spotLight.range * distance;

    // 影響力がマイナスにならないように補正をかける
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
    affect = pow(affect, 3.0f);

    // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
    diffPoint *= affect;
    specPoint *= affect;
    rimPoint *= affect;
    //入射光と射出方向の角度を求める。
    float angle = dot(ligDir, spotLight.direction);
    
    angle = abs(acos(angle));
    
    //角度に比例して小さくなっていく影響率を計算
    affect = 1.0f - 1.0f / spotLight.angle * angle;
    //影響率がマイナスにならないように補正。
    if(affect<0.0f)
    {
        affect = 0.0f;
    }
    
    //影響の仕方を指数関数的にする。
    affect = pow(affect, 0.5f);
    
    //角度による影響率を反射光に乗算。
    diffPoint *= affect;
    specPoint *= affect;
    rimPoint *= affect;
    
    return diffPoint + specPoint + rimPoint;
    
}
//リムライトの計算。
float3 CalcRimLight(SPSIn psIn, float3 direction,float3 color)
{
    //サーフェイスの法線と光の入射方向に依存するリムの強さを求める。
    float power1 = 1.0f - max(0.0f, dot(direction, psIn.normal));
    //サーフェイスの法線と視線の方向に依存するリムの強さを求める。
    float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
    
    //最終的なリムの強さを求める。
    float limpower = power1 * power2;
    //強さの変化を指数関数的にする。
    limpower = pow(limpower, 1.3f);
    //リムライトのカラーを計算する。
    float3 limcolor = limpower * color;
    
    return limcolor;

}
float3 CalcToonRatio(float3 lightDirection,float3 normal)
{
    //ハーフランバート拡散照明によるライティング計算

    float toon = max(0.0f,dot(normal, lightDirection) * -1.0f);
    if(toon>=0.7f)
    {    
       return float3(1.0f,1.0f,1.0f);
    }
    else if (toon >=0.5f)
    {
        return float3(0.6f, 0.6f, 0.6f);
    }
    return float3(0.4f, 0.4f, 0.4f);


}

// モデル用のピクセルシェーダーのエントリーポイント
float4 PSMainCore(SPSIn psIn,uniform bool shadowreceive) : SV_Target0
{
        // step-2 ディザパターンを利用してディザリングを実装する。
    //このピクセルのスクリーン座標系でのX座標、Y座標を4で割った余りを求める。
    int x = (int) fmod(psIn.pos.x, 4.0f);
    int y = (int) fmod(psIn.pos.y, 4.0f);

    //上で求めた、xとyを利用して、このピクセルのディザリング閾値を取得する。
    int dither = pattern[y][x];

    clip(dither - 64 * clipratio);


    psIn.normal = normalize(GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv));
        //ディレクションライトによるライティングを計算する
    float3 directionLig = CalcLigFromDirectionLight(psIn);
	
    // ポイントライトによるライティングを計算する
    float3 pointLig = CalcLigFromPointLight(psIn);

    //スポットライトによるライティングを計算する。
    float3 spotLig = CalcLigFromSpotLight(psIn);
    
   
    // step-6 ライトビュースクリーン空間からUV空間に座標変換
    // 【注目】ライトビュースクリーン空間からUV座標空間に変換している
    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    //ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;
    
    // step-7 UV座標を使ってシャドウマップから影情報をサンプリング
    float3 shadowMap = 1.0f;
    
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        // step-3 シャドウマップに描き込まれているZ値と比較する
        // 計算したUV座標を使って、シャドウマップから深度値をサンプリング
        float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
        if (zInLVP > zInShadowMap)
        {
            // 遮蔽されている
            shadowMap.xyz *= 0.5f;
        }
    }
    float3 toon=CalcToonRatio(directionLight.direction,psIn.normal);
    float4 color = g_albedo.Sample(g_sampler, psIn.uv);
	//ライティングの結果をすべて加算する。
    float3 lig =  directionLig
                + pointLig
                + spotLig
                + ambientLight;
	
    
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	

	//albedoColor.xyz *= lig;
	albedoColor.xyz *=toon;
    
    float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
    if (shadowreceive == true&&zInShadowMap + 0.0001)
    {
        albedoColor.xyz *= shadowMap;
    }
 
	
	return albedoColor;
}
// モデル用のピクセルシェーダーのエントリーポイント
float4 PSMain(SPSIn psIn) : SV_Target0
{
    SPSOut psOut;

    psOut.color = PSMainCore(psIn, false);

    psOut.depth = psIn.depthInView;
    
    float2 vel = psIn.posInProj.xy / psIn.posInProj.w - psIn.prevPosInProj.xy / psIn.prevPosInProj.w;
    float velocityz = psIn.posInProj.z / psIn.posInProj.w - psIn.prevPosInProj.z / psIn.prevPosInProj.w;
    float4 velo = { vel, velocityz, 0 };
    psOut.velocity = velo;
    
    return psOut.color;
}
float4 PSMainShadowReciever(SPSIn psIn) : SV_Target0
{
    SPSOut psOut;

    psOut.color = PSMainCore(psIn, true);

    psOut.depth = psIn.depthInView;
    
    float2 vel = psIn.posInProj.xy / psIn.posInProj.w - psIn.prevPosInProj.xy / psIn.prevPosInProj.w;
    float velocityz = psIn.posInProj.z / psIn.posInProj.w - psIn.prevPosInProj.z / psIn.prevPosInProj.w;
    float4 velo = { vel, velocityz, 0 };
    psOut.velocity = velo;
    
    return psOut.color;
}