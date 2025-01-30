#pragma once
#include <KamataEngine.h>
#include <3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>


class Wall 
{
public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

private:

	KamataEngine::WorldTransform worldtransfrom_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
};
