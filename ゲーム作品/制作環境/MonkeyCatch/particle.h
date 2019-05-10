//=============================================================================
//
// パーティクルの処理 [particle.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scene2D.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_TEXTURE_000 "data/TEXTURE/effect000.jpg"		// エフェクトのテクスチャ
#define PARTICLE_TEXTURE_001 "data/TEXTURE/effect000.jpg"		// エフェクトのテクスチャ

#define PARTICLE_SIZE000 (10)			// エフェクトサイズ
#define PARTICLE_SIZE001 (5)			// エフェクトサイズ
#define PARTICLE_LIFE (10)				// エフェクトライフ
#define PARTICLE_RADIUS (15)			// エフェクトの半径
#define PARTICLE_GRAVITY (0.03f)		// エフェクトの重力

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CParticle : public CBillBoard
{// 派生させる時は : public ～を後ろにつけてやる
public:
	typedef enum
	{// パーティクルの種類
		PARTICLETYPE_000,		// 
		PARTICLETYPE_001,		// 
		PARTICLETYPE_MAX
	}PARTICLETYPE;

	CParticle();
	~CParticle();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetType(PARTICLETYPE type);


	// 静的メンバ関数
	static CParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius, PARTICLETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTICLETYPE_MAX];	// 共有テクスチャのポインタ
	D3DXVECTOR3				m_move;								// 移動量
	int						m_nLife;							// 表示時間
	int					m_nRadius;								// 半径
	D3DXCOLOR				m_col;								// 色
	PARTICLETYPE m_type;										// 種類
};
#endif