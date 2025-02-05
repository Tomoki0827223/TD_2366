#include "GaneScene.h"
#include <KamataEngine.h>
#include "GameOver.h"
#include "GameClear.h"
#include "TitleSence.h"
#include "Select_Stage.h"

using namespace KamataEngine;

// シーン（型）
enum class Scene {

	kUnknown = 0,

	kTitleScene,
	kSelectStage,
	kGameScene,
	kGameOver,
	kGameClear,

};

int mapNumber = 0;

// タイトルシーン
TitleSence* titleScene = nullptr;

//セレクトステージ
Select_Stage* selectStage = nullptr;

// ゲームシーン
GameScene* gameScene = nullptr;

// ゲームオーバーシーン
GameOver* gameOver = nullptr;

// ゲームクリアシーン
GameClear* gameClear = nullptr;

// 現在シーン（型）
Scene scene = Scene::kUnknown;

void ChangeScene();
void UpdateScene();
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"2366_スキキライ3D");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// タイトルシーンの初期化
	titleScene = new TitleSence();
	titleScene->Initialize();

	// シーンをタイトルシーンに設定
	scene = Scene::kTitleScene;

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();

		// シーンの更新と切り替え
		UpdateScene();
		ChangeScene();

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	delete gameScene;
	delete gameOver;
	delete gameClear;
	delete titleScene;
	delete selectStage;

	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {
	switch (scene) {
	case Scene::kTitleScene:
		if (titleScene && titleScene->IsSelectFinished()) {
			delete titleScene;
			titleScene = nullptr;
			scene = Scene::kSelectStage;
			selectStage = new Select_Stage();
			selectStage->Initialize();
		}

		if (titleScene && titleScene->IsGameFinished()) {
			delete titleScene;
			titleScene = nullptr;
			scene = Scene::kGameScene;
			gameScene = new GameScene();
			gameScene->Initialize();
		}

		break;
	case Scene::kSelectStage:
		if (selectStage->IsFinished()) {
			mapNumber = selectStage->GetStageNumber();
			delete selectStage;
			selectStage = nullptr;
			scene = Scene::kTitleScene;
			titleScene = new TitleSence();
			titleScene->Initialize();
		}
		break;
	case Scene::kGameScene:
		if (gameScene->IsFinished()) {
			delete gameScene;
			gameScene = nullptr;
			scene = Scene::kGameClear;
			gameClear = new GameClear();
			gameClear->Initialize();
		} else if (gameScene->IsGameOver()) {
			delete gameScene;
			gameScene = nullptr;
			scene = Scene::kGameOver;
			gameOver = new GameOver();
			gameOver->Initialize();
		}
		break;
	case Scene::kGameOver:
		if (gameOver->IsFinished()) {
			delete gameOver;
			gameOver = nullptr;
			scene = Scene::kTitleScene;
			titleScene = new TitleSence();
			titleScene->Initialize();
		}
		break;
	case Scene::kGameClear:
		if (gameClear->IsFinished()) {
			delete gameClear;
			gameClear = nullptr;
			scene = Scene::kTitleScene;
			titleScene = new TitleSence();
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() 
{
	switch (scene) {
	case Scene::kTitleScene:
		titleScene->Update();
		break;
	case Scene::kSelectStage:
		selectStage->Update();
		break;
	case Scene::kGameScene:
		gameScene->Update();
		break;
	case Scene::kGameOver:
		gameOver->Update();
		break;
	case Scene::kGameClear:
		gameClear->Update();
		break;
	}
}

void DrawScene() 
{
	switch (scene) {
	case Scene::kTitleScene:
		titleScene->Draw();
		break;
	case Scene::kSelectStage:
		selectStage->Draw();
		break;
	case Scene::kGameScene:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOver->Draw();
		break;
	case Scene::kGameClear:
		gameClear->Draw();
		break;
	}
}
