#pragma once
#include <3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include "affine.h"
class PlayerBullet {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, BulletType bulletType);

	KamataEngine::Vector3 GetWorldPosition();

	BulletType GetBulletType() const { return bulletType_; }

	void Update();

	void OnCollision();

	void Draw(const KamataEngine::Camera& camera);

	~PlayerBullet();

	bool IsDead() const { return isDead_; }

private:
	KamataEngine::WorldTransform worldtransfrom_;

	KamataEngine::Model* model_ = nullptr;

	// uint32_t textureHandle_ = 0;

	KamataEngine::Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 30 * 1;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	BulletType bulletType_;
};
