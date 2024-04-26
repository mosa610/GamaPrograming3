#pragma once

#include <DirectXMath.h>

//ƒRƒŠƒWƒ‡ƒ“
class Collision
{
public:
    //‹…‚Æ‹…‚ÌŒğ·”»’è
    static bool IntersectCylinderVsSphere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    //‹…‚Æ‰~’Œ‚ÌŒğ·”»’è
    static bool IntersectSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );
};