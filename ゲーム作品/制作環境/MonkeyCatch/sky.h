//=============================================================================
//
// �󏈗� [sky.h]
// Author : �|���j
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SKY_TEXTURE_NAME   "data/TEXTURE/sky000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define DOME_LENGTH         (100)						// �c�̒���
#define DOME_SIDE           (50)						// ���̒���
#define DOME_RADIUS         (1200.0f)					// ���a�̒���
#define DOME_HEIGHT         (2000.0f)					// �����̒���
#define BLOCK_LENGTH        (5)							// �c�̕�����
#define BLOCK_SIDE          (10)						// ���̕�����
#define SKY_ROT				(0.00002f)					// ��]��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CSky : public CScene
{
public:
	CSky();
	~CSky();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	// �ÓI�����o�֐�
	static CSky * Create(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_apVtxBuff[2];	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�̃|�C���^

	D3DXVECTOR3				m_pos;			// �ʒu
	D3DXVECTOR3				m_rot;			// ����
	D3DXMATRIX				m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX		m_mtxRot, m_mtxTrans;	// �v�Z�p�}�g���b�N�X
	float					m_Length;		// �c�̒���
	float					m_Side;			// ���̒���
	int						m_XBlock;		// ���̕�����
	int						m_YBlock;		// �c�̕�����
	int						m_nNumVertex;	// ���_��
	int						m_nNumIndex;	// �C���f�b�N�X��
	int						m_nNumPolygon;	// �|���S����
	float					m_Addtex;		//
};
#endif
