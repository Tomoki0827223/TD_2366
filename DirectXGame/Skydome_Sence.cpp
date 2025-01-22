#include "Skydome_Sence.h"

void Skydome_Sence::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	
	worldtransfrom_.Initialize();
	model_ = model;
	camera_ = camera;

}

void Skydome_Sence::Update() {}

void Skydome_Sence::Draw() { model_->Draw(worldtransfrom_, *camera_); }