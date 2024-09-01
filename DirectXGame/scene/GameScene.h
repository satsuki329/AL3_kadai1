#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "skydome.h"
#include "Death.h"
#include <vector>
#include <list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modeldeath_ = nullptr;
	//Model* modelCameracontroller = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	Death* death_ = nullptr;

	// 縦横ブロック配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	skydome* Skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	WorldTransform worldTransformSkydome_;

	MapChipField* mapChipField_;

	CameraController* Cameracontroller = nullptr;

	std::list<Enemy*> enemies_;

	void CheckAllCollisions();
};