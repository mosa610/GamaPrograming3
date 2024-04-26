#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"

Player::Player()
{
    model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    scale.x = scale.y = scale.z = 0.01f;
}

Player::~Player()
{
    delete model;
}

void Player::Update(float elapsedTime)
{
    ////�i�s�x�N�g���擾
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //
    ////�ړ�����
    //float moveSpeed = this->moveSpeed * elapsedTime;
    //position.x += moveVec.x * moveSpeed;
    //position.z += moveVec.z * moveSpeed;

    //�ړ����͏���
    InputMove(elapsedTime);

    ////���͏��̎擾
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //float ax = gamePad.GetAxisLX();
    //float ay = gamePad.GetAxisLY();
    //
    ////�ړ�����
    //float moveSpeed = 5.0f * elapsedTime;
    //{
    //    if (ax <0)
    //    {
    //        position.x -= moveSpeed;
    //    }
    //    if (ax >0)
    //    {
    //        position.x += moveSpeed;
    //    }
    //    if (ay >0)
    //    {
    //        position.z += moveSpeed;
    //    }
    //    if (ay <0)
    //    {
    //        position.z -= moveSpeed;
    //    }
    //}
    //
    //float rotateSpeed = DirectX::XMConvertToRadians(360) * elapsedTime;
    //if (gamePad.GetButton() & GamePad::BTN_A)
    //{
    //    angle.x += rotateSpeed;
    //}
    //if (gamePad.GetButton() & GamePad::BTN_B)
    //{
    //    angle.y += rotateSpeed;
    //}
    //if (gamePad.GetButton() & GamePad::BTN_X)
    //{
    //    angle.z += rotateSpeed;
    //}


    //�W�����v����
    InputJump();

    //�e�ۓ��͏���
    InputProjectile();

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);

    //�v���C���[�ƓG�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();

    //�e�ۂƓG�̏Փˏ���
    CollisionProjectilesVsEnemies();

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

//void Player::Move(float elapsedTime, float vx, float vz, float speed)
//{
//    speed *= elapsedTime;
//    position.x += vx * speed;
//    position.z += vz * speed;
//}

void Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

//void Player::Turn(float elapsedTime, float vx, float vz, float speed)
//{
//    speed *= elapsedTime;
//
//    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
//    float length = sqrtf(vx * vx + vz * vz);
//    if (length<0.0001f) return;
//
//    //�i�s�x�N�g����P�ʃx�N�g����
//    vx /= length;
//    vz /= length;
//
//
//    //���g�̉�]�l����O���������߂�
//    float frontX = sinf(angle.y);
//    float frontZ = cosf(angle.y);
//
//    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
//    float dot = (vx * frontX) + (vz * frontZ);
//
//    //���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
//    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
//    float rot = 1.0 - dot;
//    if (rot > speed) { rot = speed; }
//    //speed *= rot;
//    //���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
//    float cross = (vz * frontX) - (vx * frontZ);
//
//    //2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
//    //���E������s�����Ƃɂ���č��E��]��I������
//
//    if (cross < 0.0f)
//    {
//        angle.y += rot;
//    }
//    else
//    {
//        angle.y -= rot;
//    }
//
//}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    //�e�ە`�揈��
    projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("Position", &position.x);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //�X�P�[��
            ImGui::InputFloat3("Scale", &scale.x);
        }
    }
    ImGui::End();
}

void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::instance();

    //���ׂĂ̓G�Ƒ�������ŏՓ˔���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsSphere(
            position, radius,height,
            enemy->GetPosition(), enemy->GetRadius(),enemy->GetHeight(),
            outPosition))
        {
            enemy->SetPosition(outPosition);
            if (position.y >= (enemy->GetPosition().y + enemy->GetHeight())-0.1f)
            {
                Jump(jumpSpeed);
                enemy->ApplyDamage(1, 0.5f);
            }
        }
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::instance();

    //���ׂĂ̒e�ۂƂ��ׂĂ̓G������������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                //�_���[�W��^����
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    //������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        //������΂���
                        const float power = 10.0f;

                        //�G�̈ʒu
                        DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                        //�e�̈ʒu
                        DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
                        //�e����G�ւ̕����x�N�g�����v�Z�i�G - �e�j
                        auto v = DirectX::XMVectorSubtract(eVec, pVec);
                        //�����x�N�g���𐳋K��
                        v = DirectX::XMVector3Normalize(v);

                        DirectX::XMFLOAT3 vec;
                        DirectX::XMStoreFloat3(&vec, v);

                        impulse.x = power * vec.x;
                        impulse.y = power * 0.5f;
                        impulse.z = power * vec.z;

                        enemy->AddImpulse(impulse);
                    }
                    //�e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }
}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�e�ۃf�o�b�O�v���~�e�B�u�`��
    projectileManager.DrawDebugPrimitive();
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏��̎擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //�J���������ƃX�e�B�b�N�̓��͂ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XY���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /=  cameraRightLength;
        cameraRightZ /=  cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //�X�e�B�b�N�̐������͒l���J�����̉E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����̑O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX*ax) + (cameraFrontX*ay);
    vec.z = (cameraRightZ*ax) + (cameraFrontZ*ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

////�W�����v����
//void Player::Jump(float speed)
//{
//    velocity.y = speed;
//}
//
////���͏����X�V
//void Player::UpdateVelocity(float elapsedTime)
//{
//    //�o�߃t���[��
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    //�d�͏���
//    velocity.y += gravity * elapsedFrame;
//
//    //�ړ�����
//    position.y += velocity.y * elapsedTime;
//
//    //�n�ʔ���
//    if (position.y < 0.0f)
//    {
//        position.y = 0.0f;
//        velocity.y = 0.0f;
//    }
//}

void Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount == jumpLimit)
        {
            return;
        }
        jumpCount++;
        Jump(jumpSpeed);
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}

void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //���i�e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = transform._31 * 100.0f;
        dir.y = 0.0f;
        dir.z = transform._33 * 100.0f;
        //���ˈʒu�i�v���C���[�̍�������j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;
        //����
        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);
    }
    //�ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = transform._31 * 100.0f;
        dir.y = 0.0f;
        dir.z = transform._33 * 100.0f;
        //���ˈʒu�i�v���C���[�̍�������j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O��
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; i++)
        {
            //�G�Ƃ̋�������
            Enemy* enemy = EnemyManager::instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
                
            }
        }

        //����
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
}