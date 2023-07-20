#include "GamePlay.h"

GamePlay::GamePlay()
{

}

GamePlay::~GamePlay()
{
	// 各クラスの削除
	delete stage_;  // ステージ
	delete player_; // プレイヤー
	// ビーム
	for (Beam* beam : beam_)
	{
		delete beam;
	}	
	// 敵
	for (Enemy* enemy : enemy_)
	{
		delete enemy;
	}
	//
	for (int i = 0; i < 3; i++)
	{
		delete pLifeSprite[i];
	}
	//スコア数値の絵
	for (int i = 0; i < 5; i++)
	{
		delete numberSprite_[i];
	}
	// スコア文字
	delete scoreSprite_;
}

void GamePlay::Initialize(ViewProjection view)
{
	// デバッグテキスト
	debugText_ = DebugText::GetInstance();

	// ビュープロジェクションの初期化
	view_ = view;

	// 各クラスの生成
	stage_ = new Stage();   // ステージ
	player_ = new Player(); // プレイヤー
	// ビーム
	for (int i = 0; i < magazine_; i++)
	{
		Beam* tmp = new Beam();
		tmp->Initialize(view,player_);
		beam_.push_back(tmp);
	}
	// 敵(生成の際はインスタンスが出来てないので普通のfor文を使う)
	for (int i = 0; i < remainEnemys_; i++)
	{
		Enemy* tmp = new Enemy();
		tmp->Initialize(view);
		enemy_.push_back(tmp);
	}

	// 各クラスの初期化
	// ステージ
	stage_->Initialize(view_);
	// プレイヤー
	player_->Initialize(view_);
	// ビーム
	/*for (Beam* beam : beams_)
	{
		beam->Initialize(view_, player_);
	}*/
	// 敵
	for (Enemy* enemy : enemy_)
	{
		enemy->Initialize(view_);
	}

	//プレイヤーライフのスプライト
	pLifeTexHundle_ = TextureManager::Load("player.png");
	for (int i = 0; i < 3; i++)
	{
		pLifeSprite[i] = Sprite::Create(pLifeTexHundle_, {800.0f + i * 60, 20});
		pLifeSprite[i]->SetSize({40, 40});
	}
	//スコア数値
	numberTexhundle_ = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++)
	{
		numberSprite_[i] = Sprite::Create(numberTexhundle_, {300.0f + i * 26, 0});
	}
	//スコア文字
	scoreTexHundle_ = TextureManager::Load("score.png");
	scoreSprite_ = Sprite::Create(scoreTexHundle_, {170, 0});

	// デバッグテキスト
	debugText_->Initialize();

	// サウンドデータの読み込み
	audio_ = Audio::GetInstance();
	bgmSoundHundle_ = audio_->LoadWave("Audio/Ring08.wav");
	seDamagedHundle_ = audio_->LoadWave("Audio/chord.wav");
	seBeamHitHundle_ = audio_->LoadWave("Audio/tada.wav");

	// インプットクラス
	input_ = Input::GetInstance();
}

void GamePlay::Start()
{
	//プレイヤー
	player_->Start();
	//ビーム
	for (Beam *beam : beam_)
	{
		beam->Start();
	}	
	//敵
	for (Enemy *enemy : enemy_)
	{
		enemy->Start();
	}
	//プレイヤーライフ
	playerLife_ = 3;
	//ゲームスコア
	gameScore_ = 0;
	// 難易度設定用タイマー
	difficultyTimer_ = 0;
	//
	playerTimer_ = 0;
}

void GamePlay::BGMPlay()
{
	// BGMを再生
	bgmPlayHundle_ = audio_->PlayWave(bgmSoundHundle_, true);
}

void GamePlay::Shot()
{
	if (interval_ == 0)
	{
		// スペースキーを押したら
		if (input_->PushKey(DIK_SPACE))
		{
			// ビームでループ
			for (Beam* beam : beam_)
			{
				// 存在しなければ
				if (beam->GetFlag() == 0)
				{
					beam->Born();
					beam->Update();
					interval_ = 1;
					break;
				}
			}
		}		
	}
	else // 発射出来ない状態
	{
		// タイマー加算
		interval_++;
		// 一定時間で
		if (interval_ > 5)
		{
			// 発射できる状態
			interval_ = 0;
		}
	}
}

void GamePlay::Update(Scene &scene)
{
	if (playerTimer_ > 0)
	{
		playerTimer_--;
	}

	// 各当たり判定
	CollisionPtoE();
	CollisionBtoE();

	difficultyTimer_++;

	// 各クラスの更新
	// ステージ
	stage_->Update();
	// プレイヤー
	player_->Update();
	// 弾
	for (Beam* beam : beam_)
	{
		beam->Update();
	}	
	// 敵
	for (Enemy* enemy : enemy_)
	{
		enemy->Update(difficultyTimer_);
	}	
	//ゲームオーバー遷移条件
	if (playerLife_ < 1)
	{
		// 現在のBGMを停止
		audio_->StopWave(bgmPlayHundle_);
		scene = GAMEOVER;
	}
}

void GamePlay::DrawPlayerLife()
{
	for (int i = 0; i < playerLife_; i++)
	{
		pLifeSprite[i]->Draw();
	}
}

void GamePlay::DrawScore()
{
	//各桁の値を取り出す
	int eachNumber[5] = {};
	int number = gameScore_;

	int keta = 10000;
	for (int i = 0; i < 5; i++)
	{
		eachNumber[i] = number / keta;
		number = number % keta;
		keta = keta / 10;
	}

	//各桁の数値を描画
	for (int i = 0; i < 5; i++)
	{
		numberSprite_[i]->SetSize({32, 64});
		numberSprite_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		numberSprite_[i]->Draw();
	}
	//スコア文字
	scoreSprite_->Draw();
}

void GamePlay::Draw2Far()
{
	// 背景の描画
	stage_->Draw2DFar();
}

void GamePlay::Draw3D()
{
	// ステージ
	stage_->Draw3D();
	// 自機
	if (playerLife_ > 0)
	{
		if (playerTimer_ % 4 < 2)
		{
			player_->Draw3D();
		}		
	}
	// 弾
	for (Beam* beam : beam_)
	{
		beam->Draw3D();
	}	
	// 敵
	for (Enemy *enemy : enemy_)
	{
		if (enemy->GetFlag() != 0)
		{
			enemy->Draw3D();
		}		
	}
}

void GamePlay::Draw2DNear()
{
	// ゲームスコア
	DrawScore();
	DrawPlayerLife();
}

void GamePlay::CollisionPtoE()
{
	for (Enemy *enemy : enemy_)
	{
		// 敵が存在すれば
		if (enemy->GetFlag() == 1)
		{
			// 差を求める
			float dx = abs(player_->GetX() - enemy->GetX());
			float dz = abs(player_->GetZ() - enemy->GetZ());

			// 衝突したら
			if (dx < 1 && dz < 1)
			{
				enemy->Hit();
				audio_->PlayWave(seDamagedHundle_);
				playerTimer_ = 60;
				if (playerLife_ > 0)
				{
					playerLife_--;
				}
			}
		}
	}
}

void GamePlay::CollisionBtoE()
{
	for (Enemy *enemy : enemy_)
	{
		for (Beam* beam : beam_)
		{
			// 敵が生きていれば
			if (enemy->GetFlag() == 1 && beam->GetFlag())
			{
				// 差を求める
				float dx = abs(beam->GetX() - enemy->GetX());
				float dz = abs(beam->GetZ() - enemy->GetZ());

				// 衝突したら
				if (dx < 1 && dz < 1)
				{
					enemy->Hit();
					beam->Hit();
					audio_->PlayWave(seBeamHitHundle_);
					gameScore_ += 10;
				}
			}
		}		
	}
}