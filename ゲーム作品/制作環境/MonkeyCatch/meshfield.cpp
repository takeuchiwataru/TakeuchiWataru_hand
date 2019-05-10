//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : �|���j
//
//=============================================================================
#include "meshfield.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField()
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@�̃|�C���^

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	// �e�������̐ݒ�
	m_nNumVertex = VERTEX_KAZU;		// ���_��
	m_nNumIndex = INDEX_KAZU;		// �C���f�b�N�X��
	m_nNumPolygon = POLYGON_KAZU;	// �|���S����

	CManager manager;	// �C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_pos = pos;

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, MF_TEXTURE, &m_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_KAZU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_KAZU,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 2�o�C�g�̃f�[�^
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
	{// �c�̖ʂ̐������J��Ԃ�
		for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
		{// ���̖ʂ̐������J��Ԃ�
			pVtx[0].pos = D3DXVECTOR3((nCntX * MESHFILED_POS) / MAX_MF_X, 0.0f, (nCntZ * -MESHFILED_POS) / MAX_MF_Z);		// rand()% 30 * 1.0f
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	CMeshField::Nor();

	WORD * pIdx;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0,nIdx = 0; nCntZ < MAX_MF_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++,nIdx++)
		{
			pIdx[0] = (MAX_MF_X + 1) + nIdx;
			pIdx[1] = 0 + nIdx;

			pIdx += 2;

			if (nCntZ < MAX_MF_Z - 1 && nCntX == MAX_MF_X)
			{
				pIdx[0] = 0 + nIdx;
				pIdx[1] = (MAX_MF_X + 1) + nIdx + 1;

				pIdx += 2;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputKeyboard->GetPress(DIK_L) == true)
	{// �����o��
		SaveMeshFiled();
	}

	if (pInputKeyboard->GetPress(DIK_K) == true)
	{// �ǂݍ���
		LoadMeshFiled();
	}

	if (pInputKeyboard->GetPress(DIK_M) == true)
	{// �@��
		Nor();
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	CManager manager;	//�C���X�^���X

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX_KAZU, 0, POLYGON_KAZU);
}
//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CMeshField::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//=============================================================================
// �����擾
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0, vec1, vec2;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 aVtxpos[3];

	D3DXVECTOR3 posmtx = pos - m_pos;

	int nNumber;	// �O�p�̎Q�Ƃ��钸�_
	float fHeight;	// �v���C���[�̍����ۊ�

	// ���݉��u���b�N�ɂ��邩
	int nMeshX = (int)(posmtx.x / (MESHFILED_POS / MAX_MF_X));
	int nMeshZ = (int)(posmtx.z / (MESHFILED_POS / MAX_MF_Z) * -1);

	// ���ݏ���Ă钸�_�̏o����
	int nMeshLU = nMeshX + nMeshZ *(MAX_MF_X + 1);
	int nMeshRU = (nMeshX + 1) + nMeshZ * (MAX_MF_X + 1);
	int nMeshLD = nMeshX + (nMeshZ + 1) * (MAX_MF_X + 1);
	int nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (MAX_MF_X + 1);

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	////�f�o�b�N�p
	//CDebugProc::Print(1, "���㒸�_ nMeshLU : %d\n", nMeshLU);
	//CDebugProc::Print(1, "�E�㒸�_ nMeshRU : %d\n", nMeshRU);
	//CDebugProc::Print(1, "�������_ nMeshLD : %d\n", nMeshLD);
	//CDebugProc::Print(1, "�E�����_ nMeshRD : %d\n", nMeshRD);
	//CDebugProc::Print(1, "���݂̃u���b�N ���� %d �u���b�N��\n", nMeshX);
	//CDebugProc::Print(1, "���݂̃u���b�N �c�� %d �u���b�N��\n", nMeshZ);

	// �x�N�g���̌v�Z�� (�ړI - ����)
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;

	// �x�N�g���̌v�Z�� (�����̈ʒu - ����)
	vec1 = posmtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{
		//CDebugProc::Print(1, "MeshField���E�͈� : TURE\n\n");

		vec0 = posmtx - pVtx[nMeshLU].pos;

		aVtxpos[0] = pVtx[nMeshRU].pos;
		aVtxpos[1] = pVtx[nMeshRD].pos;
		aVtxpos[2] = pVtx[nMeshLU].pos;

		nNumber = 3;
	}
	else if ((vec0.x *  vec1.z) - (vec0.z * vec1.x) >= 0)
	{
		//CDebugProc::Print(1, "MeshField�����͈� : TURE\n\n");

		vec0 = posmtx - pVtx[nMeshRD].pos;

		aVtxpos[0] = pVtx[nMeshLD].pos;
		aVtxpos[1] = pVtx[nMeshLU].pos;
		aVtxpos[2] = pVtx[nMeshRD].pos;

		nNumber = 0;
	}

	fHeight = aVtxpos[0].y;

	aVtxpos[2].y -= aVtxpos[0].y;
	aVtxpos[1].y -= aVtxpos[0].y;
	aVtxpos[0].y -= aVtxpos[0].y;

	// �x�N�g�����o����
	vec0 = aVtxpos[1] - aVtxpos[0];
	vec1 = aVtxpos[2] - aVtxpos[0];

	// �O�ς̎�
	D3DXVec3Cross(&nor, &vec0, &vec1);

	// ���K��
	D3DXVec3Normalize(&nor, &nor);

	vec0 = posmtx - aVtxpos[0];

	// ���ς̎�
	((vec0.x * pVtx[nNumber].nor.x) + (vec0.y *  pVtx[nNumber].nor.y) + (vec0.z *  pVtx[nNumber].nor.z));
	vec0.y = (-(vec0.x * nor.x) - (vec0.z * nor.z)) / nor.y;

	// �v���C���[�̈ʒu�����ɖ߂�
	posmtx.y = vec0.y + fHeight + m_pos.y;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return posmtx.y;
}
//=============================================================================
// �@��
//=============================================================================
void CMeshField::Nor(void)
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 aVtxNor[2];
	int nCntZ;
	int nCntX;

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntZ = 0; nCntZ < MAX_MF_Z; nCntZ++)
	{// �c�̖ʂ̐������J��Ԃ�
		for (nCntX = 0; nCntX < MAX_MF_X; nCntX++)
		{// ���̖ʂ̐������J��Ԃ�
			pVtx[nCntX + nCntZ * (MAX_MF_X + 1)].nor = D3DXVECTOR3(0.0f, -0.3f, 0.0f);
		}
	}
	for (nCntZ = 0; nCntZ < MAX_MF_Z; nCntZ++)
	{// �c�̖ʂ̐������J��Ԃ�
		for (nCntX = 0; nCntX < MAX_MF_X; nCntX++)
		{// ���̖ʂ̐������J��Ԃ�

			int nMeshLU = nCntX + nCntZ *(MAX_MF_X + 1);
			int nMeshRU = (nCntX + 1) + nCntZ * (MAX_MF_X + 1);
			int nMeshLD = nCntX + (nCntZ + 1) * (MAX_MF_X + 1);
			int nMeshRD = (nCntX + 1) + (nCntZ + 1) * (MAX_MF_X + 1);

			// �x�N�g���̌v�Z�� (�ړI - ����)
			vec0 = pVtx[nMeshLU].pos - pVtx[nMeshLD].pos;

			// �x�N�g���̌v�Z�� (�ړI - ����)
			vec1 = pVtx[nMeshRD].pos - pVtx[nMeshLD].pos;

			// �O�ς̎�
			D3DXVec3Cross(&aVtxNor[0], &vec0, &vec1);

			// ���K��
			D3DXVec3Normalize(&aVtxNor[0], &aVtxNor[0]);

			// �x�N�g���̌v�Z�� (�ړI - ����)
			vec0 = pVtx[nMeshRD].pos - pVtx[nMeshRU].pos;

			// �x�N�g���̌v�Z�� (�ړI - ����)
			vec1 = pVtx[nMeshLU].pos - pVtx[nMeshRU].pos;

			// �O�ς̎�
			D3DXVec3Cross(&aVtxNor[1], &vec0, &vec1);

			// ���K��
			D3DXVec3Normalize(&aVtxNor[1], &aVtxNor[1]);

			pVtx[nMeshLD].nor += aVtxNor[0];
			pVtx[nMeshLU].nor += aVtxNor[0] + aVtxNor[1];
			pVtx[nMeshRD].nor += aVtxNor[0] + aVtxNor[1];
			pVtx[nMeshRU].nor += aVtxNor[1];
		}
	}

	for (nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
	{// �c�̖ʂ̐������J��Ԃ�
		for (nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
		{// ���̖ʂ̐������J��Ԃ�
			if (nCntZ == 0)
			{// ��[
				if (nCntX > 0 && nCntX < MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 3.0f;
				}
				else if (nCntX == 0)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 2.0f;
				}
			}
			else if (nCntZ == MAX_MF_Z)
			{// ���[
				if (nCntX > 0 && nCntX < MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 3.0f;
				}
				else if (nCntX == MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 2.0f;
				}
			}
			else
			{// ����
				if (nCntX > 0 && nCntX < MAX_MF_X)
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 6.0f;
				}
				else
				{// 
					pVtx[nCntX + nCntZ *(MAX_MF_X + 1)].nor /= 3.0f;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �����ݒu
//=============================================================================
void CMeshField::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	D3DXVECTOR3 posmtx = pos - m_pos;

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
	{// �c�̖ʂ̐������J��Ԃ�
		for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
		{// ���̖ʂ̐������J��Ԃ�

			float fLength = (float)sqrt((posmtx.x - pVtx->pos.x)*(posmtx.x - pVtx->pos.x) + (posmtx.z - pVtx->pos.z)*(posmtx.z - pVtx->pos.z));

			if (fLength <= fRange)
			{
				float fHeight = cosf((D3DX_PI * 0.5f)*(fLength / fRange)) * fValue;

				pVtx->pos.y += fHeight;
			}
			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ����
//=============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos)
{
	CMeshField *pMeshFiled = NULL;

	if (pMeshFiled == NULL)
	{
		pMeshFiled = new CMeshField;

		if (pMeshFiled != NULL)
		{
			pMeshFiled->Init(pos);
		}
	}
	return pMeshFiled;
}
//=============================================================================
// �Z�[�u
//=============================================================================
void CMeshField::SaveMeshFiled(void)
{
	FILE *pFile;

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �����o��
	pFile = fopen("data/TEXT/MeshfieldSave.txt", "w");
	if (pFile != NULL)
	{
		for (int nCntZ = 0; nCntZ < MAX_MF_Z; nCntZ++)
		{// �c�̖ʂ̐������J��Ԃ�
			for (int nCntX = 0; nCntX < MAX_MF_X; nCntX++)
			{// ���̖ʂ̐������J��Ԃ�
				fprintf(pFile, "%.1f\n",pVtx->pos.y);
				pVtx++;
			}
		}
		fclose(pFile);
	}
	else
	{
		printf("�t�@�C�����J���܂���\n");
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ���[�h
//=============================================================================
void CMeshField::LoadMeshFiled(void)
{
	FILE *pFile;

	// ���_����ݒ�
	VERTEX_3D * pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ǂݍ���
	pFile = fopen("data/TEXT/MeshfieldSave.txt", "r");
	if (pFile != NULL)
	{
		for (int nCntZ = 0; nCntZ <= MAX_MF_Z; nCntZ++)
		{// �c�̖ʂ̐������J��Ԃ�
			for (int nCntX = 0; nCntX <= MAX_MF_X; nCntX++)
			{// ���̖ʂ̐������J��Ԃ�
				fscanf(pFile, "%f\n", &pVtx->pos.y);
				pVtx++;
			}
		}
		fclose(pFile);
	}
	else
	{
		printf("�t�@�C�����J���܂���\n");
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}