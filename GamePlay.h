#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Stage.h"
#include "Player.h"
#include "Beam.h"
#include "Enemy.h"
#include "DebugText.h"
#include "SceneEnum.h"

class GamePlay : public SceneEnum
{
private:
	//ビュープロジェクション
	ViewProjection view_;

	// 各クラス
	// ステージ
	Stage *stage_ = nullptr;
	// プレイヤー
	Player *player_ = nullptr;
	// 弾
	static const int magazine_ = 10;
	Beam* beams_[magazine_] = {};
	// 敵
	static const int remainEnemys_ = 10;
	Enemy* enemys_[remainEnemys_] = {};

	// デバッグテキスト
	DebugText* debugText_ = nullptr;

	int gameScore_ = 0;  // ゲームスコア
	int playerLife_ = 3; // プレイヤーの残機

public:
	//コンストラクタ
	GamePlay();

	//デストラクタ
	~GamePlay();

	//初期化
	void Initialize(ViewProjection view);

	//2周目以降の初期化
	void Start();

	//
	void Shot();

	//更新
	void Update(Scene &scene);

	//2D背景描画
	void Draw2Far();

	//3D描画
	void Draw3D();

	//2D前景描画
	void Draw2DNear();

	// 衝突判定(自機と敵)
	void CollisionPtoE();

	// 衝突判定(ビームと敵)
	void CollisionBtoE();
};