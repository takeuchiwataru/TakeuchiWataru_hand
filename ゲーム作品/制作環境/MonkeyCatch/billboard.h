//=============================================================================
//
// �r���{�[�h�̏��� [billboard.h]
// Author : �|���j
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BILLBOARD_SIZE	(5)									// �r���{�[�h�̃T�C�Y

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CBillBoard : public CScene
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	CBillBoard();
	~CBillBoard();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos,D3DXVECTOR3 move);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

	void SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight);
	void SetColor(D3DXCOLOR col);
	void SetParticle(int nRadius, D3DXVECTOR3 pos);
	void SetParticle2(int nRadius, D3DXVECTOR3 pos);

	// �ÓI�����o�֐�
	static CBillBoard * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXVECTOR3				m_rot;			// ����
	D3DXCOLOR				m_col;			// �F 
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X

};

#endif
