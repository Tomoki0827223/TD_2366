#include "GaneScene.h"
#include "3d/AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelSkydome_;
	delete player_;
	delete skydome_;
	delete railCamera_;
	delete modelEnemyBullet_;
	delete modelPlayerbullet_;
	delete modelPlayerbullet2_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	skydome_ = new Skydome();
	// 3Dモデルの生成
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	modelEnemyBullet_ = KamataEngine::Model::CreateFromOBJ("Tama", true);
	modelPlayerbullet_ = KamataEngine::Model::CreateFromOBJ("TamaPlayer", true);
	modelPlayerbullet2_ = KamataEngine::Model::CreateFromOBJ("TamaPlayer", true);

	camera_.Initialize();
	player_->Initialize(modelPlayer_, &camera_, modelPlayerbullet_, modelPlayerbullet2_, playerPos);
	skydome_->Initialize(modelSkydome_, &camera_);

	// 軸方向表示の表示を有効にする
	KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	// Camera
	railCamera_ = new RailCamera();
	railCamera_->Initialize(railcameraPos, railcameraRad);
	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update() {

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}


	UpdateEnemyPopCommands();

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 敵の削除処理を追加
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	CheckAllCollisions();

	railCamera_->Update();
	player_->Update();
	camera_.matView = railCamera_->GetViewProjection().matView;
	camera_.matProjection = railCamera_->GetViewProjection().matProjection;
	camera_.TransferMatrix();
}


void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
	KamataEngine::Model::PreDraw(commandList);



	player_->Draw(camera_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}

	skydome_->Draw(camera_);
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(camera_);
	}


	KamataEngine::Model::PostDraw();
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}

void GameScene::AddEnemyBullet(EnemyBullet* bullet) 
{ 
	enemyBullets_.push_back(bullet);
}

void GameScene::EnemySpawn(const Vector3& position) {

	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(modelEnemy_, modelEnemyBullet_, position);

	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);
	player_->SetEnemy(newEnemy);
	newEnemy->SetGameScene(this);

	enemies_.push_back(newEnemy);
}

void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (timerflag) {
		timer--;
		if (timer <= 0) {
			// 待機完了
			timerflag = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ　
	while (getline(enemyPopCommands, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		if (word.find("POP") == 0) {

			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemySpawn(Vector3(x, y, z));

			// WAITコマンド
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始

			timerflag = true;
			timer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::CheckAllCollisions() {
	KamataEngine::Vector3 posA[3]{}, posB[3]{};
	float radiusA[3] = {0.8f, 2.0f, 0.8f}; // プレイヤーの半径（固定値）
	float radiusB[3] = {0.8f, 2.0f, 0.8f}; // 敵弾の半径（固定値）

	// 自弾
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA[0] = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		// 敵弾の座標
		posB[0] = bullet->GetWorldPosition();

		// 2つの球の中心間の距離の二乗を計算
		float distanceSquared = (posA[0].x - posB[0].x) * (posA[0].x - posB[0].x) + (posA[0].y - posB[0].y) * (posA[0].y - posB[0].y) + (posA[0].z - posB[0].z) * (posA[0].z - posB[0].z);

		// 半径の合計の二乗
		float combinedRadiusSquared = (radiusA[0] + radiusB[0]) * (radiusA[0] + radiusB[0]);

		if (distanceSquared <= combinedRadiusSquared) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	for (Enemy* enemy : enemies_) {
		// 敵
		posA[1] = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			posB[1] = bullet->GetWorldPosition();
			float distanceSquared = (posA[1].x - posB[1].x) * (posA[1].x - posB[1].x) + (posA[1].y - posB[1].y) * (posA[1].y - posB[1].y) + (posA[1].z - posB[1].z) * (posA[1].z - posB[1].z);
			float combinedRadiusSquared = (radiusA[2] + radiusB[2]) * (radiusA[2] + radiusB[2]);

			if (distanceSquared <= combinedRadiusSquared) {
				enemy->OnCollision();
				bullet->OnCollision();
				enemy->SetDead();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* bullet : enemyBullets_) {
		for (PlayerBullet* bullet2 : playerBullets) {
			posA[2] = bullet->GetWorldPosition();
			posB[2] = bullet2->GetWorldPosition();

			float distanceSquared = (posA[2].x - posB[2].x) * (posA[2].x - posB[2].x) + (posA[2].y - posB[2].y) * (posA[2].y - posB[2].y) + (posA[2].z - posB[2].z) * (posA[2].z - posB[2].z);
			float combinedRadiusSquared = (radiusA[1] + radiusB[1]) * (radiusA[1] + radiusB[1]);

			if (distanceSquared <= combinedRadiusSquared) {
				bullet->OnCollision();
				bullet2->OnCollision();
			}
		}
	}

#pragma endregion
}
