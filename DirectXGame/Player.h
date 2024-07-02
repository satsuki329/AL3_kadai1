#pragma once

#include "Input.h"
#include "MathUtilityForText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {

public:
	enum class LRDirection {
		kRight,
		kLeft,
	};

	void Initialize(Model* model, ViewProjection* viewprojection, const Vector3& position);

	void Update();

	void Draw();

private:
	WorldTransform worldtransform_;
	ViewProjection* viewprojection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t texturehandle_ = 0;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 0.3f;
	static inline const float kTimeTurn = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turntimer_ = 0.0f;

	bool onGround_ = true;

	bool landing = false;

	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 0.5f;
};