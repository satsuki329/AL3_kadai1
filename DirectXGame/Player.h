#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MathUtilityForText.h"

class Player
{

public:
	void Initialize(Model* model, ViewProjection* viewprojection, const Vector3& position);

	void Update();

	void Draw();

private:
	WorldTransform worldtransform_;
	ViewProjection* viewprojection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t texturehandle_ = 0;
};