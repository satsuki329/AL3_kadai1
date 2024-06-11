#include "skydome.h"

void skydome::Initialize(Model* model, ViewProjection* viewprojection) {
	
	model_ = model;
	viewprojection_ = viewprojection;
	worldTransformSkydome_.Initialize();
}

void skydome::Update()
{  
}

void skydome::Draw() { 
	model_->Draw(worldTransformSkydome_, *viewprojection_); 
}