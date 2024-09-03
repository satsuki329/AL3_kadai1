#pragma once

#include "AABB.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class MapChipField;

class Enemy;

class Player {

public:
	enum class LRDirection {
		kRight,
		kLeft,
	};

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	void Initialize(ViewProjection* viewprojection, const Vector3& position);

	void Update();

	void Draw();

	Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const WorldTransform& GetWorldTransform() const { return worldtransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void Move();

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	bool IsDead() const { return isDead_; }

private:
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	WorldTransform worldtransform_;
	ViewProjection* viewprojection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t texturehandle_ = 0u;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 0.1f;
	static inline const float kTimeTurn = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.01f;
	static inline const float kAttenuationWall = 0.2f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turntimer_ = 0.0f;

	bool onGround_ = true;

	bool landing = false;

	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kAttenuationLanding = 0.0f;
	static inline const float kGroundSearchHeight = 0.2f;

	MapChipField* mapChipField_ = nullptr;

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);

	void UpdateOnGround(const CollisionMapInfo& info);

	bool isDead_ = false;
};