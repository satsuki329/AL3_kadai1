#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Player
{

public:
	void Initialize(Model* model, uint32_t texturehandle, ViewProjection* viewprojection);

	void Update();

	void Draw();

private:
	WorldTransform worldtransform_;
	ViewProjection* viewprojection_;

	Model* model_ = nullptr;

	uint32_t texturehandle_ = 0;
};