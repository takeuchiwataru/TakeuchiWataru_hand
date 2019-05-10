//=============================================================================
//
// �e���� [shadow.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_TEXTURE "data/TEXTURE/shadow000.jpg"
#define SHADOW_SIZE (25.0f)		// �e�̃T�C�Y

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CShadow : public CScene
{
public:
	CShadow();
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void DeleteShadow(void);

	// �ÓI�����o�֐�
	static CShadow * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// �v�Z�p�}�g���b�N�X
	bool					m_bUse;			// �g�p���Ă��邩�ǂ���
};

#endif