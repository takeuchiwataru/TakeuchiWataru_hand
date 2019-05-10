//=============================================================================
//
// �G���� [enemy.h]
// Author : �|���j
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
#define ENEMY_MOVE_SPEED	(2.0f)
#define MAX_ENEMY_MODEL	(10)			// �G�̃p�[�c��
#define NUM_MAX_ENEMY_KEY	(2)			// �L�[�ő吔
#define NUM_MAX_ENEMY_MOTION	(3)		// ���[�V�����ő吔
#define ENEMY_MAX_POS	(1490)			// �X�e�[�W�ő�
#define ENEMY_MIN_POS	(0)				// �X�e�[�W�ŏ�
#define NUM_ENEMY	(10)				// �G�̑���
#define ENEMY_GRAVITY	(0.5f)			// �d��
#define ENEMY_REMAIN	(1)				// �c�@
#define ENEMY_SCORE		(1000)			// �X�R�A
#define ENEMY_MOVESTAND	(0.2f)			// �ړ��
#define ENEMY_MOVE000	(0.05f)			// �ړ�
#define ENEMY_MOVE001	(0.04f)			// �ړ�
#define ENEMY_COUNTER000	(60)		// �G�J�E���^�[
#define ENEMY_COUNTER001	(120)		// �G�J�E���^�[
#define ENEMY_DIFF	(0.1f)				// �p�x�̍���
#define ENEMY_AROUND	(2.0f)			// �p�x�̎���
#define ENEMY_VECLIGHT	(D3DXVECTOR4(-0.22f, 0.4f, -0.2f, 0.0f))	// �e�p�̃��C�g
#define ENEMY_POS	(D3DXVECTOR3(0.0f, 0.8f, 0.0f))					// �e�p�̈ʒu
#define ENEMY_NORMAL	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))				// �e�p�̖@��

#define E_MAX_PARTICLE	(20)			// �p�[�e�B�N����
#define E_PARTICLE_LIFE	(30)			// �p�[�e�B�N�����C�t
#define E_PARTICLE_RADIUS	(30)		// �p�[�e�B�N���̑傫��
#define E_PARTICLE_CREATEPOS	(D3DXVECTOR3(m_pos.x, m_pos.y + 40, m_pos.z))	// �p�[�e�B�N�������ʒu
#define E_PARTICLE_AROUND	(rand() % 628 / 100.0f)								// �p�[�e�B�N������
#define E_PARTICLE_RAND	(rand() % 7 + 1)										// �����_���ɏo���p�[�e�B�N��
#define ENEMY_COLLISION	(2000.0f)		// �G�̓����蔻��̔��a

#define ENEMY_FILE ("data/TEXT/motion_enemy.txt")
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
class CEnemy : public CScene
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
		E_KEY aKey[MAX_ENEMY_MODEL];
	}KEY_INFO;

	typedef struct
	{// ���[�V�������
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[NUM_MAX_ENEMY_KEY];
	}MOTION_INFO;

	typedef enum
	{// �G�̏��
		ENEMYSTATE_NONE = 0,
		ENEMYSTATE_NORMAL,		// �ʏ�
		ENEMYSTATE_MOVE,		// �ړ�
		ENEMYSTATE_MAX
	}ENEMYSTATE;

	typedef enum
	{// �G�̎��
		ENEMYTYPE_000,			// �����Ȃ�
		ENEMYTYPE_001,			// �����_��
		ENEMYTYPE_002,			// ��
		ENEMYTYPE_003,			// �c
		ENEMYTYPE_004,			// �� ver2
		ENEMYTYPE_005,			// �c ver2
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	CEnemy();
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void UpdateTutorial(void);
	void Draw(void);
	void LoadMotion(void);
	void UpdateMotion(void);
	char *ReadLine(FILE * pFile, char *pDst);
	char *GetLineTop(char *pSrc);
	int PopString(char * pSrc, char * pDst);
	void SetAngle(void);
	void MapCollision(void);
	void CollisionAll(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin);
	void SetType(ENEMYTYPE type);

	// �ÓI�����o�֐�
	static CEnemy * Create(D3DXVECTOR3 pos, ENEMYTYPE type);
	static int GetNumEnemy(void);
	static int GetCatchEnemy(void);

	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`���̃|�C���^
	static LPD3DXMESH		m_apMesh[MAX_ENEMY_MODEL];				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER		m_apBuffMat[MAX_ENEMY_MODEL];			// �}�e���A�����ւ̃|�C���^
	static DWORD			m_anNumMat[MAX_ENEMY_MODEL];			// �}�e���A�����̐�
	static int m_nNumEnemy;											// �G�̑���
	static int m_nGetEnemy;											// �Q�b�g�����G�̐�
	static CMeshOrbit * m_pMeshOrbit;
	static CObject * m_pObject;
	static CPlayer *m_pPlayer;

private:
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posold;						// �ʒu
	D3DXVECTOR3 m_move;							// �ړ�
	D3DXVECTOR3 m_rot;							// ����
	ENEMYSTATE m_state;							// ���
	D3DXVECTOR3	m_aPosOffset[MAX_ENEMY_MODEL];	// ���f���̃I�t�Z�b�g
	D3DXMATRIX	m_mtxWorld;						// ���[���h�}�g���b�N�X
	bool m_bJump;								// �W�����v���邩���Ȃ���
	bool m_bDisp;								// �\�����邩�ǂ���
	float m_fDest;								// �ړI�̊p�x
	float m_fDiff;								// �p�x�̍���

	CModel * m_apModel[MAX_ENEMY_MODEL];
	D3DXVECTOR3 m_VtxMin, m_VtxMax;				// �ŏ��l�A�ő�l

	KEY_INFO *m_pKeyInfo;					// �L�[���̃|�C���^
	int m_nKey;								// ���݂̃L�[No.
	int m_nCouterMotion;					// ���[�V�����J�E���^�[
	int m_StateSave;						// ��ԕۑ��p
	int m_nNumParts;						// �p�[�c��
	int m_aIndexParent[MAX_ENEMY_MODEL];	// �C���f�b�N�X
	E_KEY m_aKeyOffset[MAX_ENEMY_MODEL];
	MOTION_INFO m_aMotionInfo[NUM_MAX_ENEMY_MOTION];
	char m_aFilenameModel[MAX_ENEMY_MODEL][MAX_CHAR];

	bool m_bUse;
	bool m_bHit;							// �����������ǂ���
	bool m_bHitGrass;						// ���ɓ����������ǂ���
	bool m_bHitObject;						// �I�u�W�F�N�g�ɓ����������ǂ���
	ENEMYTYPE m_type;						// �G�̎��
	int m_MoveCounter;						// �ړ��J�E���^�[
};
#endif