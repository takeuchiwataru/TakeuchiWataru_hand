//=============================================================================
//
// ���͏��� [input.cpp]
// Author : �|���j
//
//=============================================================================
#include "input.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CInput::Update(void)
{

}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// �L�[���̃N���A
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0x00;
		m_aKeyStateTrigger[nCntKey] = 0x00;
	}
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// �L�[�{�[�h���͏��ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}
//=============================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputJoyPad::CInputJoyPad()
{
	// �L�[���̃N���A
	for (int nCntKey = 0; nCntKey < DIJS_BUTTON_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0x00;
		m_aKeyStateTrigger[nCntKey] = 0x00;
		m_aKeyStateRelese[nCntKey] = 0x00;
	}
	m_LeftAxizX = 0.0f;
	m_LeftAxizY = 0.0f;
	m_RightAxizX = 0.0f;
	m_RightAxizY = 0.0f;
	m_Radian = 0.0f;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputJoyPad::~CInputJoyPad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputJoyPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (m_pDevice != NULL)
	{// �����ł���
		DIPROPRANGE diprg;

		// ���̒l�͈̔͂�ݒ�
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// ���̐ݒ�
		// ���A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �\���L�[(���0�x�Ƃ����v���Ɋp�x * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �W���C�X�e�B�b�N�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_pDevice->Acquire();
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CInputJoyPad::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CInputJoyPad::Update(void)
{
	DIJOYSTATE dijs;

	int nCntKey;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs)))
		{
			m_LeftAxizX = (float)dijs.lX;
			m_LeftAxizY = (float)dijs.lY;
			m_RightAxizX = (float)dijs.lZ;
			m_RightAxizY = (float)dijs.lRz;

			//-----------------------------	
			// ���A�i���O�X�e�B�b�N
			//-----------------------------
			if (m_LeftAxizY <= -100)
			{// ��ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}
			if (m_LeftAxizY >= 100)
			{// ���ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_23] = 0x80;
			}
			if (m_LeftAxizX <= -100)
			{// ���ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (m_LeftAxizX >= 100)
			{// �E�ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_22] = 0x80;
			}

			//-----------------------------	
			// �E�A�i���O�X�e�B�b�N
			//-----------------------------
			if (m_RightAxizY <= -100)
			{// ��ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_25] = 0x80;
			}
			if (m_RightAxizY >= 100)
			{// ���ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_27] = 0x80;
			}
			if (m_RightAxizX <= -100)
			{// ���ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_24] = 0x80;
			}
			if (m_RightAxizX >= 100)
			{// �E�ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_26] = 0x80;
			}

			// �\���L�[
			if (dijs.rgdwPOV[0] / 100 == 0)
			{// ��
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 45)
			{// �E��
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 90)
			{// �E
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 135)
			{// �E��
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 180)
			{// ��
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 225)
			{// ����
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 270)
			{// ��
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 315)
			{// ����
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			for (nCntKey = 0; nCntKey < DIJS_BUTTON_MAX; nCntKey++)
			{
				// �W���C�p�b�h���͏��ۑ�(�g���K�[)
				m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ dijs.rgbButtons[nCntKey]) & dijs.rgbButtons[nCntKey];

				// �W���C�p�b�h���͏��ۑ�(�����[�X)
				m_aKeyStateRelese[nCntKey] = (m_aKeyState[nCntKey] ^ dijs.rgbButtons[nCntKey]) & dijs.rgbButtons[nCntKey];

				// �W���C�p�b�h���͏��ۑ�(�v���X)
				m_aKeyState[nCntKey] = dijs.rgbButtons[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
		}
	}
}
//=============================================================================
// �W���C�p�b�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputJoyPad::GetPress(int nButton)
{
	return(m_aKeyState[nButton] & 0x80) ? true : false;
}
//=============================================================================
// �W���C�p�b�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputJoyPad::GetTrigger(int nButton)
{
	return(m_aKeyStateTrigger[nButton] & 0x80) ? true : false;
}
//=============================================================================
// �W���C�p�b�h�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CInputJoyPad::GetRelese(int nButton)
{
	return(m_aKeyStateRelese[nButton] & 0x80) ? true : false;
}
//=============================================================================
// ���X�e�B�b�N
//=============================================================================
float CInputJoyPad::GetLeftAxiz(void)
{
	float Axiz = atan2f(m_LeftAxizX, m_LeftAxizY);
	return Axiz;
}
//=============================================================================
// �E�X�e�B�b�N
//=============================================================================
float CInputJoyPad::GetRightAxiz(void)
{
	float Axiz = atan2f(m_RightAxizX, m_RightAxizY);
	return Axiz;
}
//=============================================================================
// �\���L�[
//=============================================================================
float CInputJoyPad::GetRadian(void)
{
	float Radian = m_Radian / 100;
	return Radian;
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXInputJoyPad::CXInputJoyPad()
{

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CXInputJoyPad::~CXInputJoyPad()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CXInputJoyPad::Init()
{
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}
//=============================================================================
// �X�V����
//=============================================================================
void CXInputJoyPad::Update(void)
{
	// 
	UpdateControllerState();

	WORD xijs;

	for (DWORD dCnt = 0; dCnt < MAX_CONTROLLERS; dCnt++)
	{
		if ((m_Controllers[dCnt].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
			m_Controllers[dCnt].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
			(m_Controllers[dCnt].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
				m_Controllers[dCnt].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
		{
			m_Controllers[dCnt].state.Gamepad.sThumbLX = 0;
			m_Controllers[dCnt].state.Gamepad.sThumbLY = 0;
		}

		if ((m_Controllers[dCnt].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
			m_Controllers[dCnt].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
			(m_Controllers[dCnt].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
				m_Controllers[dCnt].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
		{
			m_Controllers[dCnt].state.Gamepad.sThumbRX = 0;
			m_Controllers[dCnt].state.Gamepad.sThumbRY = 0;
		}

		m_LeftAxizX[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbLX;
		m_LeftAxizY[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbLY;
		m_RightAxizX[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbRX;
		m_RightAxizY[dCnt] = (float)m_Controllers[dCnt].state.Gamepad.sThumbRY;

		// ��񎝂��Ă�
		xijs = m_Controllers[dCnt].state.Gamepad.wButtons;

		// �W���C�p�b�h���͏��ۑ�(�g���K�[)
		m_aKeyStateTrigger[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// �W���C�p�b�h���͏��ۑ�(�����[�X)
		m_aKeyStateRelese[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// �W���C�p�b�h���͏��ۑ�(�v���X)
		m_aKeyState[dCnt] = xijs;
	}
}
//=============================================================================
// 
//=============================================================================
HRESULT CXInputJoyPad::UpdateControllerState(void)
{
	DWORD dwResult;
	for (DWORD dCnt = 0; dCnt < MAX_CONTROLLERS; dCnt++)
	{
		dwResult = XInputGetState(dCnt, &m_Controllers[dCnt].state);
		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[dCnt].bConnected = true;
		}
		else
		{
			m_Controllers[dCnt].bConnected = false;
		}
	}
	return S_OK;
}
//=============================================================================
// XInput�̓��͏��(�v���X���)���擾
//=============================================================================
bool CXInputJoyPad::GetPress(int nButton, int indexpad)
{
	return(m_aKeyState[indexpad] & nButton) ? true : false;
}
//=============================================================================
// XInput�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CXInputJoyPad::GetTrigger(int nButton, int indexpad)
{
	return(m_aKeyStateTrigger[indexpad] & nButton) ? true : false;
}
//=============================================================================
// �W���C�p�b�h�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CXInputJoyPad::GetRelese(int nButton, int indexpad)
{
	return(m_aKeyStateRelese[indexpad] & nButton) ? true : false;
}
//=============================================================================
// ���X�e�B�b�N
//=============================================================================
float CXInputJoyPad::GetLeftAxiz(int indexpad)
{
	float Axiz = atan2f(m_LeftAxizX[indexpad], m_LeftAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// �E�X�e�B�b�N
//=============================================================================
float CXInputJoyPad::GetRightAxiz(int indexpad)
{
	float Axiz = atan2f(m_RightAxizX[indexpad], m_RightAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// �X�e�B�b�N�̎擾
//=============================================================================
bool CXInputJoyPad::GetStick(int nLR, int indexpad)
{
	if (m_Controllers[indexpad].bConnected == true)
	{
		if (nLR == 0)
		{
			if (m_Controllers[indexpad].state.Gamepad.sThumbLX < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbLX > XINPUT_STICK_MAX * 0.1f ||
				m_Controllers[indexpad].state.Gamepad.sThumbLY < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbLY > XINPUT_STICK_MAX * 0.1f)
			{
				return true;
			}
		}
		else if (nLR == 1)
		{
			if (m_Controllers[indexpad].state.Gamepad.sThumbRX < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbRX > XINPUT_STICK_MAX * 0.1f ||
				m_Controllers[indexpad].state.Gamepad.sThumbRY < XINPUT_STICK_MIN * 0.1f || m_Controllers[indexpad].state.Gamepad.sThumbRY > XINPUT_STICK_MAX * 0.1f)
			{
				return true;
			}
		}
	}
	return false;
}
