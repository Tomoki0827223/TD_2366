#pragma once
#include <audio/Audio.h>
#include <3d/Model.h>
#include <2d/Sprite.h>
#include <3d/WorldTransform.h>
#include <3d/Camera.h>
#include "Skydome.h"
#include "input/Input.h"
#include <KamataEngine.h>


/// <summary>
/// タイトルシーン
/// </summary>
class TitleSence {

public:
	~TitleSence();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 5.0f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = -5.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalklMotionTime = 1.0f;
	// タイマー
	float Timer_ = 0.0f;

	bool finished_ = false;
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::WorldTransform titleWorldTransform_;
	KamataEngine::WorldTransform titleWorldTransformFont_;
	KamataEngine::WorldTransform titleskydome;
	KamataEngine::Camera Camera_;
	KamataEngine::Model* titlemodel_ = nullptr;
	KamataEngine::Model* titlemodelFont_ = nullptr;
	KamataEngine::Model* TitleSkydome_ = nullptr;
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	// 天球
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;
};
