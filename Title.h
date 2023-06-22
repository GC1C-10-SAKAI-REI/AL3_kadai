#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Title
{
private:
	//タイトル
	uint32_t titleTexHundle_ = 0;
	Sprite *titleSprite_ = nullptr;
	//ヒットエンターキー
	uint32_t hitEnterTexHundle_ = 0;
	Sprite *hitEnterSprite_ = nullptr;

public:
	// コンストラクタ
	Title();

	// デストラクタ
	~Title();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 2D前景描画
	void Draw2DNear();
};