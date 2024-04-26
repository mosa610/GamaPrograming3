#include "Character.h"

//�s��X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

//�W�����v����
void Character::Jump(float speed)
{
    velocity.y = speed;
}

void Character::Move(float vx, float vz, float speed)
{
    /*speed *= elapsedTime;
    position.x += vx * speed;
    position.z += vz * speed;*/

    //�ړ������x�N�g��
    moveVecX = vx;
    moveVecZ = vz;

    //�ő呬�x�ݒ�
    maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.0001f) return;

    //�i�s�x�N�g����P�ʃx�N�g����
    vx /= length;
    vz /= length;


    //���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (vx * frontX) + (vz * frontZ);

    //���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0 - dot;
    if (rot > speed) { rot = speed; }
    //speed *= rot;
    //���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (vz * frontX) - (vx * frontZ);

    //2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //���E������s�����Ƃɂ���č��E��]��I������

    if (cross < 0.0f)
    {
        angle.y += rot;
    }
    else
    {
        angle.y -= rot;
    }

}

//���͏����X�V
void Character::UpdateVelocity(float elapsedTime)
{
    //�o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    //�������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    //�������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    //�����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    //�����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);





    ////�d�͏���
    //velocity.y += gravity * elapsedFrame;

    ////�ړ�����
    //position.y += velocity.y * elapsedTime;

    ////�n�ʔ���
    //if (position.y < 0.0f)
    //{
    //    position.y = 0.0f;
    //    velocity.y = 0.0f;

    //    //�ڒn����
    //    if (IsGround() == false)
    //    {
    //        OnLanding();
    //    }
    //    isGround = true;
    //}
    //else
    //{
    //    isGround = false;
    //}
}

void Character::UpdateInvincibleTime(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

bool Character::ApplyDamage(int damage, float invincibleTime)
{
        //�@�_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
        if (damage == 0) return false;

        //���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
        if (health <= 0) return false;
        if (invincibleTimer <= 0)
        {
            //�_���[�W����
            health -= damage;
            invincibleTimer = invincibleTime;
            //���S�ʒm
            if (health <= 0)
            {
                OnDead();
            }
            else
            {
                OnDamaged();
            }

            //���N��Ԃ��ύX�����ꍇ��true��Ԃ�
            return true;
        }
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //�d�͏���
    velocity.y += gravity * elapsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    //�ړ�����
    position.y += velocity.y * elapsedTime;

    //�n�ʔ���
    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        velocity.y = 0.0f;

        //�ڒn����
        if (IsGround() == false)
        {
            OnLanding();
        }
        isGround = true;
    }
    else
    {
        isGround = false;
    }
}

void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //�@XZ���ʂ̑��͂���������
    DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

    DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
    float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));
    if (length > 0.0f)
    {
        //���C��
        float friction = this->friction * elapsedFrame;

        //�󒆂ɂ���Ƃ��͖��C�͂����炷
        if (isGround == false) friction *= airControl;

        //���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            //���x��P�ʃx�N�g����
            auto v = DirectX::XMVector2Normalize(velVel);

            //�P�ʃx�N�g�����������x���@friction ���X�P�[�����O
            v = DirectX::XMVectorScale(v, friction);

            //�X�P�[�����O�����x�N�g���𑬓x�Ɍ��Z
            DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

            velocity.x = vel.x;
            velocity.z = vel.y;
        }
        //�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        //�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            //������
            float acceleration = this->acceleration * elapsedFrame;

            if (isGround == false) acceleration *= airControl;

            //�ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            

            //�ő呬�x����
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
        }
    }
    //�ړ��x�N�g�������Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsedTime)
{
    position.x += velocity.x * elapsedTime;
    position.z += velocity.z * elapsedTime;
}
