/*!
 * @brief �u���[��
 */

cbuffer cb : register(b0)
{
    float4x4 mvp; // MVP�s��
    float4 mulColor; // ��Z�J���[
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
 * @brief ���_�V�F�[�_�[
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}
Texture2D<float4> g_scenedepthTextuere : register(t0);//�V�[���̐[�x�e�N�X�`��

Texture2D<float4> g_depthTexture : register(t1); //�[�x�l�e�N�X�`��

sampler g_sampler : register(s0); //�T���v���[

/*!
 *@brief    �s�N�Z���V�F�[�_�[�ւ̓��́B
 */

float4 PSMain(PSInput psIn) : SV_Target0
{
   

    // �ߖT8�e�N�Z���ւ�UV�I�t�Z�b�g
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 900.0f), //��
        float2(0.0f, -1.0f / 900.0f), //��
        float2(1.0f / 1600.0f, 0.0f), //�E
        float2(-1.0f / 1600.0f, 0.0f), //��
        float2(1.0f / 1600.0f, 1.0f / 900.0f), //�E��
        float2(-1.0f / 1600.0f, 1.0f / 900.0f), //����
        float2(1.0f / 1600.0f, -1.0f / 900.0f), //�E��
        float2(-1.0f / 1600.0f, -1.0f / 900.0f) //����
    };

    // ���̃s�N�Z���̐[�x�l���擾
    float depth = g_depthTexture.Sample(g_sampler, psIn.uv).x;

    // �ߖT8�e�N�Z���̐[�x�l�̕��ϒl���v�Z����
    float depth2 = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_depthTexture.Sample(g_sampler, psIn.uv + uvOffset[i]).x;
    }
    depth2 /= 8.0f;
    float scenedepth = g_scenedepthTextuere.Sample(g_sampler, psIn.uv).r;
    // ���g�̐[�x�l�ƋߖT8�e�N�Z���̐[�x�l�̍��𒲂ׂ�
    if (abs(depth - depth2) > 0.015f&& psIn.pos.z > depth-0.01f)
    {
        
        // �[�x�l�����\�Ⴄ�ꍇ�̓s�N�Z���J���[�����ɂ���
        // ->���ꂪ�G�b�W�J���[�ƂȂ�
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
   

    return float4(1.0f, 1.0f, 1.0f, 0.0f);

}
