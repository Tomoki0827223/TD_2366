#include "Wall.h"

void Wall::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

	worldtransfrom_.Initialize();
	model_ = model;
	camera_ = camera;
	worldtransfrom_.translation_ = position;
}

void Wall::Update() {}

void Wall::Draw() 
{ model_->Draw(worldtransfrom_, *camera_); }
