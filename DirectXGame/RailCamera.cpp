#include "RailCamera.h"
#include <cmath>

void RailCamera::Initialize(const KamataEngine::Vector3& pos, const KamataEngine::Vector3& rad) {
	worldtransfrom_.translation_ = pos;
	worldtransfrom_.rotation_ = rad;

	worldtransfrom_.Initialize();
	camera_.Initialize();
}

void RailCamera::Update() {
	// ユーザー入力に基づいてカメラの位置と回転を更新
	float deltaX = GetInput("MoveX+"); // X軸方向の正の移動入力
	deltaX -= GetInput("MoveX-");      // X軸方向の負の移動入力
	float deltaY = GetInput("MoveY+"); // Y軸方向の正の移動入力
	deltaY -= GetInput("MoveY-");      // Y軸方向の負の移動入力

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

float RailCamera::GetInput(const std::string& inputName) {
	static std::unordered_map<std::string, int> keyMap = {
	    {"MoveX+", 'S'}, // 'D'キーで右移動
	    {"MoveX-", 'W'}, // 'A'キーで左移動
	    {"MoveY+", 'D'}, // 'W'キーで上移動
	    {"MoveY-", 'A'}  // 'S'キーで下移動
	};

	if (keyMap.find(inputName) != keyMap.end()) {
		if (GetAsyncKeyState(keyMap[inputName]) & 0x8000) {
			return 1.0f; // キーが押されている場合
		}
	}

	return 0.0f; // キーが押されていない場合
}
