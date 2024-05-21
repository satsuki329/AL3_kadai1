#include "Player.h"
#include "TextureManager.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t texturehandle, ViewProjection* viewProjection) { 
	assert(model); 
	model_ = model;
	texturehandle_ = texturehandle;
	viewprojection_ = viewProjection;
	worldtransform_.Initialize();
}

void Player::Update()
{ 
	worldtransform_.TransferMatrix(); 

}

void Player::Draw() 
{ 
	model_->Draw(worldtransform_,*viewprojection_,texturehandle_);
}