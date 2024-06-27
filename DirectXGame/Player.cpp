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
	worldtransform_.UpdateMatrix(); 
	worldtransform_.TransferMatrix();
}

void Player::Draw() 
{ 
	model_->Draw(worldtransform_,*viewprojection_,texturehandle_);
}