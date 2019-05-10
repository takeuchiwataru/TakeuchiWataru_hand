//=============================================================================
//
// �����L���O�����N���� [rankingrank.h]
// Author : �|���j
//
//=============================================================================
#ifndef _RANKINGRANK_H_
#define _RANKINGRANK_H_

#include "main.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKINGRANK_TEXTURE "data/TEXTURE/ranking_rank.png"		// �����L���O�����N
#define RANKINGRANK_WIDTH	(80)	// ��
#define RANKINGRANK_HEIGHT	(250)	// ����

//*****************************************************************************
// �����L���O�����N�N���X�̒�`
//*****************************************************************************
class CRankingRank : public CLogo
{
public:
	CRankingRank();
	~CRankingRank();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CRankingRank * Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`���̃|�C���^
};
#endif
