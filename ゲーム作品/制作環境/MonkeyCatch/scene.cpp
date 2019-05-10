//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : �|���j
//
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "debugproc.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CScene * CScene::m_apTop[NUM_PRIORITY] = {};
CScene * CScene::m_apCur[NUM_PRIORITY] = {};

int CScene::m_nNumAll = 0;
int CScene::m_nNumPriority[NUM_PRIORITY] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	// ���S�t���OOFF
	m_bDeath = false;

	if (m_apCur[nPriority] != NULL)
	{// �J�����g��NULL����Ȃ�������
		m_apCur[nPriority]->m_pNext = this;
	}
	else
	{
		m_apTop[nPriority] = this;
	}

	m_pPrev = m_apCur[nPriority];
	m_pNext = NULL;

	m_apCur[nPriority] = this;

	m_nPriority = nPriority;		// �D�揇�ʂ̐ݒ�
	m_objType = objType;			// �I�u�W�F�N�g�̎�ސݒ�
	m_nNumPriority[nPriority]++;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}
//=============================================================================
// �S�ĉ��
//=============================================================================
void CScene::ReleseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		if (m_apTop[nCntPriority] != NULL)
		{
			while (m_apTop[nCntPriority])
			{
				// �I������
				m_apTop[nCntPriority]->Uninit();

				// �S�ďI��
				m_apTop[nCntPriority]->UninitAll(nCntPriority);
			}
		}
	}
}
//=============================================================================
// �S�čX�V
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CScene * pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene)
		{
			CScene * pSceneNext = pScene->m_pNext;	// �⌾  /* �X�V�����Ƃ��ɏ�����\�������邩�� */ 

			// �X�V����
			pScene->Update();

			pScene = pSceneNext;
		}
		//CDebugProc::Print(1, "%d�Ԗ� : %d\n", nCntPriority, m_nNumPriority[nCntPriority]);
	}
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CScene * pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene)
		{
			CScene * pSceneNext = pScene->m_pNext;	// �⌾  /* �X�V�����Ƃ��ɏ�����\�������邩�� */ 

			if (pScene->m_bDeath == true)
			{// ���S�t���O�������Ă����
				pScene->UninitAll(nCntPriority);
			}

			pScene = pSceneNext;
		}
		//CDebugProc::Print(1, "%d�Ԗ� : %d\n", nCntPriority, m_nNumPriority[nCntPriority]);
	}
	//CDebugProc::Print(1, "\n");
}
//=============================================================================
// �S�ĕ`��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CScene * pScene = m_apTop[nCntPriority];	// �擪����n�߂�

		while (pScene)
		{
			CScene * pSceneNext = pScene->m_pNext;	// �⌾

			// �`�揈��
			pScene->Draw();

			pScene = pSceneNext;
		}
	}
}
//=============================================================================
// �S�ďI��
//=============================================================================
void CScene::UninitAll(int nPriority)
{
	if (m_pPrev != NULL)
	{// �O�̃I�u�W�F�N�g��NULL����Ȃ�������
		m_pPrev->m_pNext = m_pNext;
	}
	else
	{
		m_apTop[nPriority] = m_pNext;
	}
	if (m_pNext != NULL)
	{//�@���̃I�u�W�F�N�g��NULL����Ȃ�������
		m_pNext->m_pPrev = m_pPrev;
	}
	else
	{
		m_apCur[nPriority] = m_pPrev;
	}
	delete this;

	m_nNumPriority[nPriority]--;
}

//=============================================================================
// �I�u�W�F�N�g�̔j��(���S�t���O)
//=============================================================================
void CScene::Release(void)
{
	// ���S�t���OON
	m_bDeath = true;
}
//=============================================================================
// �I�u�W�F�N�g�̎�ސݒ�
//=============================================================================
void CScene::SetObjType(OBJTYPE ObjType)
{
	m_objType = ObjType;
}
//=============================================================================
// �I�u�W�F�N�g�̎�ގ擾
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}
//=============================================================================
// �I�u�W�F�N�g�̐擪�擾
//=============================================================================
CScene *CScene::GetTop(int nPriority)
{
	return m_apTop[nPriority];
}
//=============================================================================
// �I�u�W�F�N�g�̎��̎擾
//=============================================================================
CScene *CScene::GetNext(void)
{
	return m_pNext;
}
//=============================================================================
// ���S�t���O�̎擾
//=============================================================================
bool CScene::GetDeath(void)
{
	return m_bDeath;
}
