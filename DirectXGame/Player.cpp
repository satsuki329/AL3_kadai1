#define NOMINMAX

#include "Player.h"
#include "Easing.h"
#include "TextureManager.h"
#include <algorithm>
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

void Player::Update() {
	if (onGround_ == true) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 accelerration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				accelerration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					turnFirstRotationY_ = worldtransform_.rotation_.y;

					turntimer_ = 0.7f;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				accelerration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;

					turnFirstRotationY_ = worldtransform_.rotation_.y;

					turntimer_ = 0.7f;
				}
			}
			velocity_.x += accelerration.x;
			velocity_.y += accelerration.y;
			velocity_.z += accelerration.z;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

			if (accelerration.x >= 0.01f || accelerration.x <= -0.01f) {
				accelerration.x = 0;
			}

			if (turntimer_ > 0.0f) {
				turntimer_ -= 1.0f / 60.0f;

				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

				worldtransform_.rotation_.y = Easing::Liner(destinationRotationY, turnFirstRotationY_, Easing::EaseInOut(turntimer_));
			}

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAcceleration, 0);
			velocity_.x += 0;
			velocity_.y += kJumpAcceleration;
			velocity_.z += 0;
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		landing = false;

		if (velocity_.y < 0) {
			if (worldtransform_.translation_.y <= 2.0f) {
				landing = true;
			}
		}
	}

	worldtransform_.translation_.x += velocity_.x;
	worldtransform_.translation_.y += velocity_.y;
	worldtransform_.translation_.z += velocity_.z;

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldtransform_.translation_.y = 2.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	worldtransform_.UpdateMatrix();
	worldtransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldtransform_, *viewprojection_); }