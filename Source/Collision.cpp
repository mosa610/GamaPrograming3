#include "Collision.h"

//円柱と円柱の交差判定
bool Collision::IntersectCylinderVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    //Aの足元がBの頭より上なら当たってない
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    //Aの頭がBの足元より下なら当たってない
    if(positionA.y+heightA<positionB.y)
    {
        return false;
    }

    DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
    DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);

    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);

    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    // Vec の長さを計算(XMVector2LengthSq を利用しても良い)
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // radiusA と radiusB の長さ
    float range = radiusA + radiusB;

    // radisuA + radisuB より大きかったら
    if (lengthSq > range)
    {
        // 当たってない
        return false;
    }

    // A が B を押し出す（B は下に押し出してはいけない）

    // あたっている場合

    // Vec 方向の単位ベクトル（Normalize）を取得
    Vec = DirectX::XMVector2Normalize(Vec);
    // 上記のベクトルを range 分スケール
    Vec = DirectX::XMVectorScale(Vec, range);
    // そのベクトルを位置 A （PositionA）からの足した位置に移動
    Vec = DirectX::XMVectorAdd(PositionA, Vec);
    // 出力用の位置（outPositionB）に代入する
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);

    outPositionB.x = resultPos.x;
    outPositionB.y = positionB.y;
    outPositionB.z = resultPos.y;

    return true;


    ////A→Bの単位ベクトルを算出
    //DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    //DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    //DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    //DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    //float lengthSq;
    //DirectX::XMStoreFloat(&lengthSq, LengthSq);

    ////距離判定
    //float range = radiusA + radiusB;
    //if (lengthSq > range)
    //{
    //    return false;
    //}

    ////AがBを押し出す
    ////A→Bベクトルを正規化
    //Vec = DirectX::XMVector3Normalize(Vec);
    ////ベクトルをrange分スケール
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
    // 球の頭(.y + radius )が円柱の足元(.y)より下なら false
    if (spherePosition.y + sphereRadius < cylinderPosition.y)
        return false;

    // 球の足元( .y - radius)が円柱の頭(.y + Height)より上なら false
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderRadius)
        return false;

    // XZ 平面での範囲チェック
    // positionB - positionA の距離が
    DirectX::XMFLOAT2 posA(spherePosition.x, spherePosition.z);
    DirectX::XMFLOAT2 posB(cylinderPosition.x, cylinderPosition.z);

    // posA を XMVECTOR に変換
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
    // posB を XMVECTOR に変換
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
    // posA から posB へのベクトルを計算(posB - posA)
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    // Vec の長さを計算(XMVector2LengthSq を利用しても良い)
    DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // radiusA と radiusB の長さ
    float range = sphereRadius + cylinderRadius;

    // radisuA + radisuB より大きかったら
    if (lengthSq > range)
    {
        // 当たってない
        return false;
    }

    // A が B を押し出す（B は下に押し出してはいけない）

    // あたっている場合
    // Vec 方向の単位ベクトル（Normalize）を取得
    Vec = DirectX::XMVector2Normalize(Vec);
    // 上記のベクトルを range 分スケール
    Vec = DirectX::XMVectorScale(Vec, range);
    // そのベクトルを位置 A （PositionA）からの足した位置に移動
    Vec = DirectX::XMVectorAdd(PositionA, Vec);
    // 出力用の位置（outPositionB）に代入する
    DirectX::XMFLOAT2 resultPos;
    DirectX::XMStoreFloat2(&resultPos, Vec);


    outCylinderPosition.x = spherePosition.x + resultPos.x;
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = spherePosition.z + resultPos.y;

    return true;
}