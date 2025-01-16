#pragma once
#include <audio/Audio.h>
#include <3d/Model.h>
#include <2d/Sprite.h>
#include <3d/WorldTransform.h>
#include <3d/Camera.h>
#include "Skydome.h"
#include <input/Input.h>
#include <base/TextureManager.h>
#include <KamataEngine.h>

class Select_Stage {
public:

	~Select_Stage();

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

	uint32_t GetStageNumber() const { return StageNumber_; }

	uint32_t StageNumber_ = 0;

private:

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// ビュープロジェクション
	KamataEngine::Camera camera_;

	bool finished_ = false;

	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;

};