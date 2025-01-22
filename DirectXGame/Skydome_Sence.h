#pragma once
#include "KamataEngine.h"

class Skydome_Sence {

public:
	
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

private:
	
	KamataEngine::WorldTransform worldtransfrom_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

};
