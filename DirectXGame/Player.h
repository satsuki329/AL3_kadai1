#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MathUtilityForText.h"
#include "Input.h"
#include <algorithm>

class Player
{

public:
	enum class LRDirection
	{
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

	static inline const float kAcceleration = 1;
	static inline const float kAttenuation = 1;
	static inline const float kLimitRunSpeed = 5;
	static inline const float kTimeTurn = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turntimer_ = 0.0f;
};