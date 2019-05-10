//=============================================================================
//
// 画像処理 [image.cpp]
// Author : 竹内亘
//
//=============================================================================
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define IMAGE_TEXTURE000 "data/TEXTURE/remain.png"			// サルのカウント画像
#define IMAGE_TEXTURE001 "data/TEXTURE/UI000.png"			// 残り
#define IMAGE_TEXTURE002 "data/TEXTURE/tutorial000.png"		// コントローラー説明
#define IMAGE_TEXTURE003 "data/TEXTURE/UI001.png"			// 捕まえた数
#define IMAGE_TEXTURE004 "data/TEXTURE/UI002.png"			// 残り時間
#define IMAGE_TEXTURE005 "data/TEXTURE/UI003.png"			// 合計

//*****************************************************************************
// 画像クラスの定義
//*****************************************************************************
class CImage : public CLogo
{
public:
	typedef enum
	{// UIの種類
		IMAGE_REMAIN,
		IMAGE_NOKORI,
		IMAGE_TUTORIAL,
		IMAGE_CATCHNUM,
		IMAGE_REMAINTIME,
		IMAGE_TOTAL,
		IMAGE_MAX,
	}IMAGE;

	CImage();
	~CImage();
	HRESULT Init(D3DXVECTOR3 pos, IMAGE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CImage * Create(D3DXVECTOR3 pos, IMAGE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[IMAGE_MAX];	// 共有テクスチャのポインタ
	IMAGE m_nType;										// 画像の種類
};

#endif
