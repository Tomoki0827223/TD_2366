#include "GroundModel.h"

void GroundModel::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	modelGround_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatarix();
}

void GroundModel::Update() { worldTransform_.UpdateMatarix(); }

void GroundModel::Draw() 
{ modelGround_->Draw(worldTransform_, *camera_); }
