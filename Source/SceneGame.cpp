#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"


// ������
void SceneGame::Initialize()
{
	stage = new Stage();

	player = new Player();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	//�J�����R���g���[���[������
	cameraController = new CameraController();

	//�G�l�~�[������
	//Enemy* enemy = new EnemySlime;
	//enemy->SetPosition({ 0,0,5 });
	//EnemyManager::instance().Register(enemy);
	for (int i = 0; i < 2; i++)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		EnemyManager::instance().Register(slime);
	}

}

// �I����
void SceneGame::Finalize()
{
	//�G�l�~�[�I����
	EnemyManager::instance().clear();
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	stage->Update(elapsedTime);
	player->Update(elapsedTime);

	//�G�l�~�[�X�V����
	EnemyManager::instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//// �r���[�s��
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// �J�����̎��_�i�ʒu�j
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// �J�����̒����_�i�^�[�Q�b�g�j
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// �J�����̏����
	//
	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// �v���W�F�N�V�����s��
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// ����p
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// ��ʏc���䗦
	//	float nearZ = 0.1f;	// �J�������f���o���̍ŋߋ���
	//	float farZ = 1000.0f;	// �J�������f���o���̍ŉ�����
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		stage->Render(dc, shader);

		//�G�l�~�[�`��
		EnemyManager::instance().Render(dc, shader);

		//�v���C���[�`��
		player->Render(dc, shader);
		shader->End(dc);

	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();

		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{

	}

	// 2D�f�o�b�OGUI�`��
	{
		//�v���C���[�f�o�b�O�`��
		player->DrawDebugGUI();
		cameraController->DrawDebugGUI();
	}
}
