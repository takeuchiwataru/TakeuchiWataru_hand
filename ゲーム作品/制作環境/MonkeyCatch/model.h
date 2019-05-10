//=============================================================================
//
// ���f������ [model.h]
// Author : �|���j
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CModel
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	CModel();
	~CModel();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pNumMat);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	void SetParent(CModel * pModel);
	D3DXMATRIX &GetMtxWorld(void);
	void SetRot(D3DXVECTOR3 rot);
	void SetPos(D3DXVECTOR3 pos);

	// �ÓI�����o�֐�
	static CModel * Create(D3DXVECTOR3 pos);

private:
	// X�t�@�C���Ǘ��ɕK�v�ȗv��
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_rot;				// ����
	D3DXVECTOR3				m_move;				// �ړ�
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X

	LPD3DXMESH		m_pMesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	m_pBuffMat;					// �}�e���A�����ւ̃|�C���^
	DWORD			m_nNumMat;					// �}�e���A�����̐�

	D3DXVECTOR3 m_VtxMinModel, m_VtxMaxModel;	// ���f���̍ŏ��l�A�ő�l

	CModel * m_pParent;							// �e���f���̃|�C���^
};
#endif