//=============================================================================
//
// �t���[������ [logo.cpp]
// Author : �|���j
//
//=============================================================================
#ifndef _FRAME_H_
#define _FRAME_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// �t���[���N���X�̒�`
//*****************************************************************************
class CFrame : public CLogo
{
public:
	typedef enum
	{// �t���[���̎��
		FRAMETYPE_TIMER,
		FRAMETYPE_SCORE,
		FRAMETYPE_MAX,
	}FRAMETYPE;

	CFrame();
	~CFrame();
	HRESULT Init(D3DXVECTOR3 pos,FRAMETYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CFrame * Create(D3DXVECTOR3 pos,FRAMETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[FRAMETYPE_MAX];	// ���L�e�N�X�`���̃|�C���^
	FRAMETYPE m_nType;										// �t���[���̎��
	float m_fWidth;											// ��
	float m_fHeight;										// ����
};
#endif
