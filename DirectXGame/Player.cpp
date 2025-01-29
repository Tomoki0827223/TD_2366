#include "Player.h"


Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	for (PlayerBullet2* bullet : bullets2_) {
		delete bullet;
	}
}

void Player::Initialize(
    KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Model* normalBulletModel, KamataEngine::Model* specialBulletModel, KamataEngine::Model* type2BulletModel,
    const KamataEngine::Vector3& position) {
	assert(model);
	assert(camera);
	assert(normalBulletModel);
	assert(specialBulletModel);
	assert(type2BulletModel);

	model_ = model;
	camera_ = camera;
	normalBulletModel_ = normalBulletModel;
	specialBulletModel_ = specialBulletModel;
	type2BulletModel_ = type2BulletModel;

	worldtransform_.Initialize();

	worldtransform_.translation_ = position;

	input_ = KamataEngine::Input::GetInstance();
}

void Player::OnCollision(const EnemyBullet* enemyBullet) {
	(void)enemyBullet;

	isDead_ = true;
}

void Player::Attack() {
	KamataEngine::Model* bulletModel = nullptr;

	if (input_->TriggerKey(DIK_SPACE)) {

		switch (currentBulletType_) {
		case BulletType::Normal:
			bulletModel = normalBulletModel_;
			break;
		case BulletType::Special:
			bulletModel = specialBulletModel_;
			break;
		case BulletType::Type2:
			bulletModel = type2BulletModel_;
			break;
		}

		if (currentBulletType_ == BulletType::Type2) {
			PlayerBullet2* newBullet = new PlayerBullet2();
			newBullet->Initialize(bulletModel, GetWorldPosition(), KamataEngine::Vector3(0, 0, 1));
			bullets2_.push_back(newBullet);
		} else {
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel, GetWorldPosition(), KamataEngine::Vector3(0, 0, 1));
			bullets_.push_back(newBullet);
		}
	}
}

void Player::SwitchBulletType(BulletType type) { currentBulletType_ = type; }

KamataEngine::Vector3 Player::GetWorldPosition() {
	KamataEngine::Vector3 worldPos;
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::SetParent(const KamataEngine::WorldTransform* parent) { worldtransform_.parent_ = parent; }

void Player::Update() {

	if (input_->TriggerKey(DIK_1)) {
		SwitchBulletType(BulletType::Normal);

	} else if (input_->TriggerKey(DIK_2)) {
	
		SwitchBulletType(BulletType::Special);
	}

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	for (PlayerBullet2* bullet : bullets2_) {
		bullet->Update();
	}

	bullets_.erase(
	    std::remove_if(
	        bullets_.begin(), bullets_.end(),
	        [](PlayerBullet* bullet) {
		        if (bullet->IsDead()) {
			        delete bullet;
			        return true;
		        }
		        return false;
	        }),
	    bullets_.end());

	bullets2_.erase(
	    std::remove_if(
	        bullets2_.begin(), bullets2_.end(),
	        [](PlayerBullet2* bullet) {
		        if (bullet->IsDead()) {
			        delete bullet;
			        return true;
		        }
		        return false;
	        }),
	    bullets2_.end());

	KamataEngine::Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.3f;
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_A)) {
		worldtransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldtransform_.rotation_.y -= kRotSpeed;
	}

	worldtransform_.translation_.x += move.x;
	worldtransform_.translation_.y += move.y;

	worldtransform_.UpdateMatarix();

	Attack();
}

void Player::Draw(const KamataEngine::Camera& camera) {
	model_->Draw(worldtransform_, camera);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
	for (PlayerBullet2* bullet : bullets2_) {
		bullet->Draw(camera);
	}
}
