//=============================================================================
//
// ���b�V���I�[�r�b�g���� [meshorbit.h]
// Author : �|���j
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MO_X (25)															// X(���̐�)
#define MAX_MO_Z (1)															// Z(�c�̐�)
#define MAX_VERTEX	((MAX_MO_X + 1) * (MAX_MO_Z + 1))							// �ő咸�_��
#define MAX_POLYGON (((MAX_MO_X * MAX_MO_Z) * 2) + ((MAX_MO_Z - 1) * 4))		// �ő�|���S����
#define MAX_OFFSET	(2)															// ���[�̃I�t�Z�b�g��
#define ORBIT_COLOR	(0.05f)

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CMeshOrbit : public CScene
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	CMeshOrbit();
	~CMeshOrbit();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetMtxParent(D3DXMATRIX *pMtx);
	D3DXMATRIX GetMtxWorld(void);

	// �ÓI�����o�֐�
	static CMeshOrbit * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX				*m_pMtxParent;	// �e�̃}�g���b�N�X�ւ̃|�C���^

	D3DXMATRIX m_mtxRot, m_mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 m_aPosVertex[MAX_VERTEX];	// �Z�o�������_���W
	D3DXVECTOR3 m_aOffset[MAX_OFFSET];		// ���[�̃I�t�Z�b�g
	int m_nNumPos;
	int m_nNumPolygon;
	bool m_bUse;
	float m_fCol;

	static CPlayer *m_pPlayer;
};
#endif