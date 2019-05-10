//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 竹内亘
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSound
{
public:
	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum //ラベルと数と並びを揃える
	{
		SOUND_LABEL_BGM_GAME = 0,
		SOUND_LABEL_BGM_TITLE,
		SOUND_LABEL_BGM_TUTORIAL,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_SE_MUSIAMI,
		SOUND_LABEL_SE_JUMP,
		SOUND_LABEL_SE_JYAN,
		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_ENETR,
		SOUND_LABEL_SE_OPEN,
		SOUND_LABEL_SE_CLOSE,
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	virtual HRESULT Init(HWND hWnd);
	virtual void Uninit(void);

	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

																// 各音素材のパラメータ ラベルと数と並びを揃える
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/BGM_game.wav",-1 },		// ゲームBGM
		{ "data/BGM/BGM_title.wav",-1 },	// タイトルBGM / ランキング
		{ "data/BGM/BGM_tutorial.wav",-1 },	// チュートリアルBGM
		{ "data/BGM/BGM_result.wav",-1 },	// リザルトBGM
		{ "data/SE/musiami.wav", 0 },		// 虫網
		{ "data/SE/jump.wav", 0 },			// ジャンプ
		{ "data/SE/jyan.wav", 0 },			// ジャーン
		{ "data/SE/select.wav", 0 },		// 選択音
		{ "data/SE/enter.wav", 0 },			// 決定音
		{ "data/SE/open.wav", 0 },			// ポーズ開く音
		{ "data/SE/close.wav", 0 },			// ポーズ閉じる音
	};
};
#endif
