#include "GroundModel.h"

void GroundModel::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) 
{
	worldTransform_.Initialize();
	modelGround_ = model;
	camera_ = camera;
}

void GroundModel::Update() 
{

}

void GroundModel::Draw() 
{ modelGround_->Draw(worldTransform_, *camera_); }
