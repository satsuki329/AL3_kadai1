#include "Player.h"
#include "TextureManager.h"
#include <cassert>
#include <numbers>

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) { 
	assert(model); 
	model_ = model;
	viewprojection_ = viewProjection;
	worldtransform_.Initialize();
	worldtransform_.translation_ = position;
	worldtransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update()
{ 
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
	{
		Vector3 accelerration = {};

		if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			if (velocity_.x < 0.0f)
			{
				velocity_.x *= (1.0f - kAttenuation);
			}
			accelerration.x += kAcceleration;

			if (lrDirection_ != LRDirection::kRight)
			{
				lrDirection_ = LRDirection::kRight;
			}
		} 
		else if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			if (velocity_.x > 0.0f) 
			{
				velocity_.x *= (1.0f - kAttenuation);
			}

			accelerration.x -= kAcceleration;

			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
			}
		} 
		else
		{
			velocity_.x *= (1.0f - kAttenuation);
		}

		velocity_ += accelerration;

		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}

		
	worldtransform_.translation_ += velocity_;

	worldtransform_.UpdateMatrix(); 
	worldtransform_.TransferMatrix();
}

void Player::Draw() 
{ 
	model_->Draw(worldtransform_,*viewprojection_,texturehandle_);
}