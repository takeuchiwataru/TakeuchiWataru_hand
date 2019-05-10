//=============================================================================
//
// �I�u�W�F�N�gX���� [sceneX.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CSceneX : public CScene
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	CSceneX();
	~CSceneX();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw2(void);
	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPosition(void);
	void BindModelTex(LPDIRECT3DTEXTURE9 *pTexture,LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetMax(void);
	D3DXVECTOR3 GetMin(void);

	// �ÓI�����o�֐�
	static CSceneX * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		*m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_rot;				// ����
	D3DXVECTOR3				m_move;				// �ړ�

	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9			m_matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXVECTOR3 m_VtxMin, m_VtxMax;				// ���f���̍ŏ��l�A�ő�l

	LPD3DXMESH		m_pMesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	m_pBuffMat;					// �}�e���A�����ւ̃|�C���^
	DWORD			m_nNumMat;					// �}�e���A�����̐�
};
#endif