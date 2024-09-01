#define NOMINMAX

#include "Death.h"
#include "MathUtilityForText.h"
#include <cassert>

void Death::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);

	model_ = model;

	for (auto& worldTransform : worldtransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	viewProjection_ = viewProjection;
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void Death::Update() {
	if (isFinished_) {
		return;
	}
	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}
	
	for (uint32_t i = 0; i < worldtransforms_.size(); ++i) {
		Vector3 velocity = {kSpeed, 0, 0};

		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotation);

		worldtransforms_[i].translation_ += velocity;
	}

	for (auto& worldTransform : worldtransforms_) {
		worldTransform.UpdateMatrix();
	}

	color_.w = std::max(0.0f, 1.0f - counter_ / kDuration);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();
}

void Death::Draw() {
	if (isFinished_) {
		return;
	}

	for (auto& worldTransform : worldtransforms_) {
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}