#pragma once
#include <KamataEngine.h>
#include <3d/Model.h>

class GroundModel 
{
public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

private:
	KamataEngine::Model* modelGround_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_;
};
