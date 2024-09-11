#define NOMIMAX

#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

Title::~Title()
{ 
	delete modelPlayer_;
	delete modelTitle_;
}

void Title::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("Title", true);
	modelPlayer_ = Model::CreateFromOBJ("player");

	viewProjection_.Initialize();

	const float kPlayerTitle = 2.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kPlayerTitle, kPlayerTitle, kPlayerTitle};

	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v < float>;
	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -10.0f;
}

void Title::Update()
{ 
	
	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		finished_ = true;
	}

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	viewProjection_.TransferMatrix();

	worldTransformTitle_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();

}

void Title::Draw()
{

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);
	Model::PostDraw();
}


