//=============================================================================
//
// �|�[�Y�䎆���� [pausemat.h]
// Author : �|���j
//
//=============================================================================
#ifndef _PAUSEMAT_H_
#define _PAUSEMAT_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PAUSEMAT_WIDTH (250)
#define PAUSEMAT_HEIGHT (250)

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CPauseMat : public CLogo
{
public:
	CPauseMat();
	~CPauseMat();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CPauseMat * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`���̃|�C���^
};
#endif
