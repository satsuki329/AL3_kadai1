#pragma once
#include "WorldTransform.h"
#include "Model.h"

class skydome
{
public:

	void Initialize(Model* model, ViewProjection* viewprojection);

	void Update();

	void Draw();


private:

	WorldTransform worldTransform_;
	ViewProjection* viewprojection_ = nullptr;

	Model* model_ = nullptr;
};