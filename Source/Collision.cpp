#include "Collision.h"

//�~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    //A�̑�����B�̓�����Ȃ瓖�����ĂȂ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    //A�̓���B�̑�����艺�Ȃ瓖�����ĂȂ�
    if(positionA.y+heightA<positionB.y)
    {
        return false;
    }

    DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
    DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);

    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);

    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    // Vec �̒������v�Z(XMVector2LengthSq �𗘗p���Ă��ǂ�)
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // radiusA �� radiusB �̒���
    float range = radiusA + radiusB;

    // radisuA + radisuB ���傫��������
    if (lengthSq > range)
    {
        // �������ĂȂ�
        return false;
    }

    // A �� B �������o���iB �͉��ɉ����o���Ă͂����Ȃ��j

    // �������Ă���ꍇ

    // Vec �����̒P�ʃx�N�g���iNormalize�j���擾
    Vec = DirectX::XMVector2Normalize(Vec);
    // ��L�̃x�N�g���� range ���X�P�[��
    Vec = DirectX::XMVectorScale(Vec, range);
    // ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
    Vec = DirectX::XMVectorAdd(PositionA, Vec);
    // �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);

    outPositionB.x = resultPos.x;
    outPositionB.y = positionB.y;
    outPositionB.z = resultPos.y;

    return true;


    ////A��B�̒P�ʃx�N�g�����Z�o
    //DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    //DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    //DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    //DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    //float lengthSq;
    //DirectX::XMStoreFloat(&lengthSq, LengthSq);

    ////��������
    //float range = radiusA + radiusB;
    //if (lengthSq > range)
    //{
    //    return false;
    //}

    ////A��B�������o��
    ////A��B�x�N�g���𐳋K��
    //Vec = DirectX::XMVector3Normalize(Vec);
    ////�x�N�g����range���X�P�[��
    //Vec = DirectX::XMVectorScale(Vec, range);
    ////
    //Vec = DirectX::XMVectorAdd(PositionA, Vec);

    //DirectX::XMStoreFloat3(&outPositionB, Vec);

}



bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition,
    float sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition
)
{
    // ���̓�(.y + radius )���~���̑���(.y)��艺�Ȃ� false
    if (spherePosition.y + sphereRadius < cylinderPosition.y)
        return false;

    // ���̑���( .y - radius)���~���̓�(.y + Height)����Ȃ� false
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderRadius)
        return false;

    // XZ ���ʂł͈̔̓`�F�b�N
    // positionB - positionA �̋�����
    DirectX::XMFLOAT2 posA(spherePosition.x, spherePosition.z);
    DirectX::XMFLOAT2 posB(cylinderPosition.x, cylinderPosition.z);

    // posA �� XMVECTOR �ɕϊ�
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
    // posB �� XMVECTOR �ɕϊ�
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
    // posA ���� posB �ւ̃x�N�g�����v�Z(posB - posA)
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    // Vec �̒������v�Z(XMVector2LengthSq �𗘗p���Ă��ǂ�)
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // radiusA �� radiusB �̒���
    float range = sphereRadius + cylinderRadius;

    // radisuA + radisuB ���傫��������
    if (lengthSq > range)
    {
        // �������ĂȂ�
        return false;
    }

    // A �� B �������o���iB �͉��ɉ����o���Ă͂����Ȃ��j

    // �������Ă���ꍇ
    // Vec �����̒P�ʃx�N�g���iNormalize�j���擾
    Vec = DirectX::XMVector2Normalize(Vec);
    // ��L�̃x�N�g���� range ���X�P�[��
    Vec = DirectX::XMVectorScale(Vec, range);
    // ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
    Vec = DirectX::XMVectorAdd(PositionA, Vec);
    // �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);


    outCylinderPosition.x = spherePosition.x + resultPos.x;
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + resultPos.y;

    return true;
}