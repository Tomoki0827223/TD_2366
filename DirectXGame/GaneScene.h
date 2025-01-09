#pragma once
#include "AABB.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include <sstream>
using namespace KamataEngine;

class GameScene {
public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
	// 衝突判定と応答
	void CheckAllCollisions();

	// 弾を追加
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	void EnemySpawn(const Vector3& position);

	int32_t timer = 0;
	bool timerflag = true;

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	// Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	Vector3 playerPos = {0, 0, 25};
	Vector3 RailCamerPos = {0, 0, 0};
	Vector3 RailCamerRad = {0, 0, 0};

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelEnemyBullet_ = nullptr;
	Model* modelPlayerbullet_ = nullptr;
	Model* modelPlayerbullet2_ = nullptr;

	// カメラ
	WorldTransform worldTransform_;
	Camera camera_;

	Vector3 railcameraPos = {0, 0, 0};
	Vector3 railcameraRad = {0, 0, 0};

	// 敵弾リストを追加
	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	std::list<Enemy*> enemies_;
};