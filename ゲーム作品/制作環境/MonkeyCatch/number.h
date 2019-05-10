//=============================================================================
//
// �i���o�[���� [number.h]
// Author : �|���j
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_NUMBER_SIZE (30.0f)
#define SCORE_TEXTURE "data/TEXTURE/number.png"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos,float width,float height);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	void SetColor(D3DXCOLOR col);

	// �ÓI�����o�֐�
	static CNumber * Create(D3DXVECTOR3 pos, float width, float height,D3DXCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXCOLOR				m_col;			// �F
	float					m_fWidth;		// ��
	float					m_fHeight;		// ����
};
#endif