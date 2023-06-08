﻿#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "Input.h"

class Player
{
private:
	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	//プレイヤー
	uint32_t playerTexHundle_ = 0;
	Model *playerModel_ = nullptr;
	WorldTransform playerWorldTransform_;

	Input* input_ = nullptr;

public:
	//コンストラクタ
	Player();

	//デストラクタ
	~Player();

	//初期化
	void Initialize(ViewProjection viewProjection);

	//更新
	void Update();

	//2D背景描画
	void Draw3D();
};