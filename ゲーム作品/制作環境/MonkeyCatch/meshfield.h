//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : �|���j
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHFILED_POS	(2500.0f)											// �|���S���̃T�C�Y
#define MAX_MF_X (250)														// X(���̐�)
#define MAX_MF_Z (250)														// Z(�c�̐�)
#define VERTEX_KAZU ((MAX_MF_X + 1) * (MAX_MF_Z + 1))							// ���_��
#define INDEX_KAZU ((((MAX_MF_X + 1) * 2) * (MAX_MF_Z)) + ((MAX_MF_Z - 1) * 2))	// �C���f�b�N�X��
#define POLYGON_KAZU (((MAX_MF_X * MAX_MF_Z) * 2) + ((MAX_MF_Z - 1) * 4))		// �|���S����// �|���S����
#define MF_TEXTURE ("data/TEXTURE/field001.jpg")

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CMeshField : public CScene
{
public:
	CMeshField();
	~CMeshField();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	float GetHeight(D3DXVECTOR3 pos);
	void Nor(void);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);
	void SaveMeshFiled(void);
	void LoadMeshFiled(void);

	// �ÓI�����o�֐�
	static CMeshField * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXVECTOR3				m_move;			// �ړ�
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;		// �v�Z�p�}�g���b�N�X

	LPD3DXMESH m_pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD m_nNumMat;						// �}�e���A�����̐�

	int m_nNumVertex;				// ���_��
	int m_nNumIndex;				// �C���f�b�N�X��
	int m_nNumPolygon;				// �|���S����

	D3DXVECTOR3 m_vecA;
	D3DXVECTOR3 m_vecB;
	D3DXVECTOR3 m_vecC;

	D3DXVECTOR3 m_aPos[9];
};
#endif
