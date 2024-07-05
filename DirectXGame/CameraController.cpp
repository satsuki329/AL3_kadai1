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
	Vector3 targetVelocity = target_->GetVelocity();


	destination_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias_;
	destination_.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias_;
	destination_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias_;
	

	viewprojection_.translation_.x = Lerp(viewprojection_.translation_.x, destination_.x, kInterpolationRate_);
	viewprojection_.translation_.y = Lerp(viewprojection_.translation_.y, destination_.y, kInterpolationRate_);
	viewprojection_.translation_.z = Lerp(viewprojection_.translation_.z, destination_.z, kInterpolationRate_);

    viewprojection_.translation_.x = std::clamp(viewprojection_.translation_.x, targetWorldTransform.translation_.x + targetMargin.left, targetWorldTransform.translation_.x + targetMargin.right);
	viewprojection_.translation_.y = std::clamp(viewprojection_.translation_.y, targetWorldTransform.translation_.y + targetMargin.bottom, targetWorldTransform.translation_.y + targetMargin.top);
	viewprojection_.translation_.x = std::clamp(viewprojection_.translation_.x, movableArea_.left, movableArea_.right);
	viewprojection_.translation_.y = std::clamp(viewprojection_.translation_.y, movableArea_.bottom, movableArea_.top);

	viewprojection_.UpdateMatrix();
}

void CameraController::Reset() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewprojection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewprojection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewprojection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}