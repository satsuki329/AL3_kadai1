#include "skydome.h"
#include <cassert>

void skydome::Initialize(Model* model, ViewProjection* viewprojection) {
	assert(model);
	model_ = model;
	viewprojection_ = viewprojection;
	worldTransform_.Initialize();
}

void skydome::Update()
{

}

void skydome::Draw() { 
	model_->Draw(worldTransform_, *viewprojection_); 
}