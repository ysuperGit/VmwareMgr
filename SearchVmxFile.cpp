#include "VmwareMgr.h"



int SearchDirOfFile(char* Path)
{	
	WIN32_FIND_DATA FileData = { 0 };
	HANDLE hFind = NULL;
	char szdestDirectory[MAX_PATH] = { 0 };
	char szVolumeDirectory[MAX_PATH] = { 0 };
	char szFileName[MAX_PATH] = { 0 };
	char* pszExtensionName = NULL;

	sprintf(szdestDirectory, "%s\\*.*", Path);

	//��ʼ���������ļ�
	hFind = FindFirstFile(szdestDirectory, &FileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{	
		//����ʧ�� ��ΪԴĿ¼������
		//printf("error %d \n", GetLastError());
		return FALSE;
	}	

	for (;;)
	{	
		if (FileData.cFileName[0] != '.')
		{	
			//�ж��ļ��Ƿ���һ��Ŀ¼
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{	
				//��ȥ�����ļ�
			}	
			else
			{	
				strcpy(szFileName, FileData.cFileName);
				pszExtensionName = PathFindExtension(szFileName);
				//�ж��ļ��Ƿ���VMX�ļ���ʽ
				if (pszExtensionName && !strcmp(pszExtensionName,".vmx"))
				{	
					//����ļ���vmx��ʽ ����ӽ��ṹ
					strcpy(szVolumeDirectory, Path);
					strcat(szVolumeDirectory, "\\");
					strcat(szVolumeDirectory, FileData.cFileName);
					InserVmInfo(szVolumeDirectory);
				}	
			}	
		}	

		if (!FindNextFile(hFind, &FileData)) {	
			break;
		}	
	}	

	FindClose(hFind);

	return TRUE;
}	

int SearchVolumeOfDir(char* SymbolPath)
{	
	WIN32_FIND_DATA FileData = { 0 };
	HANDLE hFind = NULL;
	char szdestDirectory[MAX_PATH] = { 0 };
	char szVolumeDirectory[MAX_PATH] = { 0 };

	sprintf(szdestDirectory, "%s\\*.*", SymbolPath);

	//��ʼ���������ļ�
	hFind = FindFirstFile(szdestDirectory, &FileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{	
		//����ʧ�� ��ΪԴĿ¼������
		//printf("error %d \n", GetLastError());
		return FALSE;
	}	

	for (;;)
	{	
		if (FileData.cFileName[0] != '.')
		{	
			//�ж��ļ��Ƿ���һ��Ŀ¼
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{	
				strcpy(szVolumeDirectory, SymbolPath);
				strcat(szVolumeDirectory, "\\");
				strcat(szVolumeDirectory, FileData.cFileName);
				//��ȥ����VMX��ʽ�ļ�
				SearchDirOfFile(szVolumeDirectory);
				SearchVolumeOfDir(szVolumeDirectory);
			}	
		}	

		if (!FindNextFile(hFind, &FileData))
		{	
			break;
		}	
	}	

	FindClose(hFind);

	return TRUE;
}	

int SearchVmxFile()
{	
	SearchVolumeOfDir("c:");
	SearchVolumeOfDir("d:");

	return TRUE;
}	