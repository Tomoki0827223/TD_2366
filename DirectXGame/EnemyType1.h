#pragma once

class Enemy;

class EnemyType1 {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Model* TamaModel, const KamataEngine::Vector3& pos);
	void Update();
	void Draw(const KamataEngine::Camera& camera);
	void Fire();
	void OnCollision();

private:
	Enemy* enemy_ = nullptr; // ポインタとして宣言
};
