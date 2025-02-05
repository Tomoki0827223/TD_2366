#include "TitleSence.h"
#include <numbers>

TitleSence::~TitleSence() {
	delete titlemodel_;
	delete titlemodelFont_;
	delete TitleSkydome_;
}

void TitleSence::Initialize() {
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	textureHandle_ = KamataEngine::TextureManager::Load("Title/Title1.png");
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, {0, 0});
	textureHandle2_ = KamataEngine::TextureManager::Load("Title/Title2.png");
	sprite2_ = KamataEngine::Sprite::Create(textureHandle2_, {0, 0});
	textureHandle3_ = KamataEngine::TextureManager::Load("Title/Title3.png");
	sprite3_ = KamataEngine::Sprite::Create(textureHandle3_, {0, 0});
	textureHandle4_ = KamataEngine::TextureManager::Load("Title/Title4.png");
	sprite4_ = KamataEngine::Sprite::Create(textureHandle4_, {0, 0});

	titleskydome.Initialize();
	Camera_.Initialize();

	Timer_ = 0.0f;

	// 天球を生成
	skydome_ = new Skydome_Sence();
	modelSkydome_ = KamataEngine::Model::CreateFromOBJ("skydome", true);

	// 天球を初期化
	skydome_->Initialize(modelSkydome_, &Camera_);

	// タイトルを中央に寄せるために調整
	titleWorldTransform_.translation_ = {0.0f, 30.0f, -40.0f}; // x, y, zの値を調整

	titleWorldTransformFont_.translation_ = {-18.0f, -10.0f, 0.0f}; // x, y, zの値を調整

	titleskydome.translation_ = {0.0f, 0.0f, 0.0f};

	// スプライトの初期化
	InitializeSprites();
}

void TitleSence::InitializeSprites() {
	sprites.push_back(sprite_);
	sprites.push_back(sprite2_);
	sprites.push_back(sprite3_);
	sprites.push_back(sprite4_);
}

void TitleSence::Update() {
	
	// キー入力によるシーンの遷移
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 現在のスプライトが textureHandle2_ の場合のみシーン遷移
		if (sprites[currentSpriteIndex] == sprite2_) {
			isFinished_ = true;
		}
	}

	// キー入力によるシーンの遷移
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 現在のスプライトが textureHandle2_ の場合のみシーン遷移
		if (sprites[currentSpriteIndex] == sprite_) {
			isGameFinished_ = true;
		}
	}

	// 上下の矢印キー入力によるスプライトの切り替え
	if (input_->TriggerKey(DIK_UP)) {
		currentSpriteIndex = (currentSpriteIndex - 1 + sprites.size()) % sprites.size();
	} else if (input_->TriggerKey(DIK_DOWN)) {
		currentSpriteIndex = (currentSpriteIndex + 1) % sprites.size();
	}
}


void TitleSence::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// 現在のスプライトを描画
	sprites[currentSpriteIndex]->Draw();

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}
