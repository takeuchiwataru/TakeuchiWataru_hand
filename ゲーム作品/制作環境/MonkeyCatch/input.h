//=============================================================================
//
// 入力処理 [input.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)		// キー最大数
#define DI_JOY_I_INPUT (1000)
#define MAX_CONTROLLERS (4)		// コントローラー4つ分
#define INPUT_DEADZONE  (0.24f * FLOAT(0x7FFF) )
#define XINPUT_STICK_MAX (32767)
#define XINPUT_STICK_MIN (-32768)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CInput
{
public:
	CInput();
	virtual~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};
//*****************************************************************************
// キーボードクラス
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
};
//*****************************************************************************
// ジョイパッドクラス(DirectInput)
//*****************************************************************************
class CInputJoyPad : public CInput
{
public:
	CInputJoyPad();
	~CInputJoyPad();

	typedef enum
	{
		DIJS_BUTTON_1 = 0,	// X
		DIJS_BUTTON_2,		// Y
		DIJS_BUTTON_3,		// B
		DIJS_BUTTON_4,		// A
		DIJS_BUTTON_5,		// LB
		DIJS_BUTTON_6,		// RB
		DIJS_BUTTON_7,		// LT
		DIJS_BUTTON_8,		// RT

		DIJS_BUTTON_9,
		DIJS_BUTTON_10,
		DIJS_BUTTON_11,
		DIJS_BUTTON_12,		// STARTボタン
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,

		DIJS_BUTTON_20,		// 左スティック←
		DIJS_BUTTON_21,		// 左スティック↑
		DIJS_BUTTON_22,		// 左スティック→
		DIJS_BUTTON_23,		// 左スティック↓

		DIJS_BUTTON_24,		// 右スティック←
		DIJS_BUTTON_25,		// 右スティック↑
		DIJS_BUTTON_26,		// 右スティック→
		DIJS_BUTTON_27,		// 右スティック↓

		DIJS_BUTTON_28,		// 十字キー←
		DIJS_BUTTON_29,		// 十字キー↑
		DIJS_BUTTON_30,		// 十字キー→
		DIJS_BUTTON_31,		// 十字キー↓
		DIJS_BUTTON_MAX,
	}DIJS_BUTTON;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelese(int nButton);

	float GetLeftAxiz(void);
	float GetRightAxiz(void);
	float GetRadian(void);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelese[NUM_KEY_MAX];
	float m_LeftAxizX;
	float m_LeftAxizY;
	float m_RightAxizX;
	float m_RightAxizY;
	float m_Radian;
};
//*****************************************************************************
// ジョイパッドクラス(XInput)
//*****************************************************************************
class CXInputJoyPad
{
public:
	struct CONTROLER_STATE
	{
		XINPUT_STATE state;
		bool bConnected;
	};

	CXInputJoyPad();
	~CXInputJoyPad();

	HRESULT Init(void);
	void Update(void);
	bool GetPress(int nButton, int indexpad);
	bool GetTrigger(int nButton, int indexpad);
	bool GetRelese(int nButton, int indexpad);
	bool GetStick(int nLR, int indexpad);
	float GetLeftAxiz(int indexpad);
	float GetRightAxiz(int indexpad);

private:
	HRESULT UpdateControllerState(void);

	CONTROLER_STATE m_Controllers[MAX_CONTROLLERS];
	WORD m_aKeyState[MAX_CONTROLLERS];
	WORD m_aKeyStateTrigger[MAX_CONTROLLERS];
	WORD m_aKeyStateRelese[MAX_CONTROLLERS];
	WORD m_Stick[MAX_CONTROLLERS];

	float m_LeftAxizX[MAX_CONTROLLERS];
	float m_LeftAxizY[MAX_CONTROLLERS];
	float m_RightAxizX[MAX_CONTROLLERS];
	float m_RightAxizY[MAX_CONTROLLERS];


	//WCHAR m_szMessage[4][1024] = { 0 };
	//HWND    m_hWnd;
	//bool    m_bDeadZoneOn = true;
};
#endif