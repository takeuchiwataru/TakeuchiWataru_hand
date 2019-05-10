//=============================================================================
//
// �摜���� [image.cpp]
// Author : �|���j
//
//=============================================================================
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define IMAGE_TEXTURE000 "data/TEXTURE/remain.png"			// �T���̃J�E���g�摜
#define IMAGE_TEXTURE001 "data/TEXTURE/UI000.png"			// �c��
#define IMAGE_TEXTURE002 "data/TEXTURE/tutorial000.png"		// �R���g���[���[����
#define IMAGE_TEXTURE003 "data/TEXTURE/UI001.png"			// �߂܂�����
#define IMAGE_TEXTURE004 "data/TEXTURE/UI002.png"			// �c�莞��
#define IMAGE_TEXTURE005 "data/TEXTURE/UI003.png"			// ���v

//*****************************************************************************
// �摜�N���X�̒�`
//*****************************************************************************
class CImage : public CLogo
{
public:
	typedef enum
	{// UI�̎��
		IMAGE_REMAIN,
		IMAGE_NOKORI,
		IMAGE_TUTORIAL,
		IMAGE_CATCHNUM,
		IMAGE_REMAINTIME,
		IMAGE_TOTAL,
		IMAGE_MAX,
	}IMAGE;

	CImage();
	~CImage();
	HRESULT Init(D3DXVECTOR3 pos, IMAGE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CImage * Create(D3DXVECTOR3 pos, IMAGE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[IMAGE_MAX];	// ���L�e�N�X�`���̃|�C���^
	IMAGE m_nType;										// �摜�̎��
};

#endif
