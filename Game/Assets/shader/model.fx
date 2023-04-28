/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const float PI = 3.1415926f; // π
// ディレクションライト
struct DirectionLight
{
    float3 direction; // ライトの方向
    float3 color; // ライトのカラー
    
};

// ポイントライト
struct PointLight
{
    float3 position; //位置
    float3 color; //カラー
    float range; //影響範囲
};

//スポットライト
struct SpotLight
{
    float3 position; //位置
    float3 color; //カラー
    float range; //影響範囲
    float3 direction; //方向
    float angle; //角度
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
    DirectionLight directionLight[4];
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
    float3 tangent  : TANGENT;      // 接ベクトル
    float3 biNormal : BINORMAL;     // 従ベクトル
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2; //カメラ空間の法線
    float4 posInLVP : TEXCOORD3; // ライトビュースクリーン空間でのピクセルの座標1
    float depthInView : TEXCOORD4;
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
float3 PBRdiffuse(float3 toEye, float3 lightDirection, float3 lightColor, float3 normal, float3 albedoColor);
float3 PBRSpecular(float3 toEye, float3 lightDirection, float3 lightColor, float3 normal, float smooth, float metallic, float3 specColor);

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(SPSIn psIn,float3 lightDirection, float3 lightColor,float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromDirectionLight(float3 toEye, float3 albedoColor, float smooth, float metallic, float3 normal, int LigNum);
float3 CalcNoPBRDirectionLight(SPSIn psIn, int ligNo);
float3 CalcLigFromSpotLight(SPSIn psIn);
float3 CalcRimLight(SPSIn psIn, float3 lightdirection, float3 lightcolor);
float4 DrawVelocityMap(SVSIn vsIn, SPSIn psIn);

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_normalMap : register(t1);//法線マップ
Texture2D<float4> g_specularMap : register(t2);//スペキュラーマップ
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



// ベックマン分布を計算する
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

// フレネルを計算。Schlick近似を使用
float SpcFresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// Cook-Torranceモデルの鏡面反射を計算
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metallic">金属度</param>
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
    float microfacet = 0.76f;

    // 金属度を垂直入射の時のフレネル反射率として扱う
    // 金属度が高いほどフレネル反射は大きくなる
    float f0 = metallic;

    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

    // 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

    // D項をベックマン分布を用いて計算する
    float D = Beckmann(microfacet, NdotH);

    // F項をSchlick近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

    // G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

    // m項を求める
    float m = PI * NdotV * NdotH;

    // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}
/// <summary>
/// フレネル反射を考慮した拡散反射を計算
/// </summary>
/// <remark>
/// この関数はフレネル反射を考慮した拡散反射率を計算します
/// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
/// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
/// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
///
/// </remark>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    // step-4 フレネル反射を考慮した拡散反射光を求める

    // 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める
    float dotNL = saturate(dot(N, L));

    // 法線と視線に向かうベクトルがどれだけ似ているかを内積で求める
    float dotNV = saturate(dot(N, V));

    // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
    return (dotNL * dotNV);
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
SPSIn VSSkinMain(SVSIn vsIn)
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
//物理ベースレンダリングをもとにした拡散反射光の計算
float3 PBRdiffuse(float3 toEye, float3 lightDirection, float3 lightColor, float3 normal,float3 albedoColor)
{
     // フレネル反射を考慮した拡散反射を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(
            normal, -lightDirection, toEye);

        // 正規化Lambert拡散反射を求める
    float NdotL = saturate(dot(normal, -lightDirection));
    float3 lambertDiffuse = lightColor * NdotL / PI;

        // 最終的な拡散反射光を計算する
    float3 diffuse = albedoColor * diffuseFromFresnel*lambertDiffuse;
    return diffuse;
}

//物理ベースレンダリングをもとにした鏡面反射光の計算
float3 PBRSpecular(float3 toEye, float3 lightDirection, float3 lightColor, float3 normal, float smooth, float metallic, float3 specColor)
{
    float3 spec = CookTorranceSpecular(
            -lightDirection, toEye, normal, smooth)
            * lightColor;

        // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
        // スペキュラカラーの強さを鏡面反射率として扱う
    spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);
    return spec;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(SPSIn psIn,float3 lightDirection, float3 lightColor,float3 normal)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);
 

    // 鏡面反射の強さを求める
    float t = dot(refVec, psIn.toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f) * 5.5f;
    //スペキュラーマップからサンプリング。
    float specPower = g_specularMap.Sample(g_sampler, psIn.uv).a;
    lightColor *= specPower;
 
    // 鏡面反射光を求める
    return lightColor * t;
}
/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromDirectionLight(float3 toEye, float3 albedoColor, float smooth, float metallic, float3 normal,int LigNum)
{
    float3 diffDirection = PBRdiffuse(toEye, directionLight[LigNum].direction, directionLight[LigNum].color, normal, albedoColor);
    float3 specDirection = PBRSpecular(toEye, directionLight[LigNum].direction, directionLight[LigNum].color, normal, smooth, metallic, albedoColor);
        
    return diffDirection * (1.0f - smooth) + specDirection;
}
float3 CalcNoPBRDirectionLight(SPSIn psIn,int ligNo)
{
        // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(directionLight[ligNo].direction, directionLight[ligNo].color, psIn.normal);
    float3 dir = directionLight[ligNo].direction;
    dir.xz *= -1.0f;
    diffDirection += CalcLambertDiffuse(dir, directionLight[ligNo].color, psIn.normal);
    
    //ディレクションライトによるPhong鏡面反射光を計算する

    float3 specDirection = CalcPhongSpecular(psIn, directionLight[ligNo].direction, directionLight[ligNo].color, psIn.normal);
    specDirection += CalcPhongSpecular(psIn, dir, directionLight[ligNo].color, psIn.normal);
    
    //ディレクションライトによるリムライトを計算する。
    float3 rimDirection = CalcRimLight(psIn, directionLight[ligNo].direction, directionLight[ligNo].color);
   
    return diffDirection + specDirection + rimDirection;
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
        psIn,
        ligDir, // ライトの方向
        pointLight.color, // ライトのカラー
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
        psIn,
        ligDir, // ライトの方向
        spotLight.color, // ライトのカラー
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
    if (affect < 0.0f)
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
float3 CalcRimLight(SPSIn psIn, float3 direction, float3 color)
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
float4 DrawVelocityMap(SVSIn vsIn, SPSIn psIn)
{
    
    float4 now_pos, prev_pos;
      
    /*float4x4 nowWV = mul(mView, mWorld);
    float4x4 nowWVP = mul(mProj, nowWV);
     
    float4x4 prevWV = mul(mPrevView, mPrevWorld);
    float4x4 prevWVP = mul(mPrevProj, prevWV);
        
    now_pos = mul(nowWVP, vsIn.pos);
    prev_pos = mul(prevWVP, vsIn.pos);
    */
    
    now_pos = mul(mWorld, vsIn.pos);
    now_pos = mul(mView, now_pos);
    now_pos = mul(mProj, now_pos);
    
    prev_pos = mul(mPrevWorld, vsIn.pos);
    prev_pos = mul(mPrevView, prev_pos);
    prev_pos = mul(mPrevProj, prev_pos);
    
    float2 velocity = now_pos.xy / now_pos.w - prev_pos.xy / prev_pos.w;
    
   //velocity *= .0f;
    
   float4 velo = { velocity, 0.0f, 0.0f };
   
    return velo;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMainCore(SPSIn psIn, uniform bool shadowreceive, uniform bool hasNormalMap)
{
    
    if (hasNormalMap)
    {
        psIn.normal = normalize(GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv));
    }
      
     
    float3 toEye = eyePos - psIn.worldPos;
    toEye = normalize(toEye);
    
    // step-2 各種マップをサンプリングする
    // アルベドカラー（拡散反射光）
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
    //return albedoColor;
    // スペキュラカラーはアルベドカラーと同じにする
    float3 specColor = albedoColor;

    // 金属度
    float metallic = g_specularMap.Sample(g_sampler, psIn.uv).r;
    //metallic = 0.0f;

    // 滑らかさ
    float smooth = g_specularMap.Sample(g_sampler, psIn.uv).a;
    //smooth = 1.0f;
    
    float3 lig;
    for (int ligNo = 0; ligNo < 4; ligNo++)
    {
        //ディレクションライトによるライティングを計算する
        float3 directionLig = CalcLigFromDirectionLight(toEye, albedoColor.xyz, smooth, metallic, psIn.normal, ligNo);
        lig += directionLig;
        //float3 directionLig = CalcNoPBRDirectionLight(psIn,ligNo);
        //lig += directionLig;
    }
    //// ポイントライトによるライティングを計算する
    //float3 pointLig = CalcLigFromPointLight(psIn);

    ////スポットライトによるライティングを計算する。
    //float3 spotLig = CalcLigFromSpotLight(psIn);
   
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
        if (zInLVP > zInShadowMap + 0.0001)
        {
            // 遮蔽されている
            shadowMap.xyz *= 0.5f;
        }
    }
   
   
	//ライティングの結果をすべて加算する。
    lig +=      //+ pointLig
                //+ spotLig
                + ambientLight;
	
    
    // step-2 ディザパターンを利用してディザリングを実装する。
    //このピクセルのスクリーン座標系でのX座標、Y座標を4で割った余りを求める。
    int x = (int) fmod(psIn.pos.x, 4.0f);
    int y = (int) fmod(psIn.pos.y, 4.0f);

    //上で求めた、xとyを利用して、このピクセルのディザリング閾値を取得する。
    int dither = pattern[y][x];

    clip(dither - 64*clipratio);


    //lig += albedoColor.xyz;

    albedoColor.xyz *= lig;
    if (shadowreceive == true)
    {
        //albedoColor.xyz *= shadowMap;
    }
    
    return albedoColor;
    
  
}
// モデル用のピクセルシェーダーのエントリーポイント
SPSOut PSMain(SPSIn psIn)
{
    SPSOut psOut;

    psOut.color = PSMainCore(psIn, false, true);
    psOut.depth = psIn.depthInView;
    
    float2 vel = psIn.posInProj.xy / psIn.posInProj.w - psIn.prevPosInProj.xy / psIn.prevPosInProj.w;
    float velocityz = psIn.posInProj.z / psIn.posInProj.w - psIn.prevPosInProj.z / psIn.prevPosInProj.w;
    float4 velo = { vel, velocityz, 0 };
    psOut.velocity = velo;
    
    return psOut;
}
SPSOut PSMainShadowReciever(SPSIn psIn)
{
    SPSOut psOut;

    psOut.color = PSMainCore(psIn, true, true);

    psOut.depth = psIn.depthInView;
    
    float2 vel = psIn.posInProj.xy / psIn.posInProj.w - psIn.prevPosInProj.xy / psIn.prevPosInProj.w;
    float velocityz = psIn.posInProj.z / psIn.posInProj.w - psIn.prevPosInProj.z / psIn.prevPosInProj.w;
    float4 velo = { vel, velocityz, 0 };
    psOut.velocity = velo;
    
    return psOut;
}

SPSOut PSMainShadowRecieverNoNormalMap(SPSIn psIn)
{
    SPSOut psOut;

    psOut.color = PSMainCore(psIn, true, false);

    psOut.depth = psIn.depthInView;
    
    float2 vel = psIn.posInProj.xy / psIn.posInProj.w - psIn.prevPosInProj.xy / psIn.prevPosInProj.w;
    float velocityz = psIn.posInProj.z / psIn.posInProj.w - psIn.prevPosInProj.z / psIn.prevPosInProj.w;
    float4 velo = { vel, velocityz, 0 };
    psOut.velocity = velo;
    
    return psOut;
}