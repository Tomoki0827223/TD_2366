#include "EnemyType1.h"
#include "Player.h"

void EnemyType1::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& pos) 
{
	assert(model);
	model_ = model;
	worldtransfrom_.translation_ = pos;
	worldtransfrom_.Initialize();
}

void EnemyType1::Update() {
	// キャラクターの移動ベクトル
	KamataEngine::Vector3 move = {0, 0, 0};
	// 接近
	KamataEngine::Vector3 accessSpeed = {-0.1f, -0.1f, -0.1f};
	// 離脱
	KamataEngine::Vector3 eliminationSpeed = {-0.3f, -0.3f, -0.3f};

	worldtransfrom_.UpdateMatarix();
}

void EnemyType1::Draw(const KamataEngine::Camera& camera) 
{
	// モデルの描画
	model_->Draw(worldtransfrom_, camera);
}

EnemyType1::~EnemyType1() 
{

}

void EnemyType1::OnCollision() 
{ isDead_ = true; }

KamataEngine::Vector3 EnemyType1::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}
