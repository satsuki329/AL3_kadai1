#pragma once
#include "ViewProjection.h"
#include "Player.h"

class CameraController
{
public:

	void Initialize();

	void Update();

	void SetTarget(Player* target)
	{ 
		target_ = target;
	}

	void Reset();

private:
	ViewProjection viewprojection_;

	Player* target_ = nullptr;

	Vector3 targetOffset_ = {0, 0, -15.0f};

};