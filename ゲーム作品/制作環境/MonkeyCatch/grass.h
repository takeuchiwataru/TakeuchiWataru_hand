//=============================================================================
//
// 草処理 [grass.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _GRASS_H_
#define _GRASS_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CObject;
class CPlayer;
class CRemain;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_GRASS_MODEL	(9)				// 草のパーツ数
#define NUM_MAX_GRASS_KEY	(2)			// キー最大数
#define NUM_MAX_GRASS_MOTION	(1)		// モーション最大数
#define GRASS_MOVESTAND	(0.2f)			// 移動基準
#define G_MAX_PARTICLE (40)				// パーティクル数
#define G_PARTICLE_LIFE (30)			// パーティクルライフ
#define G_PARTICLE_RADIUS (30)			// パーティクルの大きさ
#define G_PARTICLE_CREATEPOS	(D3DXVECTOR3(m_pos.x, m_pos.y + 40, m_pos.z))	// パーティクル生成位置
#define G_PARTICLE_AROUND	(rand() % 628 / 100.0f)								// パーティクル周り
#define G_PARTICLE_RAND	(rand() % 7 + 1)										// ランダムに出すパーティクル
#define GRASS_COLLISION (3500.0f)		// 草の当たり判定の半径

#define GRASS_FILE ("data/TEXT/motion_grass.txt")
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME   "MODEL_FILENAME = "
#define CHARACTERSET "CHARACTERSET"
#define END_CHARACTERSET "END_CHARACTERSET"
#define NUM_PARTS "NUM_PARTS = "
#define PARTSSET "PARTSSET"
#define END_PARTSSET "END_PARTSSET"
#define INDEX "INDEX = "
#define PARENT "PARENT = "
#define POS "POS = "
#define ROT "ROT = "

#define MOTIONSET "MOTIONSET"
#define END_MOTIONSET "END_MOTIONSET"
#define LOOP "LOOP = "
#define NUM_KEY "NUM_KEY = "
#define KEYSET "KEYSET"
#define END_KEYSET "END_KEYSET"
#define FRAME "FRAME = "
#define KEY "KEY"
#define END_KEY "END_KEY"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CGrass : public CScene
{
public:
	typedef struct
	{// キー設定
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	}E_KEY;

	typedef struct
	{// キー情報
		int nFrame;
		E_KEY aKey[MAX_GRASS_MODEL];
	}KEY_INFO;

	typedef struct
	{// モーション情報
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[NUM_MAX_GRASS_KEY];
	}MOTION_INFO;

	typedef enum
	{// 草の状態
		GRASSSTATE_NONE = 0,
		GRASSSTATE_NORMAL,		// 通常
		GRASSSTATE_MOVE,		// 移動
		GRASSSTATE_MAX
	}GRASSSTATE;

	typedef enum
	{// 草の種類
		GRASSTYPE_000,			// 壊れない
		GRASSTYPE_001,			// 壊れる
		GRASSTYPE_MAX
	}GRASSTYPE;

	CGrass();
	~CGrass();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadMotion(void);
	void UpdateMotion(void);
	char *ReadLine(FILE * pFile, char *pDst);
	char *GetLineTop(char *pSrc);
	int PopString(char * pSrc, char * pDst);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin);
	void SetType(GRASSTYPE type);

	// 静的メンバ関数
	static CGrass * Create(D3DXVECTOR3 pos, GRASSTYPE type);

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャのポインタ
	static LPD3DXMESH		m_apMesh[MAX_GRASS_MODEL];				// メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_apBuffMat[MAX_GRASS_MODEL];			// マテリアル情報へのポインタ
	static DWORD			m_anNumMat[MAX_GRASS_MODEL];			// マテリアル情報の数
	static CMeshOrbit * m_pMeshOrbit;
	static CObject * m_pObject;
	static CPlayer *m_pPlayer;

private:
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posold;						// 位置
	D3DXVECTOR3 m_move;							// 移動
	D3DXVECTOR3 m_rot;							// 向き
	GRASSSTATE m_state;							// 状態
	D3DXVECTOR3	m_aPosOffset[MAX_GRASS_MODEL];	// モデルのオフセット
	D3DXMATRIX	m_mtxWorld;						// ワールドマトリックス
	bool m_bJump;								// ジャンプするかしないか
	bool m_bDisp;								// 表示するかどうか
	float m_fDest;								// 目的の角度
	float m_fDiff;								// 角度の差分

	CModel * m_apModel[MAX_GRASS_MODEL];
	D3DXVECTOR3 m_VtxMin, m_VtxMax;				// 最小値、最大値

	KEY_INFO *m_pKeyInfo;					// キー情報のポインタ
	int m_nKey;								// 現在のキーNo.
	int m_nCouterMotion;					// モーションカウンター
	int m_StateSave;						// 状態保存用
	int m_nNumParts;						// パーツ数
	int m_aIndexParent[MAX_GRASS_MODEL];	// インデックス
	E_KEY m_aKeyOffset[MAX_GRASS_MODEL];
	MOTION_INFO m_aMotionInfo[NUM_MAX_GRASS_MOTION];
	char m_aFilenameModel[MAX_GRASS_MODEL][256];

	bool m_bHit;		// 当たったかどうか
	bool m_bUse;
	GRASSTYPE m_type;						// 敵の種類

};
#endif