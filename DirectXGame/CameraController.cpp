#include "CameraController.h"
#include "Player.h"
#include <algorithm>

void CameraController::Initialize()
{ 
	viewprojection_.Initialize();
}


void CameraController::Update() 
{ 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewprojection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

	viewprojection_.UpdateMatrix();
}

void CameraController::Reset() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewprojection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewprojection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewprojection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}