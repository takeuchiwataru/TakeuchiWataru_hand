//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_PRIORITY (8)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene
{
public:
	typedef enum
	{// オブジェクトの種類
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,
		OBJTYPE_3D,
		OBJTYPE_X,
		OBJTYPE_BILLBOARD,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_OBJECT,
		OBJTYPE_GRASS,
		OBJTYPE_ITEM,
		OBJTYPE_FRAME,
		OBJTYPE_PARTICLE,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual	HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual	void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	
	void SetObjType(OBJTYPE ObjType);
	OBJTYPE GetObjType(void);
	CScene *GetTop(int nPriority = 3);
	CScene *GetNext(void);
	bool GetDeath(void);

	// 静的メンバ関数
	static void ReleseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	void UninitAll(int nPriority);

protected:
	void Release(void);
	CScene * m_pNext;			// 次のオブジェクトへのポインタ

private:
	static CScene * m_apTop[NUM_PRIORITY];		// 先頭のオブジェクトへのポインタ
	static CScene * m_apCur[NUM_PRIORITY];		// 現在(最後尾)のオブジェクトへのポインタ
	CScene * m_pPrev;			// 前のオブジェクトへのポインタ
	int m_nPriority;			// 優先順位の番号
	OBJTYPE m_objType;			// オブジェクトの種類
	bool m_bDeath;				// 死亡フラグ

	static int m_nNumAll;
	static int m_nNumPriority[NUM_PRIORITY];
	int m_nID;	// 自分自身の番号
};
#endif