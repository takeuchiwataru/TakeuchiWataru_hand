//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : �|���j
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MW_TEXTURE ("data/TEXTURE/block100.png")
#define TEX_POS_X_INIT			(1.0f)								// �e�N�X�`�����WU�̏����ʒu
#define TEX_POS_Y_INIT			(1.0f)								// �e�N�X�`�����WV�̏����ʒu
#define TEX_LEFT				(0.0f)								// �e�N�X�`�����WU��
#define TEX_RIGHT				(1.0f)								// �e�N�X�`�����WU�E
#define TEX_TOP					(0.0f)								// �e�N�X�`�����WV��
#define TEX_BOT					(1.0f)								// �e�N�X�`�����WV��
#define X_POSSIZE				(25)								// X�̑傫��
#define Y_POSSIZE				(25)								// Z�̑傫��
#define MAX_MW_X				(32)								// X�̕�
#define MAX_MW_Y			 	(2)									// Y�̕�
#define VERTEX_MW_KAZU ((MAX_MW_X + 1) * (MAX_MW_Y + 1))							// ���_��
#define INDEX_MW_KAZU (((MAX_MW_X + 1) * (MAX_MW_Y + 1) +((MAX_MW_Y - 1) * 2) + (MAX_MW_X + 1) * (MAX_MW_Y - 1)))	// �C���f�b�N�X��
#define MAX_MESHWALL (4)													// ���b�V���I�[���̍ő吔

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CMeshWall : public CScene
{
public:
	CMeshWall();
	~CMeshWall();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	// �ÓI�����o�֐�
	static CMeshWall * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// �v�Z�p�}�g���b�N�X
	float					m_fHeight;		// ����
	float					m_fWidth;		// ��
	int						m_nVtxIndex;	
	int						m_nIdxIndex;
	float					m_fPos_X;
	float					m_fPos_Y;
	float					m_fXpos;
	float					m_fYpos;

	LPD3DXMESH m_pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD m_nNumMat;						// �}�e���A�����̐�

	int m_nNumVertex;						// ���_��
	int m_nNumIndex;						// �C���f�b�N�X��
	int m_nNumPolygon;						// �|���S����
};
#endif
