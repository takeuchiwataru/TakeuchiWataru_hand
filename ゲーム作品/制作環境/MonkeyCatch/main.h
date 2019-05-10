//=============================================================================
//
// ���C������ [main.h]
// Author : �|���j
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "d3dx9.h"						// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"						// ���͌n�ɕK�v
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "xaudio2.h"					// �T�E���h�����ɕK�v
#include <XInput.h> // XInput API

// �������[���[�N
#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������ɕK�v
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"XInput.lib")	//XInput�ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |D3DFVF_TEX1)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				//���_���W
	float rhw;						//1.0f�ŌŒ�
	D3DCOLOR col;					//���_�J���[
	D3DXVECTOR2 tex;				//�e�N�X�`���̍��W
}VERTEX_2D;

typedef struct
{// 3�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_3D;

//*****************************************************************************
// �O���錾 �C���N���[�h�͂���Ȃ�
//*****************************************************************************
class CRenderer;
class CDebugProc;
class CScene;
class CRenderer;
class CManager;
class CInputKeyboard;
class CInputJoyPad;
class CCamera;
class CLight;
class CSound;
class CScene3D;
class CSceneX;
class CMeshField;
class CModel;
class CMeshOrbit;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR	(256)	// �ő啶����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int GetFPS(void);

#endif