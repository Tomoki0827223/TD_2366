#pragma once
#include "AABB.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "KamataEngine.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include "Skydome2.h"
#include "GroundModel.h"
#include "Wall.h"
#include <sstream>
#include "TitleSence.h"
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
	void DrawHertText();

	// 弾を追加
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	void EnemySpawn(const Vector3& position);

	void LoadEnemy2PopData();
	void UpdateEnemy2PopCommands();
	void Enemy2Spawn(const Vector3& position);

	void LoadEnemy3PopData();
	void UpdateEnemy3PopCommands();
	void Enemy3Spawn(const Vector3& position);

	int32_t timer = 0;
	int32_t timer1 = 0;
	int32_t timer2 = 0;
	bool timerflag = true;
	bool timerflag1 = true;
	bool timerflag2 = true;

	
    bool IsFinished() const { return isFinished_; }
	bool IsGameOver() const { return isGameOver_; } // ゲームオーバーのフラグをチェック
	bool IsBGMplaying() const { return isBGMPlaying_; }

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	// Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	Skydome2* skydome2_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	GroundModel* ground_ = nullptr;
	Wall* wall_ = nullptr;
	Wall* wall2_ = nullptr;

	Vector3 playerPos = {0, -4, 15};
	Vector3 RailCamerPos = {0, 0, 0};
	Vector3 RailCamerRad = {0, 0, 0};

	Vector3 WallPos = {20, 0, 0};
	Vector3 WallPos2 = {-20, 0, 0};
	Vector3 groundPos = {0, 5, 0};

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelEnemy2_ = nullptr;
	Model* modelEnemy3_ = nullptr;
	
	Model* modelSkydome_ = nullptr;
	Model* modelSkydome2_ = nullptr;

	Model* modelEnemyBullet_ = nullptr;
	Model* modelPlayerbullet_ = nullptr;
	Model* modelPlayerbullet2_ = nullptr;
	Model* modelPlayerbullet3_ = nullptr;

	Model* modelWall_ = nullptr;
	Model* modelWall2_ = nullptr;
	Model* modelGround_ = nullptr;

	Sprite* healthBar_ = nullptr;
	TextureManager* healthText_ = nullptr;
	TitleSence* titleSence_ = nullptr;

	// カメラ
	WorldTransform worldTransform_;
	Camera camera_;

	Vector3 railcameraPos = {0, 0, 0};
	Vector3 railcameraRad = {0, 0, 0};

	// 敵弾リストを追加
	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	std::stringstream enemy2PopCommands;

	std::list<Enemy*> enemies_;
	std::list<Enemy2*> enemies2_;
	std::list<Enemy3*> enemies3_;

	bool isFinished_ = false;
	float elapsedTime_ = 90.0f; // 経過時間

	bool isOverFinished_ = false;
	bool isGameOver_ = false;  // ゲームオーバーのフラグ

	float maxHp = 100.0f;
	float nowHp = maxHp;
	float width = 640.0f;

	float maxHertHP = 50.0f;
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
	Sprite* hertSprite3_ = nullptr;
	uint32_t hertTextureHandle3_ = 0;
	Sprite* hertSprite4_ = nullptr;
	uint32_t hertTextureHandle4_ = 0;
	Sprite* hertSprite5_ = nullptr;
	uint32_t hertTextureHandle5_ = 0;

	//// 追加　
	//bool isInvisible_ = false;
	//float invisibleTimmer = 10 * 5;

	uint32_t fontTextureHandle_;                    // フォントのテクスチャハンドル
	Sprite* charSprites_[10] = {0};              // 制限時間（秒）

	uint32_t fontTextureHandle2_; // フォントのテクスチャハンドル
	Sprite* charSprites2_[10] = {0}; // 制限時間（秒）

	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	uint32_t soundBGM1_ = 0;
	uint32_t soundBGMHanlde_1 = 0;
	bool isBGMPlaying_ = false;

	uint32_t soundSE_ = 0;
	uint32_t soundSEHanlde_ = 0;

	uint32_t soundSE1_ = 0;
	uint32_t soundSE2_ = 0;
	uint32_t soundSE3_ = 0;
};