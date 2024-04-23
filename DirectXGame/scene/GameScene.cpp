#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite;
	delete model;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle = TextureManager::Load("bed.png");
	sprite = Sprite::Create(textureHandle, {100, 50});
	model = Model::Create();
	worldTransform.Initialize();
	viewProjection.Initialize();

	soundDateHandle = audio_->LoadWave("fanfare.wav");
	//audio_->PlayWave(soundDateHandle);
	//voiceHandle = audio_->PlayWave(soundDateHandle,true);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
}

void GameScene::Update() { 
	Vector2 position = sprite->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite->SetPosition(position);

	if (input_->TriggerKey(DIK_SPACE))
	{
		audio_->StopWave(voiceHandle);
	}

	#ifdef _DEBUG
	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();
#endif // DEBUG

	ImGui::ShowDemoWindow();
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
	
	//sprite->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	model->Draw(worldTransform, viewProjection, textureHandle);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	
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
