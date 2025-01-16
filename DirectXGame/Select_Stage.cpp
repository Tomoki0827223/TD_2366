#include "Select_Stage.h"
#include <iostream>

Select_Stage::~Select_Stage() 
{
	delete skydome_;
	delete modelSkydome_;
	delete sprite_;
}

void Select_Stage::Initialize() 
{
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	
	// ビュープロジェクションの初期化
	camera_.Initialize();

	// 天球を生成
	skydome_ = new Skydome();
	modelSkydome_ = KamataEngine::Model::CreateFromOBJ("skydome", true);
	// 天球を初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	textureHandle_ = KamataEngine::TextureManager::Load("alStart.png");
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, {50, 25});
}

void Select_Stage::Update() 
{
	if (input_->TriggerKey(DIK_1)) {
		finished_ = true;
		StageNumber_ = 0;
		skydome_->Update();
		// std::cout << "ステージ0が選択されました。" << std::endl;
	} else if (input_->TriggerKey(DIK_2)) {
		finished_ = true;
		StageNumber_ = 1;
		skydome_->Update();
		// std::cout << "ステージ1が選択されました。" << std::endl;
	} else if (input_->TriggerKey(DIK_3)) {
		finished_ = true;
		StageNumber_ = 2;
		skydome_->Update();
		// std::cout << "ステージ1が選択されました。" << std::endl;
	}
}

void Select_Stage::Draw() 
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

	skydome_->Draw(camera_);

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}
