//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 竹内亘
//
//=============================================================================
#include "input.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}
//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CInput::Update(void)
{

}
//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// キー情報のクリア
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0x00;
		m_aKeyStateTrigger[nCntKey] = 0x00;
	}
}
//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボード入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
	}
}
//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
// コンストラクタ
//=============================================================================
CInputJoyPad::CInputJoyPad()
{
	// キー情報のクリア
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
// デストラクタ
//=============================================================================
CInputJoyPad::~CInputJoyPad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputJoyPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (m_pDevice != NULL)
	{// 生成できた
		DIPROPRANGE diprg;

		// 軸の値の範囲を設定
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// 軸の設定
		// 左アナログスティック
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイスティックへのアクセス権を獲得(入力制御開始)
		m_pDevice->Acquire();
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CInputJoyPad::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CInputJoyPad::Update(void)
{
	DIJOYSTATE dijs;

	int nCntKey;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs)))
		{
			m_LeftAxizX = (float)dijs.lX;
			m_LeftAxizY = (float)dijs.lY;
			m_RightAxizX = (float)dijs.lZ;
			m_RightAxizY = (float)dijs.lRz;

			//-----------------------------	
			// 左アナログスティック
			//-----------------------------
			if (m_LeftAxizY <= -100)
			{// 上に倒された
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}
			if (m_LeftAxizY >= 100)
			{// 下に倒された
				dijs.rgbButtons[DIJS_BUTTON_23] = 0x80;
			}
			if (m_LeftAxizX <= -100)
			{// 左に倒された
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (m_LeftAxizX >= 100)
			{// 右に倒された
				dijs.rgbButtons[DIJS_BUTTON_22] = 0x80;
			}

			//-----------------------------	
			// 右アナログスティック
			//-----------------------------
			if (m_RightAxizY <= -100)
			{// 上に倒された
				dijs.rgbButtons[DIJS_BUTTON_25] = 0x80;
			}
			if (m_RightAxizY >= 100)
			{// 下に倒された
				dijs.rgbButtons[DIJS_BUTTON_27] = 0x80;
			}
			if (m_RightAxizX <= -100)
			{// 左に倒された
				dijs.rgbButtons[DIJS_BUTTON_24] = 0x80;
			}
			if (m_RightAxizX >= 100)
			{// 右に倒された
				dijs.rgbButtons[DIJS_BUTTON_26] = 0x80;
			}

			// 十字キー
			if (dijs.rgdwPOV[0] / 100 == 0)
			{// 上
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 45)
			{// 右上
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 90)
			{// 右
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 135)
			{// 右下
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 180)
			{// 下
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 225)
			{// 左下
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 270)
			{// 左
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 315)
			{// 左上
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			for (nCntKey = 0; nCntKey < DIJS_BUTTON_MAX; nCntKey++)
			{
				// ジョイパッド入力情報保存(トリガー)
				m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ dijs.rgbButtons[nCntKey]) & dijs.rgbButtons[nCntKey];

				// ジョイパッド入力情報保存(リリース)
				m_aKeyStateRelese[nCntKey] = (m_aKeyState[nCntKey] ^ dijs.rgbButtons[nCntKey]) & dijs.rgbButtons[nCntKey];

				// ジョイパッド入力情報保存(プレス)
				m_aKeyState[nCntKey] = dijs.rgbButtons[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
		}
	}
}
//=============================================================================
// ジョイパッドの入力情報(プレス情報)を取得
//=============================================================================
bool CInputJoyPad::GetPress(int nButton)
{
	return(m_aKeyState[nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputJoyPad::GetTrigger(int nButton)
{
	return(m_aKeyStateTrigger[nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(リリース情報)を取得
//=============================================================================
bool CInputJoyPad::GetRelese(int nButton)
{
	return(m_aKeyStateRelese[nButton] & 0x80) ? true : false;
}
//=============================================================================
// 左スティック
//=============================================================================
float CInputJoyPad::GetLeftAxiz(void)
{
	float Axiz = atan2f(m_LeftAxizX, m_LeftAxizY);
	return Axiz;
}
//=============================================================================
// 右スティック
//=============================================================================
float CInputJoyPad::GetRightAxiz(void)
{
	float Axiz = atan2f(m_RightAxizX, m_RightAxizY);
	return Axiz;
}
//=============================================================================
// 十字キー
//=============================================================================
float CInputJoyPad::GetRadian(void)
{
	float Radian = m_Radian / 100;
	return Radian;
}
//=============================================================================
// コンストラクタ
//=============================================================================
CXInputJoyPad::CXInputJoyPad()
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CXInputJoyPad::~CXInputJoyPad()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CXInputJoyPad::Init()
{
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}
//=============================================================================
// 更新処理
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

		// 情報持ってる
		xijs = m_Controllers[dCnt].state.Gamepad.wButtons;

		// ジョイパッド入力情報保存(トリガー)
		m_aKeyStateTrigger[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// ジョイパッド入力情報保存(リリース)
		m_aKeyStateRelese[dCnt] = (m_aKeyState[dCnt] ^ xijs)& xijs;

		// ジョイパッド入力情報保存(プレス)
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
// XInputの入力情報(プレス情報)を取得
//=============================================================================
bool CXInputJoyPad::GetPress(int nButton, int indexpad)
{
	return(m_aKeyState[indexpad] & nButton) ? true : false;
}
//=============================================================================
// XInputの入力情報(トリガー情報)を取得
//=============================================================================
bool CXInputJoyPad::GetTrigger(int nButton, int indexpad)
{
	return(m_aKeyStateTrigger[indexpad] & nButton) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(リリース情報)を取得
//=============================================================================
bool CXInputJoyPad::GetRelese(int nButton, int indexpad)
{
	return(m_aKeyStateRelese[indexpad] & nButton) ? true : false;
}
//=============================================================================
// 左スティック
//=============================================================================
float CXInputJoyPad::GetLeftAxiz(int indexpad)
{
	float Axiz = atan2f(m_LeftAxizX[indexpad], m_LeftAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// 右スティック
//=============================================================================
float CXInputJoyPad::GetRightAxiz(int indexpad)
{
	float Axiz = atan2f(m_RightAxizX[indexpad], m_RightAxizY[indexpad]);

	return Axiz;
}
//=============================================================================
// スティックの取得
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
