#include "Player.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
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

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	isDead_ = true;
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		KamataEngine::Model* bulletModel = nullptr;

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
		default:
			return;
		}

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel, GetWorldPosition(), KamataEngine::Vector3(0, 0, 1), currentBulletType_);
		bullets_.push_back(newBullet);
	}
}

void Player::SwitchBulletType(BulletType type) { currentBulletType_ = type; }

KamataEngine::Vector3 Player::GetWorldPosition() { return {worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1], worldtransform_.matWorld_.m[3][2]}; }

AABB Player::GetAABB() {
	KamataEngine::Vector3 worldPos = GetWorldPosition();
	return {
	    {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f},
        {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f}
    };
}

void Player::SetParent(const KamataEngine::WorldTransform* parent) { worldtransform_.parent_ = parent; }

void Player::Update() {
	// 弾種変更
	if (input_->TriggerKey(DIK_1)) {
		SwitchBulletType(BulletType::Normal);
	} else if (input_->TriggerKey(DIK_2)) {
		SwitchBulletType(BulletType::Special);
	}

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
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

	// 移動処理
	KamataEngine::Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.3f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 位置を更新
	worldtransform_.translation_.x += move.x;
	worldtransform_.translation_.y += move.y;

	// 画面の境界を超えないように制限
	worldtransform_.translation_.x = std::clamp(worldtransform_.translation_.x, kScreenLeft, kScreenRight);
	worldtransform_.translation_.y = std::clamp(worldtransform_.translation_.y, kScreenBottom, kScreenTop);

	worldtransform_.UpdateMatarix();

	Attack();
}

void Player::Draw(const KamataEngine::Camera& camera) {
	model_->Draw(worldtransform_, camera);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}
