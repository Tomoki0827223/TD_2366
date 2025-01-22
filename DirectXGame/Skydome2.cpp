#include "Skydome2.h"

void Skydome2::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	worldtransfrom_.Initialize();
	model_ = model;
	camera_ = camera;
}

void Skydome2::Update() {}

void Skydome2::Draw() { model_->Draw(worldtransfrom_, *camera_); }