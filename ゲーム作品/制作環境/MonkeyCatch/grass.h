//=============================================================================
//
// ������ [grass.h]
// Author : �|���j
//
//=============================================================================
#ifndef _GRASS_H_
#define _GRASS_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CObject;
class CPlayer;
class CRemain;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_GRASS_MODEL	(9)				// ���̃p�[�c��
#define NUM_MAX_GRASS_KEY	(2)			// �L�[�ő吔
#define NUM_MAX_GRASS_MOTION	(1)		// ���[�V�����ő吔
#define GRASS_MOVESTAND	(0.2f)			// �ړ��
#define G_MAX_PARTICLE (40)				// �p�[�e�B�N����
#define G_PARTICLE_LIFE (30)			// �p�[�e�B�N�����C�t
#define G_PARTICLE_RADIUS (30)			// �p�[�e�B�N���̑傫��
#define G_PARTICLE_CREATEPOS	(D3DXVECTOR3(m_pos.x, m_pos.y + 40, m_pos.z))	// �p�[�e�B�N�������ʒu
#define G_PARTICLE_AROUND	(rand() % 628 / 100.0f)								// �p�[�e�B�N������
#define G_PARTICLE_RAND	(rand() % 7 + 1)										// �����_���ɏo���p�[�e�B�N��
#define GRASS_COLLISION (3500.0f)		// ���̓����蔻��̔��a

#define GRASS_FILE ("data/TEXT/motion_grass.txt")
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME   "MODEL_FILENAME = "
#define CHARACTERSET "CHARACTERSET"
#define END_CHARACTERSET "END_CHARACTERSET"
#define NUM_PARTS "NUM_PARTS = "
#define PARTSSET "PARTSSET"
#define END_PARTSSET "END_PARTSSET"
#define INDEX "INDEX = "
#define PARENT "PARENT = "
#define POS "POS = "
#define ROT "ROT = "

#define MOTIONSET "MOTIONSET"
#define END_MOTIONSET "END_MOTIONSET"
#define LOOP "LOOP = "
#define NUM_KEY "NUM_KEY = "
#define KEYSET "KEYSET"
#define END_KEYSET "END_KEYSET"
#define FRAME "FRAME = "
#define KEY "KEY"
#define END_KEY "END_KEY"

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CGrass : public CScene
{
public:
	typedef struct
	{// �L�[�ݒ�
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	}E_KEY;

	typedef struct
	{// �L�[���
		int nFrame;
		E_KEY aKey[MAX_GRASS_MODEL];
	}KEY_INFO;

	typedef struct
	{// ���[�V�������
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[NUM_MAX_GRASS_KEY];
	}MOTION_INFO;

	typedef enum
	{// ���̏��
		GRASSSTATE_NONE = 0,
		GRASSSTATE_NORMAL,		// �ʏ�
		GRASSSTATE_MOVE,		// �ړ�
		GRASSSTATE_MAX
	}GRASSSTATE;

	typedef enum
	{// ���̎��
		GRASSTYPE_000,			// ���Ȃ�
		GRASSTYPE_001,			// ����
		GRASSTYPE_MAX
	}GRASSTYPE;

	CGrass();
	~CGrass();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadMotion(void);
	void UpdateMotion(void);
	char *ReadLine(FILE * pFile, char *pDst);
	char *GetLineTop(char *pSrc);
	int PopString(char * pSrc, char * pDst);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin);
	void SetType(GRASSTYPE type);

	// �ÓI�����o�֐�
	static CGrass * Create(D3DXVECTOR3 pos, GRASSTYPE type);

	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`���̃|�C���^
	static LPD3DXMESH		m_apMesh[MAX_GRASS_MODEL];				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER		m_apBuffMat[MAX_GRASS_MODEL];			// �}�e���A�����ւ̃|�C���^
	static DWORD			m_anNumMat[MAX_GRASS_MODEL];			// �}�e���A�����̐�
	static CMeshOrbit * m_pMeshOrbit;
	static CObject * m_pObject;
	static CPlayer *m_pPlayer;

private:
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posold;						// �ʒu
	D3DXVECTOR3 m_move;							// �ړ�
	D3DXVECTOR3 m_rot;							// ����
	GRASSSTATE m_state;							// ���
	D3DXVECTOR3	m_aPosOffset[MAX_GRASS_MODEL];	// ���f���̃I�t�Z�b�g
	D3DXMATRIX	m_mtxWorld;						// ���[���h�}�g���b�N�X
	bool m_bJump;								// �W�����v���邩���Ȃ���
	bool m_bDisp;								// �\�����邩�ǂ���
	float m_fDest;								// �ړI�̊p�x
	float m_fDiff;								// �p�x�̍���

	CModel * m_apModel[MAX_GRASS_MODEL];
	D3DXVECTOR3 m_VtxMin, m_VtxMax;				// �ŏ��l�A�ő�l

	KEY_INFO *m_pKeyInfo;					// �L�[���̃|�C���^
	int m_nKey;								// ���݂̃L�[No.
	int m_nCouterMotion;					// ���[�V�����J�E���^�[
	int m_StateSave;						// ��ԕۑ��p
	int m_nNumParts;						// �p�[�c��
	int m_aIndexParent[MAX_GRASS_MODEL];	// �C���f�b�N�X
	E_KEY m_aKeyOffset[MAX_GRASS_MODEL];
	MOTION_INFO m_aMotionInfo[NUM_MAX_GRASS_MOTION];
	char m_aFilenameModel[MAX_GRASS_MODEL][256];

	bool m_bHit;		// �����������ǂ���
	bool m_bUse;
	GRASSTYPE m_type;						// �G�̎��

};
#endif