//=============================================================================
//
// �|���S������ [polygon.h]
// Author : �|���j
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LIFE_SIZE (30.0f)
#define LIFE_TEXTURE "data/TEXTURE/life000.png"					// ���C�t�̃e�N�X�`��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CPolygon
{
public:
	CPolygon();
	~CPolygon();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetLife(int nLife);

	// �ÓI�����o�֐�
	static CPolygon * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;			// �ʒu
};
#endif