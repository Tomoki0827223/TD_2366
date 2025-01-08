#pragma once
#include "Enemy.h"
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
using namespace KamataEngine;

class GameScene {
public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	void CheckAllCollisions();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Vector3 playerPos = {};
	Vector3 enemyPos = {0, 3, 100};

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;

	// std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	Camera camera_;

	bool isDebugCameraActive_ = false;
	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	// 敵が消える時間を管理する変数
	int enemyDisappearTimer_ = 0;
	const int kEnemyDisappearTime = 180; // 敵が消える時間（フレーム数）
};