//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// 前方宣言 インクルードはいらない
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE (2.0f)
#define ANGLE_VIEW (45.0f)
#define CAMERA_SIDE	(10.0f)
#define CAMERA_MOTH	(10000.0f)
#define CAMERA_POSV (0.2f)
#define CAMERA_POSR (0.2f)
#define CAMERA_DEST	(0.3f)
#define CAMERA_CENTER	(D3DXVECTOR3(750.0f, 50.0f, -750.0f))
#define CAMERA_LENGTH_GAME_XZ	(250)
#define CAMERA_LENGTH_GAME_Y	(100)
#define CAMERA_LENGTH_TITLE_XZ	(400)
#define CAMERA_LENGTH_TITLE_Y	(0)
#define CAMERA_ROT_TITLE	(1200)
#define CAMERA_ROT_GAME	(120)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	D3DXVECTOR3 GetCamera(void);

private:
	typedef enum
	{// カメラの状態
		CAMERASTATE_NORMAL = 0, // 通常
		CAMERASTATE_MOVE,       // 移動
		CAMERASTATE_SPIN,       // 旋回
		CAMERASTATE_MAX
	}CAMERASTATE;

	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXVECTOR3 m_posVDest;		// 目的の視点
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
	D3DXVECTOR3 m_posVAdd;		// 目的の座標
	D3DXVECTOR3 m_posRAdd;		// 目的の座標
	D3DXVECTOR3 m_rotDest;		// 目的の向き

	float m_rotDiff;			// 向きの差分
	D3DXVECTOR3 m_fLength;		// 距離
	CAMERASTATE m_state;		// 状態
	D3DVIEWPORT9 m_ViewPort;	// ビューポート

	static CPlayer *m_pPlayer;
	static CPlayer *m_pPlayerT;
};
#endif