#include "RailCamera.h"
#include <Windows.h>
#include <cmath>
#include <unordered_map>

void RailCamera::Initialize(const KamataEngine::Vector3& pos, const KamataEngine::Vector3& rad) {
	worldtransfrom_.translation_ = pos;
	worldtransfrom_.rotation_ = rad;

	worldtransfrom_.Initialize();
	camera_.Initialize();
}

void RailCamera::Update() {
	// ユーザー入力に基づいてカメラの位置と回転を更新
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	//if (GetAsyncKeyState('D') & 0x8000) {
	//	deltaX += 1.0f; // X軸方向の正の移動入力
	//}
	//if (GetAsyncKeyState('A') & 0x8000) {
	//	deltaX -= 1.0f; // X軸方向の負の移動入力
	//}
	//if (GetAsyncKeyState('W') & 0x8000) {
	//	deltaY += 1.0f; // Y軸方向の正の移動入力
	//}
	//if (GetAsyncKeyState('S') & 0x8000) {
	//	deltaY -= 1.0f; // Y軸方向の負の移動入力
	//}

	// 回転速度のスケーリングファクター
	const float rotationSpeed = 0.02f;

	// 球面座標系の更新
	float radius = 10.0f;                                               // カメラの半径
	float theta = worldtransfrom_.rotation_.y + deltaY * rotationSpeed; // 仰角
	float phi = worldtransfrom_.rotation_.x + deltaX * rotationSpeed;   // 方位角

	// 球面座標系からデカルト座標系への変換
	worldtransfrom_.translation_.x = radius * sinf(theta) * cosf(phi);
	worldtransfrom_.translation_.y = radius * cosf(theta);
	worldtransfrom_.translation_.z = radius * sinf(theta) * sinf(phi);

	// カメラの回転を更新
	worldtransfrom_.rotation_.x += deltaX * rotationSpeed;
	worldtransfrom_.rotation_.y += deltaY * rotationSpeed;

	// 行列の更新
	worldtransfrom_.UpdateMatarix();
	camera_.UpdateMatrix();
	camera_.matView = Inverse(worldtransfrom_.matWorld_);
	camera_.TransferMatrix();
}
