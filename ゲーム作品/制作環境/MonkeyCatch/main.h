//=============================================================================
//
// メイン処理 [main.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"						// 入力系に必要
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "xaudio2.h"					// サウンド処理に必要
#include <XInput.h> // XInput API

// メモリーリーク
#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻に必要
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"XInput.lib")	//XInputに必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// ウインドウの幅
#define SCREEN_HEIGHT	(720)		// ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線ベクトル / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE |D3DFVF_TEX1)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				//頂点座標
	float rhw;						//1.0fで固定
	D3DCOLOR col;					//頂点カラー
	D3DXVECTOR2 tex;				//テクスチャの座標
}VERTEX_2D;

typedef struct
{// 3Ｄポリゴン頂点フォーマットに合わせた構造体を定義
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
// 前方宣言 インクルードはいらない
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
// マクロ定義
//*****************************************************************************
#define MAX_CHAR	(256)	// 最大文字数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int GetFPS(void);

#endif