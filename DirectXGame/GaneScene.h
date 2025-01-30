#pragma once
#include "AABB.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include "Skydome2.h"
#include "GroundModel.h"
#include "Wall.h"
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

	void DrawText();

	// 弾を追加
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	void EnemySpawn(const Vector3& position);

	int32_t timer = 0;
	bool timerflag = true;

	
    bool IsFinished() const { return isFinished_; }
	bool IsGameOver() const { return isGameOver_; } // ゲームオーバーのフラグをチェック

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	// Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	Skydome2* skydome2_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	GroundModel* groundModel_ = nullptr;
	Wall* wall_ = nullptr;

	Vector3 playerPos = {0, 0, 25};
	Vector3 RailCamerPos = {0, 0, 0};
	Vector3 RailCamerRad = {0, 0, 0};

	Vector3 WallPos = {0, 0, 25};

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	
	Model* modelSkydome_ = nullptr;
	Model* modelSkydome2_ = nullptr;

	Model* modelEnemyBullet_ = nullptr;
	Model* modelPlayerbullet_ = nullptr;
	Model* modelPlayerbullet2_ = nullptr;
	Model* modelPlayerbullet3_ = nullptr;

	Model* modelWall_ = nullptr;
	Model* modelGround_ = nullptr;

	Sprite* healthBar_ = nullptr;
	TextureManager* healthText_ = nullptr;

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

	bool isFinished_ = false;
	float elapsedTime_ = 120.0f; // 経過時間

	bool isOverFinished_ = false;
	bool isGameOver_ = false;  // ゲームオーバーのフラグ

	float maxHp = 100000.0f;
	float nowHp = maxHp;
	float width = 640.0f;

	float maxHertHP = 1000.0f;
	float nowHertHP = maxHertHP;
	float hertWidth = 256.0f;

	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	Sprite* sprite_ = nullptr;
	Sprite* sprite2_ = nullptr;

	Sprite* hertSprite_ = nullptr;
	uint32_t hertTextureHandle_ = 0;
	Sprite* hertSprite2_ = nullptr;
	uint32_t hertTextureHandle2_ = 0;

	//// 追加　
	//bool isInvisible_ = false;
	//float invisibleTimmer = 10 * 5;

	uint32_t fontTextureHandle_;                    // フォントのテクスチャハンドル
	Sprite* charSprites_[10] = {0};              // 制限時間（秒）
};