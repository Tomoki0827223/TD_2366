#include "GameOver.h"

GameOver::~GameOver() 
{
	delete skydome_;
	delete modelSkydome_;
}

void GameOver::Initialize() 
{
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	camera_.Initialize();

	// 天球を生成
	skydome_ = new Skydome_Sence();
	modelSkydome_ = KamataEngine::Model::CreateFromOBJ("skydome", true);

	// 天球を初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	modelOver_ = KamataEngine::Model::CreateFromOBJ("GameOver");
	modelOverForm_.Initialize();

	modelPushFont_ = KamataEngine::Model::CreateFromOBJ("PushToSpace");
	PushFontForm_.Initialize();

	modelOverForm_.translation_ = {-38.0f, 0.0f, 0.0f};  // x, y, zの値を調整
	PushFontForm_.translation_ = {-15.0f, -10.0f, 0.0f}; // x, y, zの値を調整
}

void GameOver::Update() 
{
	skydome_->Update();
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		// if (soundSEData_1 == 0 || !audio_->IsPlaying(soundSEData_1)) {
		//	soundSEData_1 = audio_->PlayWave(soundSE_1, false, 0.5f);
		// }

		finished_ = true;
	}

	//// ImGuiでフォントの位置を調整
	// ImGui::Begin("Title Font Position");
	//// テキストボックスを使って手入力できるようにする
	// ImGui::InputFloat("Position X", &modelOverForm_.translation_.x);
	// ImGui::InputFloat("Position Y", &modelOverForm_.translation_.y);
	// ImGui::InputFloat("Position Z", &modelOverForm_.translation_.z);

	// ImGui::InputFloat("titleSky X", &PushFontForm_.translation_.x);
	// ImGui::InputFloat("titleSky Y", &PushFontForm_.translation_.y);
	// ImGui::InputFloat("titleSky Z", &PushFontForm_.translation_.z);
	// ImGui::End();

	modelOverForm_.UpdateMatarix();
	PushFontForm_.UpdateMatarix();
}

void GameOver::Draw() 
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydome_->Draw();

	modelOver_->Draw(modelOverForm_, camera_);
	modelPushFont_->Draw(PushFontForm_, camera_);

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}