//=============================================================================
//
// �I�u�W�F�N�g3D���� [scene3D.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCENE3D_SIZE	(100)									// �|���S���̃T�C�Y
#define SCENE3D_TEXTURE "data/TEXTURE/block100.png"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene3D : public CScene
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	CScene3D();
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	float GetHeight(D3DXVECTOR3 pos);

	// �ÓI�����o�֐�
	static CScene3D * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// �v�Z�p�}�g���b�N�X

	LPD3DXMESH m_pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD m_nNumMat;						// �}�e���A�����̐�

	D3DXVECTOR3 m_vecA;
	D3DXVECTOR3 m_vecB;
	D3DXVECTOR3 m_vecC;
	bool m_bLR;			// �E������
};
#endif