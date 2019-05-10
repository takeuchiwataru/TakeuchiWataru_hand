//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PRIORITY (8)

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene
{
public:
	typedef enum
	{// �I�u�W�F�N�g�̎��
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

	// �ÓI�����o�֐�
	static void ReleseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	void UninitAll(int nPriority);

protected:
	void Release(void);
	CScene * m_pNext;			// ���̃I�u�W�F�N�g�ւ̃|�C���^

private:
	static CScene * m_apTop[NUM_PRIORITY];		// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene * m_apCur[NUM_PRIORITY];		// ����(�Ō��)�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene * m_pPrev;			// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nPriority;			// �D�揇�ʂ̔ԍ�
	OBJTYPE m_objType;			// �I�u�W�F�N�g�̎��
	bool m_bDeath;				// ���S�t���O

	static int m_nNumAll;
	static int m_nNumPriority[NUM_PRIORITY];
	int m_nID;	// �������g�̔ԍ�
};
#endif