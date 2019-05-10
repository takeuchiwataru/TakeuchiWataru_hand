//=============================================================================
//
// �|�[�Y���ڏ��� [pauseselect.h]
// Author : �|���j
//
//=============================================================================
#ifndef _PAUSESELECT_H_
#define _PAUSESELECT_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSESELECT_WIDTH (150)
#define PAUSESELECT_HEIGHT (50)
#define MAX_PAUSE_SELECT (RESTART_MAX)
#define PAUSESELECT_INTERVAL (150)

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CPauseSelect : public CScene
{
public:
	typedef enum
	{// ���
		PAUSESELECT_NONE = 0,   // �I������Ă��Ȃ����
		PAUSESELECT_SELECT,     // �I������Ă�����
		PAUSESELECT_MAX
	}PAUSESELECT;

	typedef enum
	{// ����
		RESTART_CONTINUE,  // �R���e�B�j���[
		RESTART_RETRY,     // �Q�[�����g���C
		RESTART_QUIT,      // �^�C�g���J��
		RESTART_MAX
	}RESTART;

	CPauseSelect();
	~CPauseSelect();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static RESTART GetRstart(void);

	// �ÓI�����o�֐�
	static CPauseSelect * Create();
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[RESTART_MAX];	// ���L�e�N�X�`���̃|�C���^
														// RESTART m_Restart;								// �J�ڂ����
	static RESTART m_Restart;
	RESTART m_nType;									// ���
	PAUSESELECT m_aSelect[MAX_PAUSE_SELECT];
	D3DXCOLOR m_aCol[MAX_PAUSE_SELECT];
	int m_nSelect;										// �I��
	float m_fCol;										// �F�̕ω��Ɏg�p
	CLogo * m_apLogo[MAX_PAUSE_SELECT];

};
#endif
