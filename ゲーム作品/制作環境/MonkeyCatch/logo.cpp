//=============================================================================
//
// ���S���� [logo.cpp]
// Author : �|���j
//
//=============================================================================
#include "logo.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CTitleLogo::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CResultLogo::m_apTexture[RESULTTYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CTutorialLogo::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CRankingLogo::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CPressEnter::m_apTexture[PRESSENTER_MAX] = {};
bool CPressEnter::m_bDisp = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLogo::CLogo(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLogo::~CLogo()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	m_pos = pos;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - LOGO_SIZE_X, m_pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + LOGO_SIZE_X, m_pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - LOGO_SIZE_X, m_pos.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + LOGO_SIZE_X, m_pos.y + LOGO_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �F
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CLogo::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CLogo::Update(void)
{

}
//=============================================================================
// �`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// �ʒu�̐ݒu
//=============================================================================
void CLogo::SetPosition(D3DXVECTOR3 pos, float fwidth, float fheight)
{
	m_pos = pos;

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y - fheight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y - fheight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fwidth, m_pos.y + fheight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fwidth, m_pos.y + fheight, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CLogo::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// �J���[�̐ݒu
//=============================================================================
void CLogo::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_����ݒ�
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���_�o�b�t�@�̎擾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CLogo::GetVtxBuff(void)
{
	return m_pVtxBuff;
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo(): CLogo(3)
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}
//=============================================================================
// �^�C�g�����S����������
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos)
{
	CLogo::Init(pos);

	// �ʒu�̐ݒu
	CLogo::SetPosition(pos, TITLE_SIZE_X, TITLE_SIZE_Y);

	return S_OK;
}
//=============================================================================
// �^�C�g�����S�I������
//=============================================================================
void CTitleLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// �^�C�g�����S�X�V����
//=============================================================================
void CTitleLogo::Update(void)
{

}
//=============================================================================
// �^�C�g�����S�`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// ����
//=============================================================================
CTitleLogo * CTitleLogo::Create(D3DXVECTOR3 pos)
{
	CTitleLogo *pTitleLogo = NULL;

	if (pTitleLogo == NULL)
	{
		pTitleLogo = new CTitleLogo;

		if (pTitleLogo != NULL)
		{
			pTitleLogo->Init(pos);
			pTitleLogo->BindTexture(m_pTexture);
		}
	}
	return pTitleLogo;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CTitleLogo::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLELOGO_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTitleLogo::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResultLogo::CResultLogo() :CLogo(3)
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResultLogo::~CResultLogo()
{

}
//=============================================================================
// ���U���g���S����������
//=============================================================================
HRESULT CResultLogo::Init(D3DXVECTOR3 pos, RESULTTYPE type)
{
	CLogo::Init(pos);

	// �ʒu�̐ݒu
	CLogo::SetPosition(pos, RESULT_SIZE_X, RESULT_SIZE_Y);

	m_nType = type;

	return S_OK;
}
//=============================================================================
// ���U���g���S�I������
//=============================================================================
void CResultLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// ���U���g���S�X�V����
//=============================================================================
void CResultLogo::Update(void)
{

}
//=============================================================================
// ���U���g���S�`�揈��
//=============================================================================
void CResultLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// ����
//=============================================================================
CResultLogo * CResultLogo::Create(D3DXVECTOR3 pos, RESULTTYPE type)
{
	CResultLogo *pResultLogo = NULL;

	if (pResultLogo == NULL)
	{
		pResultLogo = new CResultLogo;

		if (pResultLogo != NULL)
		{
			pResultLogo->Init(pos,type);
			pResultLogo->BindTexture(m_apTexture[type]);
		}
	}
	return pResultLogo;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CResultLogo::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RESULTLOGO_TEXTURE, &m_apTexture[0]);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CResultLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < RESULTTYPE_MAX; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorialLogo::CTutorialLogo() :CLogo(3)
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorialLogo::~CTutorialLogo()
{

}
//=============================================================================
// �`���[�g���A�����S����������
//=============================================================================
HRESULT CTutorialLogo::Init(D3DXVECTOR3 pos)
{
	CLogo::Init(pos);

	// �ʒu�̐ݒu
	CLogo::SetPosition(pos, TUTORIAL_SIZE_X, TUTORIAL_SIZE_Y);

	return S_OK;
}
//=============================================================================
// �`���[�g���A�����S�I������
//=============================================================================
void CTutorialLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// �`���[�g���A�����S�X�V����
//=============================================================================
void CTutorialLogo::Update(void)
{

}
//=============================================================================
// �`���[�g���A�����S�`�揈��
//=============================================================================
void CTutorialLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// ����
//=============================================================================
CTutorialLogo * CTutorialLogo::Create(D3DXVECTOR3 pos)
{
	CTutorialLogo *pTutorial = NULL;

	if (pTutorial == NULL)
	{
		pTutorial = new CTutorialLogo;

		if (pTutorial != NULL)
		{
			pTutorial->Init(pos);
			pTutorial->BindTexture(m_pTexture);
		}
	}
	return pTutorial;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CTutorialLogo::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TUTORIALLOGO_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTutorialLogo::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRankingLogo::CRankingLogo() :CLogo(3)
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRankingLogo::~CRankingLogo()
{

}
//=============================================================================
// �����L���O���S����������
//=============================================================================
HRESULT CRankingLogo::Init(D3DXVECTOR3 pos)
{
	CLogo::Init(pos);

	// �ʒu�̐ݒu
	CLogo::SetPosition(pos, RANKING_SIZE_X, RANKING_SIZE_Y);

	return S_OK;
}
//=============================================================================
// �����L���O���S�I������
//=============================================================================
void CRankingLogo::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// �����L���O���S�X�V����
//=============================================================================
void CRankingLogo::Update(void)
{

}
//=============================================================================
// �����L���O���S�`�揈��
//=============================================================================
void CRankingLogo::Draw(void)
{
	CLogo::Draw();
}
//=============================================================================
// ����
//=============================================================================
CRankingLogo * CRankingLogo::Create(D3DXVECTOR3 pos)
{
	CRankingLogo *pRanking = NULL;

	if (pRanking == NULL)
	{
		pRanking = new CRankingLogo;

		if (pRanking != NULL)
		{
			pRanking->Init(pos);
			pRanking->BindTexture(m_pTexture);
		}
	}
	return pRanking;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CRankingLogo::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKINGLOGO_TEXTURE, &m_pTexture);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CRankingLogo::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPressEnter::CPressEnter() : CLogo(3)
{
	m_PressEnterState = PRESSENTERSTATE_NONE;  // ������Ă��Ȃ���Ԃɂ���
	m_nCounterPressEnter = 0;                  // �J�E���^�[��0�ɂ���
	m_bDisp = false;                           // �\���͂��Ȃ�
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPressEnter::~CPressEnter()
{

}
//=============================================================================
// �v���X�G���^�[����������
//=============================================================================
HRESULT CPressEnter::Init(D3DXVECTOR3 pos, PRESSENTER type)
{
	CLogo::Init(pos);

	m_PressEnterState = PRESSENTERSTATE_NONE;  // ������Ă��Ȃ���Ԃɂ���
	m_nCounterPressEnter = 0;                  // �J�E���^�[��0�ɂ���
	m_bDisp = false;                           // �\���͂��Ȃ�

	m_nType = type;

	// �ʒu�̐ݒu
	if (m_nType == PRESSENTER_TITLE)
	{// �^�C�g��
		CLogo::SetPosition(pos, TITLE_PRESSENTER_SIZE_X, PRESSENTER_SIZE_Y);
	}
	else if(m_nType == PRESSENTER_TUTORIAL)
	{// �`���[�g���A��
		CLogo::SetPosition(pos, TUTORIAL_PRESSENTER_SIZE_X, PRESSENTER_SIZE_Y);
	}

	return S_OK;
}
//=============================================================================
// �v���X�G���^�[�I������
//=============================================================================
void CPressEnter::Uninit(void)
{
	CLogo::Uninit();
}
//=============================================================================
// �v���X�G���^�[�X�V����
//=============================================================================
void CPressEnter::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �W���C�p�b�h�̎擾
	CInputJoyPad *pInputJoyPad = CManager::GetJoyPad();

	// ���[�h�̎擾
	CManager::MODE mode;
	mode = CManager::GetMode();

	// �t�F�[�h�̎擾
	CFade *pFade = CManager::GetFade();

	// �T�E���h�̎擾
	CSound * pSound = CManager::GetSound();

	if (m_nCounterPressEnter >= PRESSENTER_COUNTER)
	{
		m_nCounterPressEnter = 0;
	}

	if (mode == CManager::MODE_TITLE)
	{// �^�C�g����������
		if (m_bDisp == true)
		{// �`�悷���ԂȂ��
			m_nCounterPressEnter++;
			if (m_PressEnterState != PRESSENTERSTATE_PUSH)
			{// �����ꂽ��ԈȊO��������
				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_1) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_2) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_3) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_4) == true)
				{// �Ώۂ̃{�^���������ꂽ��
					m_PressEnterState = PRESSENTERSTATE_PUSH;	// ������Ă����Ԃɂ���
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENETR);
				}
			}
		}
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{// �`���[�g���A����������
		if (m_bDisp == true)
		{// �`�悷���ԂȂ��
			m_nCounterPressEnter++;
			if (m_PressEnterState != PRESSENTERSTATE_PUSH)
			{// �����ꂽ��ԈȊO��������
				if (pInputKeyboard->GetTrigger(DIK_1) == true ||
					pInputJoyPad->GetTrigger(CInputJoyPad::DIJS_BUTTON_12) == true)
				{// 1 or START�����ꂽ
					m_PressEnterState = PRESSENTERSTATE_PUSH;	// ������Ă����Ԃɂ���
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENETR);
				}
			}
		}
	}
}
//=============================================================================
// �v���X�G���^�[�`�揈��
//=============================================================================
void CPressEnter::Draw(void)
{
	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = CLogo::GetVtxBuff();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_apTexture[m_nType]);

	// �|���S���̕`��
	if (m_bDisp == true)
	{// �`�悷���Ԃ�������
		if (m_PressEnterState == PRESSENTERSTATE_NONE)
		{// ������Ă��Ȃ���Ԃ�������
			if (m_nCounterPressEnter / PRESSENTER_NONE == 0)
			{// �J�E���^�[�����̒l�ɂȂ�����
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
		else if (m_PressEnterState == PRESSENTERSTATE_PUSH)
		{// ������Ă����Ԃ�������
			if (m_nCounterPressEnter % PRESSENTER_PUSH == 0)
			{// �J�E���^�[�����̒l�ɂȂ�����
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
	}
}
//=============================================================================
// ����
//=============================================================================
CPressEnter * CPressEnter::Create(D3DXVECTOR3 pos, PRESSENTER type)
{
	CPressEnter *pPressEnter = NULL;

	if (pPressEnter == NULL)
	{
		pPressEnter = new CPressEnter;

		if (pPressEnter != NULL)
		{
			pPressEnter->Init(pos,type);
			pPressEnter->BindTexture(m_apTexture[type]);
		}
	}
	return pPressEnter;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CPressEnter::Load(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PRESSENTER_TEXTURE, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PRESSENTER_TEXTURE002, &m_apTexture[1]);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CPressEnter::Unload(void)
{
	for (int nCntTex = 0; nCntTex < PRESSENTERSTATE_MAX; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}
//=============================================================================
// �v���X�G���^�[���S��`�悷�邩���Ȃ���
//=============================================================================
void CPressEnter::DispPressEnter(void)
{
	m_bDisp = true;
}
