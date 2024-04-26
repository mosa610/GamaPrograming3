#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"

class Player : public Character
{
public:
    Player();
    ~Player() override;

    //�X�V����
    void Update(float olapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�f�o�b�O�pGUI
    void DrawDebugGUI();
private:
    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    //�ړ�����
    //void Move(float elapsedTime, float vx, float vz, float speed);

    //�ړ����͏���
    void InputMove(float elapsedTime);

    //���񏈗�
    //void Turn(float elapsedTime, float vx, float vz, float speed);

    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    void CollisionProjectilesVsEnemies();

    //�W�����v����
    //void Jump(float speed);

    //���͏����X�V
    //void UpdateVelocity(float elapsedTime);

    //�e�ۓ��͏���
    void InputProjectile();

protected:

    void OnLanding() override;

    //�W�����v���͏���
    void InputJump();
private:
    Model* model = nullptr;
    float       moveSpeed = 5.0f;
    float       turnSpeed = DirectX::XMConvertToRadians(720);
    float       jumpSpeed = 20.0f;
    /*float       gravity = -1.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 };*/

    int         jumpCount = 0;
    int         jumpLimit = 2;

    ProjectileManager projectileManager;
};