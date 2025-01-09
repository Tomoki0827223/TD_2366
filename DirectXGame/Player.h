#pragma once
#include <3d/Camera.h>
#include <input/Input.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include "PlayerBullet.h"
#include "PlayerBullet2.h"
#include "AABB.h"
#include <algorithm>
#include <cassert>

class Enemy;


class Player {
public:
	enum class BulletType { Normal, Special, Type2 };

	~Player();
	void Initialize(
	    KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Model* normalBulletModel, KamataEngine::Model* specialBulletModel, KamataEngine::Model* type2BulletModel,
	    const KamataEngine::Vector3& position);
	void OnCollision();
	void Update();
	void Draw(const KamataEngine::Camera& camera);
	void SwitchBulletType(BulletType type);
	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();
	void SetParent(const KamataEngine::WorldTransform* parent);

    // 新しいメソッド
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	const std::list<PlayerBullet2*>& GetBullets2() const { return bullets2_; }

private:
	void Attack();

	Enemy* enemy_ = nullptr;

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Model* normalBulletModel_ = nullptr;
	KamataEngine::Model* specialBulletModel_ = nullptr;
	KamataEngine::Model* type2BulletModel_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::WorldTransform worldtransform_;
	std::list<PlayerBullet*> bullets_;
	std::list<PlayerBullet2*> bullets2_;
	BulletType currentBulletType_ = BulletType::Normal;
	bool isDead_ = false;

	// AABBの幅と高さを定義
	static constexpr float kWidth = 1.0f;
	static constexpr float kHeight = 1.0f;
};