#include "EnemyType1.h"
#include "Enemy.h"

void EnemyType1::Initialize(KamataEngine::Model* model, KamataEngine::Model* TamaModel, const KamataEngine::Vector3& pos) {
	enemy_ = new Enemy(); // ポインタに新しいインスタンスを割り当て
	enemy_->Initialize(model, TamaModel, pos);
}

void EnemyType1::Update() {
	if (enemy_) {
		enemy_->Update();
	}
}

void EnemyType1::Draw(const KamataEngine::Camera& camera) {
	if (enemy_) {
		enemy_->Draw(camera);
	}
}

void EnemyType1::Fire() {
	if (enemy_) {
		enemy_->Fire();
	}
}

void EnemyType1::OnCollision() {
	if (enemy_) {
		enemy_->OnCollision();
	}
}
