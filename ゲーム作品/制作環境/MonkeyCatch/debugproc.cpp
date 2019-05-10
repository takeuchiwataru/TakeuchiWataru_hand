//=============================================================================
//
// �f�o�b�O�\������ [debugproc.cpp]
// Author : �|���j
//
//=============================================================================
#include "debugproc.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[MAX_STR] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{

}
//=============================================================================
// ����������
//=============================================================================
void CDebugProc::Init(void)
{
	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

}
//=============================================================================
// �I������
//=============================================================================
void CDebugProc::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif
}
//=============================================================================
// �f�o�b�O�\������
//=============================================================================
void CDebugProc::Print(int nNumber, char * fmt, ...)
{
	char cStr[MAX_STR];
	cStr[0] = '\0';
	va_list ap;

	va_start(ap, fmt);
	vsprintf(cStr, fmt, ap);

	if (nNumber == 0)
	{
		wsprintf(&m_aStr[0], &cStr[0]);
	}
	else if (nNumber == 1)
	{
		strcat(&m_aStr[0], &cStr[0]);
	}
	va_end(ap);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}