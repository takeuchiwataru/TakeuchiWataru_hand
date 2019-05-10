//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : �|���j
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_POS_X	(0)                         // �t�F�[�h�̍���w���W
#define FADE_POS_Y	(0)                         // �t�F�[�h�̍���x���W
#define FADE_WIDTH	(SCREEN_WIDTH)              // �t�F�[�h�̕�
#define FADE_HEIGHT	(SCREEN_HEIGHT)             // �t�F�[�h�̍���
#define FADE_TOMEI	(0.05f)						// ����
#define FADE_NOTTOMEI	(0.02f)					// �s����
#define FADE_COLOR000	(0.0f)
#define FADE_COLOR001	(1.0f)

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CFade
{
public:
	typedef enum
	{// �t�F�[�h�̏��
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX
	}FADE;

	CFade();
	~CFade();
	void Init(CManager::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CFade * Create();
	static void Set(CManager::MODE modeNext, FADE fade);
	static FADE GetFade(void);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	static FADE				m_fade;			// �t�F�[�h���
	static CManager::MODE	m_modeNext;		// ���̃��[�h
	static D3DXCOLOR		m_colorFade;	// �t�F�[�h�F
};
#endif

 