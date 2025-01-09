#include "PlayerBullet2.h"

void PlayerBullet2::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& direction) {
	model_ = model;
	position_ = position;
	direction_ = direction;
}

void PlayerBullet2::Update() {
	// 弾の移動処理
	position_ += direction_;
	// 例: 一定距離を超えたら弾を消す
	if (position_.z > 100.0f) {
		isDead_ = true;
	}
}

void PlayerBullet2::Draw(const KamataEngine::Camera& camera) {
	if (model_) {
		// ワールドトランスフォームを設定して描画
		KamataEngine::WorldTransform worldTransform;
		worldTransform.translation_ = position_;
		model_->Draw(worldTransform, camera);
	}
}

bool PlayerBullet2::IsDead() const { return isDead_; }
