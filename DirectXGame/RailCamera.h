#pragma once
#include <3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <3d/DebugCamera.h>

class Player;

class RailCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	
	KamataEngine::WorldTransform worldtransfrom_;
	KamataEngine::Camera* camera_ = nullptr;


};
