//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : �|���j
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "sceneX.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OBJECT_MODEL_NAME000 "data/MODEL/tree3.x"
#define OBJ_SHADOWPOS (0.3f)	// �e���ςɂȂ�Ȃ��悤�ɂ��Ƃ����

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CObject : public CSceneX
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:

	typedef enum
	{// �I�u�W�F�N�g�̎��
		OBJECTTYPE_TREE = 0,
		OBJECTTYPE_MAX
	}OBJECTTYPE;

	CObject();
	~CObject();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin);

	// �ÓI�����o�֐�
	static CObject * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type);
	static HRESULT LoadX(void);
	static void UnloadX(void);

	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9		*m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH		m_pMesh;				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER		m_pBuffMat;				// �}�e���A�����ւ̃|�C���^
	static DWORD			m_nNumMat;				// �}�e���A�����̐�

private:
	D3DXVECTOR3				m_pos;					// �ʒu
	D3DXVECTOR3				m_rot;					// ����
	D3DXVECTOR3				m_move;					// �ړ�
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	OBJECTTYPE				m_type;					// ���
	D3DXMATRIX m_mtxRot, m_mtxTrans;			// �v�Z�p�}�g���b�N�X

};
#endif