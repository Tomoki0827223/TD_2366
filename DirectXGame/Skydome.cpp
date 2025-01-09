#include "Skydome.h"
#include "affine.h"

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	worldtransfrom_.Initialize();
	model_ = model;
	camera_ = camera;
}

void Skydome::Update() {
	// 回転角度（ラジアン）
	float rotationAngle = 0.01f; // 例: 毎フレーム0.01ラジアン回転

	// Y軸周りの回転行列を作成
	KamataEngine::Matrix4x4 rotationMatrix = CreateRotationY(rotationAngle);

	// ワールドトランスフォーム行列に回転行列を適用
	worldtransfrom_.matWorld_ = rotationMatrix * worldtransfrom_.matWorld_;

	// 行列を更新
	worldtransfrom_.UpdateMatarix();
}

void Skydome::Draw(const KamataEngine::Camera& camera) {
	// 天球の描画
	model_->Draw(worldtransfrom_, camera);
}
