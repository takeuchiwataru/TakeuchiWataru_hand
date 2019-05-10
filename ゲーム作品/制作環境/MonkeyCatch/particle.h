//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.h]
// Author : �|���j
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scene2D.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PARTICLE_TEXTURE_000 "data/TEXTURE/effect000.jpg"		// �G�t�F�N�g�̃e�N�X�`��
#define PARTICLE_TEXTURE_001 "data/TEXTURE/effect000.jpg"		// �G�t�F�N�g�̃e�N�X�`��

#define PARTICLE_SIZE000 (10)			// �G�t�F�N�g�T�C�Y
#define PARTICLE_SIZE001 (5)			// �G�t�F�N�g�T�C�Y
#define PARTICLE_LIFE (10)				// �G�t�F�N�g���C�t
#define PARTICLE_RADIUS (15)			// �G�t�F�N�g�̔��a
#define PARTICLE_GRAVITY (0.03f)		// �G�t�F�N�g�̏d��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CParticle : public CBillBoard
{// �h�������鎞�� : public �`�����ɂ��Ă��
public:
	typedef enum
	{// �p�[�e�B�N���̎��
		PARTICLETYPE_000,		// 
		PARTICLETYPE_001,		// 
		PARTICLETYPE_MAX
	}PARTICLETYPE;

	CParticle();
	~CParticle();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetType(PARTICLETYPE type);


	// �ÓI�����o�֐�
	static CParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int life, int radius, PARTICLETYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[PARTICLETYPE_MAX];	// ���L�e�N�X�`���̃|�C���^
	D3DXVECTOR3				m_move;								// �ړ���
	int						m_nLife;							// �\������
	int					m_nRadius;								// ���a
	D3DXCOLOR				m_col;								// �F
	PARTICLETYPE m_type;										// ���
};
#endif