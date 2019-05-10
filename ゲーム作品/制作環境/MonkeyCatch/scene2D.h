//=============================================================================
//
// �I�u�W�F�N�g2D���� [scene2D.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCENE2D_SIZE	(50)									// �|���S���̃T�C�Y
#define SCENE2D_TEXTURE "data/TEXTURE/kame.png"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene2D : public CScene
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	CScene2D();
	~CScene2D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos,float fwidth,float fheight);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	void SetColor(D3DXCOLOR col);
	void SetParticle(int nRadius, D3DXVECTOR3 pos);	

	// �ÓI�����o�֐�
	static CScene2D * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXCOLOR				m_col;			// �F 
};
#endif