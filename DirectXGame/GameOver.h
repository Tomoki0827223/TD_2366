#pragma once
#include <audio/Audio.h>
#include <3d/Model.h>
#include <2d/Sprite.h>
#include <input/Input.h>
#include <base/TextureManager.h>
#include "Skydome_Sence.h"
#include <KamataEngine.h>

class GameOver {

public:

	/// <summary>
	/// デストクラタ
	/// </summary>
	~GameOver();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// デスフラグの getter
	bool IsFinished() const { return finished_; }

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// ビュープロジェクション
	KamataEngine::Camera camera_;

	// 終了フラグ
	bool finished_ = false;

	// 天球
	Skydome_Sence* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;

	KamataEngine::Model* modelOver_ = nullptr;
	KamataEngine::WorldTransform modelOverForm_;

	KamataEngine::Model* modelPushFont_ = nullptr;
	KamataEngine::WorldTransform PushFontForm_;
};