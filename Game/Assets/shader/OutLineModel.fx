/*!
 * @brief	�֊s���p�̃��f���V�F�[�_�[
 */
////////////////////////////////////////////////
// �萔�o�b�t�@�B
////////////////////////////////////////////////
//���f���p�̒萔�o�b�t�@
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
// �\����
////////////////////////////////////////////////
//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn
{
    float4 pos : POSITION; //���f���̒��_���W�B
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0; //UV���W�B
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;

};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float3 normal : NORMAL; //�@���B
    float2 uv : TEXCOORD0; //uv���W�B
    float3 tangent : TANGENT; // �ڃx�N�g��
    float3 biNormal : BINORMAL; // �]�x�N�g��
    float depthInView : TEXCOORD1;
    
};

struct SPSOut
{
    float4 depth : SV_Target0;
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_normalMap : register(t1); //�@���}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B

/// <summary>
//�X�L���s����v�Z����B
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
/// ���_�V�F�[�_�[�̃R�A�֐��B
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
    psIn.normal = normalize(mul(m, vsIn.normal)); //�@������]������B
    float4 defaultpos = vsIn.pos;
    vsIn.pos += float4(vsIn.normal*0.1f, 0); //���_���W��@�������ɉ����o�����
    
    float4 worldpos = mul(m, defaultpos);
    psIn.depthInView = mul(mView, worldpos).z;
    // step-4 �ڃx�N�g���Ə]�x�N�g�������[���h��Ԃɕϊ�����
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.depthInView = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    //���_�@�����s�N�Z���V�F�[�_�[�ɓn���B
    psIn.normal = normalize(mul(m, vsIn.normal)); //�@������]������B
    psIn.uv = vsIn.uv;
    return psIn;
}
/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
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