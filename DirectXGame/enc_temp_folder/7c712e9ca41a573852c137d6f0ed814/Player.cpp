#define NOMINMAX

#include "Player.h"
#include "DirectXCommon.h"
#include "Easing.h"
#include "Input.h"
#include "MapChipField.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <algorithm>
#include <cassert>
#include <numbers>

void Player::Initialize(ViewProjection* viewProjection, const Vector3& position) {
	viewprojection_ = viewProjection;
	worldtransform_.Initialize();
	worldtransform_.translation_ = position;
	worldtransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	model_ = Model::CreateFromOBJ("player", true);
}

void Player::Update() {

	Move();

	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.move = velocity_;

	CheckMapCollision(collisionMapInfo);

	worldtransform_.translation_ += collisionMapInfo.move;

	if (collisionMapInfo.ceiling) {
		velocity_.y = 0;
	}

	if (collisionMapInfo.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}

	UpdateOnGround(collisionMapInfo);

	worldtransform_.UpdateMatrix();
	worldtransform_.TransferMatrix();
}

void Player::CheckMapCollision(CollisionMapInfo& info) { 
	CheckMapCollisionUp(info); 
	CheckMapCollisionDown(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {
	if (info.move.y <= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldtransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldtransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldtransform_.translation_.y - (kHeight / 2.0f + kBlank));

		info.ceiling = true;
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldtransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;

	// 左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldtransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldtransform_.translation_.y + (kHeight / 2.0f + kBlank));

		info.landing = true;
	}
}

void Player::UpdateOnGround(const CollisionMapInfo& info) { 
	if (onGround_)
	{
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}
		else
		{
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldtransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			bool ground = false;

			MapChipType mapChipType;

			MapChipField::IndexSet indexSet;

			// 左下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0,-kGroundSearchHeight,0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				ground = true;
			}

			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				ground = true;
			}

			if (!ground) {
				onGround_ = false;
			}
		}
	} 
	else
	{
		if (info.landing) {
			onGround_ = true;

			velocity_.x *= (1.0f - kAttenuationLanding);

			velocity_.y = 0.0f;
		}
	}

}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::Move() {
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
			velocity_ += Vector3(0, kJumpAcceleration / 60.0f, 0); //
			                                                       // velocity_.x += 0;
			// velocity_.y += kJumpAcceleration;
			// velocity_.z += 0;
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration / 60.0f, 0); //
		// velocity_.x += 0;
		// velocity_.y += -kGravityAcceleration;
		// velocity_.z += 0;

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		landing = false;

		if (velocity_.y < 0) {
			if (worldtransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}
	}

	worldtransform_.translation_ += velocity_;
	// worldtransform_.translation_.x += velocity_.x;
	// worldtransform_.translation_.y += velocity_.y;
	// worldtransform_.translation_.z += velocity_.z;

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldtransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
}

void Player::Draw() { model_->Draw(worldtransform_, *viewprojection_); }
