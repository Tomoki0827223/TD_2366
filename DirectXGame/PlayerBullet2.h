#pragma once
#include <3d/Model.h>
#include <3d/Camera.h>
#include <3d/WorldTransform.h>

class PlayerBullet2 {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& direction);
	void Update();
	void Draw(const KamataEngine::Camera& camera);
	bool IsDead() const;

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Vector3 position_;
	KamataEngine::Vector3 direction_;
	bool isDead_ = false;
};
