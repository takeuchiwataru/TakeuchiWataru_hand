//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CObject;
class CEnemy;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE_SPEED (2.0f)
#define PLAYER_RUN_SPEED (3.0f)
#define PLAYER_JUMP (8.0f)
#define MAX_PLAYER_MODEL (11)			// プレイヤーのパーツ数
#define NUM_MAX_KEY (4)					// キー最大数
#define NUM_MAX_PLAYER_MOTION (5)		// モーション最大数
#define PLAYER_OFFSET (2)
#define PLAYER_MAX_POS	(1490)			// ステージ最大
#define PLAYER_MIN_POS	(0)				// ステージ最小
#define PLAYER_GRAVITY	(0.5f)			// 重力
#define PLAYER_MOVESTAND	(0.2f)		// 移動基準
#define PLAYER_DIFF	(0.3f)				// 角度の差分
#define PLAYER_AROUND	(2.0f)			// 角度の周り
#define ATTACK_COUNTER (35)				// 攻撃状態のカウンター

#define PLAYER_VECLIGHT	(D3DXVECTOR4(-0.22f, 0.4f, -0.2f, 0.0f))	// 影用のライト
#define PLAYER_POS	(D3DXVECTOR3(0.0f, 0.8f, 0.0f))					// 影用の位置
#define PLAYER_NORMAL	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))				// 影用の法線

#define P_MAX_PARTICLE (1)				// パーティクル数
#define P_PARTICLE_LIFE (15)			// パーティクルライフ
#define P_PARTICLE_RADIUS (15)			// パーティクルの大きさ
#define P_PARTICLE_CREATEPOS	(D3DXVECTOR3(m_pos.x, m_pos.y + 10, m_pos.z))	// パーティクル生成位置
#define P_PARTICLE_AROUND	(rand() % 628 / 100.0f)								// パーティクル周り
#define P_PARTICLE_RAND	(0.5f)										// ランダムに出すパーティクル

#define PLAYER_FILE ("data/TEXT/motion_player.txt")
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
class CPlayer : public CScene
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
	}P_KEY;

	typedef struct
	{// キー情報
		int nFrame;
		P_KEY aKey[MAX_PLAYER_MODEL];
	}KEY_INFO;

	typedef struct
	{// モーション情報
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[NUM_MAX_KEY];
	}MOTION_INFO;

	typedef enum
	{// プレイヤーの状態
		PLAYERSTATE_NONE = 0,
		PLAYERSTATE_NORMAL,		// 通常
		PLAYERSTATE_MOVE,		// 移動
		PLAYERSTATE_JUMP,		// ジャンプ
		PLAYERSTATE_LANDING,	// 着地
		PLAYERSTATE_MUSIAMI,	// 虫網
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer();
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void UpdateTutorial(void);
	void Draw(void);
	void Move(void);

	void LoadMotion(void);
	void UpdateMotion(void);
	char *ReadLine(FILE * pFile, char *pDst);
	char *GetLineTop(char *pSrc);
	int PopString(char * pSrc, char * pDst);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetMove(void);
	PLAYERSTATE GetState(void);
	void SetAngle(void);
	void MapCollision(void);
	void CollisionAll(void);
	bool AttackCollision(D3DXVECTOR3 pos, float radius);

	// 静的メンバ関数
	static CPlayer * Create(D3DXVECTOR3 pos);

	// 静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;							// 共有テクスチャのポインタ
	static LPD3DXMESH		m_apMesh[MAX_PLAYER_MODEL];				// メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_apBuffMat[MAX_PLAYER_MODEL];			// マテリアル情報へのポインタ
	static DWORD			m_anNumMat[MAX_PLAYER_MODEL];			// マテリアル情報の数
	static CMeshOrbit * m_pMeshOrbit;
	static CObject * m_pObject;

private:
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posold;						// 位置
	D3DXVECTOR3 m_move;							// 移動
	D3DXVECTOR3 m_rot;							// 向き
	PLAYERSTATE m_state;						// 状態
	D3DXVECTOR3	m_aPosOffset[MAX_PLAYER_MODEL];	// モデルのオフセット
	D3DXVECTOR3 m_aPos[MAX_PLAYER_MODEL];		// 位置保存(パーツ分)
	D3DXVECTOR3 m_aRot[MAX_PLAYER_MODEL];		// 向き保存(パーツ分)
	D3DXMATRIX	m_mtxWorld;						// ワールドマトリックス
	bool m_bJump;								// ジャンプ中かどうか
	bool m_bLand;								// 着地中かどうか
	float m_fDest;								// 目的の角度
	float m_fDiff;								// 角度の差分

	bool m_bAttack;								// 攻撃してるかどうか
	bool m_bHit;								// 当たったかどうか
	int	m_nAttackCounter;						// 攻撃カウンター
	int	m_nJumpCounter;							// ジャンプカウンター

	CModel * m_apModel[MAX_PLAYER_MODEL];
	D3DXVECTOR3 m_VtxMin, m_VtxMax;				// 最小値、最大値

	KEY_INFO *m_pKeyInfo;						// キー情報のポインタ
	int m_nKey;									// 現在のキーNo.
	int m_nCouterMotion;						// モーションカウンター
	int m_StateSave;							// 状態保存用
	int m_nNumParts;							// パーツ数
	int m_aIndexParent[MAX_PLAYER_MODEL];		// インデックス
	P_KEY m_aKeyOffset[MAX_PLAYER_MODEL];
	MOTION_INFO m_aMotionInfo[NUM_MAX_PLAYER_MOTION];
	char m_aFilenameModel[MAX_PLAYER_MODEL][MAX_CHAR];
};
#endif