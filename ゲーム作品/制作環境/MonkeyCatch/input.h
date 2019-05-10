//=============================================================================
//
// ���͏��� [input.h]
// Author : �|���j
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_KEY_MAX (256)		// �L�[�ő吔
#define DI_JOY_I_INPUT (1000)
#define MAX_CONTROLLERS (4)		// �R���g���[���[4��
#define INPUT_DEADZONE  (0.24f * FLOAT(0x7FFF) )
#define XINPUT_STICK_MAX (32767)
#define XINPUT_STICK_MIN (-32768)

//*****************************************************************************
// �N���X�̒�`
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
// �L�[�{�[�h�N���X
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
// �W���C�p�b�h�N���X(DirectInput)
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
		DIJS_BUTTON_12,		// START�{�^��
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,

		DIJS_BUTTON_20,		// ���X�e�B�b�N��
		DIJS_BUTTON_21,		// ���X�e�B�b�N��
		DIJS_BUTTON_22,		// ���X�e�B�b�N��
		DIJS_BUTTON_23,		// ���X�e�B�b�N��

		DIJS_BUTTON_24,		// �E�X�e�B�b�N��
		DIJS_BUTTON_25,		// �E�X�e�B�b�N��
		DIJS_BUTTON_26,		// �E�X�e�B�b�N��
		DIJS_BUTTON_27,		// �E�X�e�B�b�N��

		DIJS_BUTTON_28,		// �\���L�[��
		DIJS_BUTTON_29,		// �\���L�[��
		DIJS_BUTTON_30,		// �\���L�[��
		DIJS_BUTTON_31,		// �\���L�[��
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
// �W���C�p�b�h�N���X(XInput)
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