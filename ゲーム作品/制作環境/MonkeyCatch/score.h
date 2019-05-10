//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCORE (8)				// �ő包��
#define SCORE_INTERVAL_SIZE (55)	// �X�R�A�Ԋu
#define SCORE_SIZE	(30)			// �X�R�A�T�C�Y
#define SCORE_DEGIT	(10)			// ����

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();
	~CScore();
	HRESULT Init(D3DXVECTOR3 pos);
	HRESULT InitTotal(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetScore(void);
	void AddScore(int nValue);
	void SetColor(D3DXCOLOR col);

	// �ÓI�����o�֐�
	static CScore * Create(D3DXVECTOR3 pos);
	static CScore * CreateTotal(D3DXVECTOR3 pos);

private:
	CNumber * m_apNumber[MAX_SCORE];
	int m_nScore;
};
#endif