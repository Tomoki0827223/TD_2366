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
	delete skydome2_;
	delete railCamera_;
	delete modelEnemyBullet_;
	delete modelPlayerbullet_;
	delete modelPlayerbullet2_;
	delete modelPlayerbullet3_;
	delete modelSkydome2_;
	delete modelGround_;
	delete sprite_;
	delete sprite2_;
	delete ground_;
	delete modelWall_;
	delete wall_;
	delete wall2_;
	delete hertSprite_;
	delete hertSprite2_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	enemyBullets_.clear(); // 追加

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear(); // 追加
}

void GameScene::Initialize() {
	
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	skydome_ = new Skydome();
	skydome2_ = new Skydome2();
	ground_ = new GroundModel();
	wall_ = new Wall();
	wall2_ = new Wall();

	// 3Dモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelSkydome2_ = Model::CreateFromOBJ("skydome2", true);

	modelEnemyBullet_ = Model::CreateFromOBJ("Tama", true);
	modelPlayerbullet_ = Model::CreateFromOBJ("TamaPlayer", true);
	modelPlayerbullet2_ = Model::CreateFromOBJ("cube", true);
	modelPlayerbullet3_ = Model::CreateFromOBJ("TamaPlayer", true);

	modelGround_ = Model::CreateFromOBJ("ground", true);

	modelWall_ = Model::CreateFromOBJ("kabe", true);
	modelWall2_ = Model::CreateFromOBJ("kabe", true);

	camera_.Initialize();
	player_->Initialize(modelPlayer_, &camera_, modelPlayerbullet_, modelPlayerbullet2_, modelPlayerbullet3_, playerPos);

	skydome_->Initialize(modelSkydome_, &camera_);
	skydome2_->Initialize(modelSkydome2_, &camera_);
	ground_->Initialize(modelGround_, &camera_, groundPos);
	wall_->Initialize(modelWall_, &camera_, WallPos);
	wall2_->Initialize(modelWall2_, &camera_, WallPos2);

	textureHandle_ = TextureManager::Load("hpBarBuck.png");
	textureHandle2_ = TextureManager::Load("hpBarFront.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	sprite2_ = Sprite::Create(textureHandle2_, {100, 50});

	hertTextureHandle_ = TextureManager::Load("hert.png");
	hertSprite_ = Sprite::Create(hertTextureHandle_, {900, 50});
	hertTextureHandle2_ = TextureManager::Load("hertHP.png");
	hertSprite2_ = Sprite::Create(hertTextureHandle2_, {900, 50});


	// ビットマップフォントの読み込み
	fontTextureHandle_ = TextureManager::Load("number.png"); // フォントのビットマップ画像を読み込む
	for (int c = 0; c <= 9; ++c) {

		charSprites_[c] = Sprite::Create(fontTextureHandle_, {c * 32.0f, c *0.0f}); // 各文字のスプライトを作成
		charSprites_[c]->SetSize({32, 64});
		charSprites_[c]->SetTextureRect({8, 0}, {32, 64});
	}


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

	wall_->Update();
	wall2_->Update();
	ground_->Update();

	// 経過時間を更新
	elapsedTime_ -= 1.0f / 24.0f; // 1フレームあたりの時間を加算 (60FPSの場合)

	// 60秒経過したらゲームクリア
	if (elapsedTime_ <= 0.0f) {
		isFinished_ = true;
		return;
	}

	// プレイヤーのHPをチェック
	if (player_->IsDead()) {
		isGameOver_ = true;
		return;
	}

	// プレイヤーの位置を取得
	Vector3 playerPosition = player_->GetWorldPosition();

	// 敵の更新と削除判定
	for (auto it = enemies_.begin(); it != enemies_.end();) {
		Enemy* enemy = *it;
		enemy->Update();

		// 敵の位置を取得
		Vector3 enemyPosition = enemy->GetWorldPosition();

		// 敵がプレイヤーの後ろ側にいるかどうかを判定
		if (enemyPosition.z < playerPosition.z) {
			// 敵を削除
			delete enemy;
			it = enemies_.erase(it);
		} else {
			++it;
		}
	}

	// 他の更新処理
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


	Vector2 size = sprite2_->GetSize();
	size.x = nowHp / maxHp * width;
	size.y = 10.0f;

	Vector2 hertSize = hertSprite2_->GetSize();
	hertSize.x = nowHertHP / maxHertHP * hertWidth;
	hertSize.y = 256.0f;

	sprite2_->SetSize(size);
	hertSprite2_->SetSize(hertSize);

	//// 無敵状態のカウントダウン
	//if (isInvisible_) {
	//	invisibleTimmer--;
	//	if (invisibleTimmer <= 0) {
	//		isInvisible_ = false;
	//		invisibleTimmer = 10 * 5;
	//	}
	//}

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

	player_->Draw(camera_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(camera_);
	}

	//modelGround_->Draw(worldTransform_, camera_);

	// 昼夜変更
	if (elapsedTime_ <= 20.0f) {
		skydome2_->Draw();
	} else {
		skydome_->Draw();
	}

	ground_->Draw();

	wall_->Draw();
	wall2_->Draw();

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	Sprite::PreDraw(commandList);

	sprite_->Draw();
	sprite2_->Draw();
	hertSprite_->Draw();
	hertSprite2_->Draw();

	// 制限時間の描画
	DrawText();


	Sprite::PostDraw();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
#pragma endregion
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

			nowHp -= rand() % 11 + 1;

			//if (isInvisible_ == false) {
			//	nowHp -= rand() % 11 + 1;
			//	isInvisible_ = true;
			//}
			
			if (nowHp <= 0) {
				nowHp = 0;
				player_->IsDead();

				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(bullet);
				// 敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision(player_);
			}

			//if (isInvisible_ == true) {
			//	invisibleTimmer--;
			//}
			//if (invisibleTimmer <= 0) {
			//	isInvisible_ = false;
			//	invisibleTimmer = 10 * 5;
			//}
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
				bullet->OnCollision(player_);
				bullet2->OnCollision();
			}
		}
	}

#pragma endregion
}

void GameScene::DrawText() {

	// 経過時間を描画
	int Time = (int)elapsedTime_;

	// 桁をずらす
	for (int i = 0; i < 3; i++) {

		// 1の位の数字を取得
		int number = Time % 10;
		// 1の位の数字を描画
		charSprites_[2 - i]->SetTextureRect({(float)number * 32, 0}, {32, 64});
		// 桁をずらす
		Time /= 10;
		// ワールドトランスフォームを設定して描画
		charSprites_[i]->Draw();
	}
}
