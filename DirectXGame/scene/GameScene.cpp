#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include <cstdint>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemies_)
	{
		delete enemy;
	}

	delete model_;
	delete modelBlock_;
	delete modelEnemy_;
	delete modelDeath_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
			worldTransformBlock = nullptr;
		}
	}

	worldTransformBlocks_.clear();

	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete Cameracontroller;
	delete player_;
	delete death_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	// 3Dモデルの生成

	//textureHandle_ = TextureManager::Load("block.jpg");
	model_ = Model::Create();
	modelBlock_ = Model::CreateFromOBJ("block",true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelDeath_ = Model::CreateFromOBJ("deathParticle", true);
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	worldTransformSkydome_.Initialize();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	// 自キャラの生成

	player_ = new Player();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 16);

	// 自キャラの初期化
	player_->Initialize( &viewProjection_, playerPosition);

	// スカイドームの生成
	modelSkydome_ = Model::CreateFromOBJ("sphere", true);

	Skydome_ = new skydome();

	Skydome_->Initialize(modelSkydome_, &viewProjection_);

	player_->SetMapChipField(mapChipField_);

	GenerateBlocks();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	
	Cameracontroller = new CameraController;

	Cameracontroller->Initialize();

	Cameracontroller->SetTarget(player_);

	Cameracontroller->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	Cameracontroller->SetMovableArea(cameraArea);

	Enemy* newEnemy = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	enemies_.push_back(newEnemy);

	phase_ = Phase::kPlay;
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();

	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransForm = new WorldTransform();
				worldTransForm->Initialize();
				worldTransformBlocks_[i][j] = worldTransForm;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Update() {

	switch (phase_)
	{ 
	case Phase::kPlay:

		Cameracontroller->Update();

		// カメラ処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
			viewProjection_.matView = Cameracontroller->GetViewProjection().matView;
			viewProjection_.matProjection = Cameracontroller->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();
		}

		// 自キャラの更新
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		Skydome_->Update();

		// 縦横ブロック更新
		for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
				if (!worldTransformBlockYoko)
					continue;

				worldTransformBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_, worldTransformBlockYoko->translation_);

				// アフィン変換行列の作成
				worldTransformBlockYoko->TransferMatrix();
			}
		}

		CheckAllCollisions();

		ChangePhase();

		break;


	case Phase::kDeath:

		if (death_ && death_->IsFinished())
		{
			finished_ = true;
		}

		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
			viewProjection_.matView = Cameracontroller->GetViewProjection().matView;
			viewProjection_.matProjection = Cameracontroller->GetViewProjection().matProjection;
			// ビュープロジェクションの転送
			viewProjection_.TransferMatrix();
		}

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		Skydome_->Update();

		// 縦横ブロック更新
		for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
				if (!worldTransformBlockYoko)
					continue;

				worldTransformBlockYoko->matWorld_ = MakeAffineMatrix(worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_, worldTransformBlockYoko->translation_);

				// アフィン変換行列の作成
				worldTransformBlockYoko->TransferMatrix();
			}
		}

		if (death_) {
			death_->Update();
		}

		break;
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;
	}
#endif
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画

	// 3Dモデル描画
	modelSkydome_->Draw(worldTransformSkydome_, viewProjection_);
	/// <summary>

	model_->Draw(worldTransform_, viewProjection_,textureHandle_);

	// 縦横ブロック描画
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
		}
	}
	
	
	// 自キャラの描画
	
	if (!player_->IsDead())
	{
		player_->Draw();
	}
		

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (death_) {
		death_->Draw();
	}

	// Skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{ 
	AABB aabb1, aabb2;
	#pragma region
	{ 
		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {
			aabb2 = enemy->GetAABB();

			if (IsCollision(aabb1, aabb2))
			{
				player_->OnCollision(enemy);
				enemy->OnCollision(player_);
			}
		}

	}
    #pragma endregion
}

void GameScene::ChangePhase()
{
	switch (phase_) {
	case Phase::kPlay:
		if (player_ -> IsDead())
		{
			phase_ = Phase::kDeath;

			const Vector3& deathposition = player_->GetWorldPosition();

			death_ = new Death();

			death_->Initialize(modelDeath_, &viewProjection_, deathposition);
		}

		break;

	case Phase::kDeath:

		
		break;
	}
}

