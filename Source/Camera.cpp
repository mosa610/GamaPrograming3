#include <imgui.h>
#include "Camera.h"

//�w�����������
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const
    DirectX::XMFLOAT3& up)
{
    //���_�A�����_�A���������̃r���[�s����쐬
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view, View);

    //�r���[�s����t�s�񉻂��A���[���h�s��ɖ߂�
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    //�J�����̕��������o��
    this->right.x = world.m[0][0];
    this->right.y = world.m[0][1];
    this->right.z = world.m[0][2];

    this->up.x = world.m[1][0];
    this->up.y = world.m[1][1];
    this->up.z = world.m[1][2];

    this->front.x = world.m[2][0];
    this->front.y = world.m[2][1];
    this->front.z = world.m[2][2];

    //���_�A�����_��ۑ�
    this->eye = eye;
    this->focus = focus;

}

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
    //��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}

