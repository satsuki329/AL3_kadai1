#pragma once
#include "ViewProjection.h"
#include "Player.h"
#include "MathUtilityForText.h"

class CameraController
{
public:

	struct Rect {
		float left = 0.0f;  
		float right = 1.0f;  
		float bottom = 0.0f;
		float top = 1.0f;  
	};

	void Initialize();

	void Update();

	void SetTarget(Player* target)
	{ 
		target_ = target;
	}

	ViewProjection& GetViewProjection() { return viewprojection_; }

	void Reset();

	void SetMovableArea(Rect area) { movableArea_ = area; }

	float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }


private:
	ViewProjection viewprojection_;

	Player* target_ = nullptr;

	Vector3 targetOffset_ = {0, 0, -15.0f};

	Rect movableArea_ = {0, 100, 0, 100};

	Vector3 destination_;

	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};

	static inline const float kInterpolationRate_ = 0.1f;
	static inline const float kVelocityBias_ = 25.0f;

};