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

	//开始遍历所有文件
	hFind = FindFirstFile(szdestDirectory, &FileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{	
		//返回失败 因为源目录不存在
		//printf("error %d \n", GetLastError());
		return FALSE;
	}	

	for (;;)
	{	
		if (FileData.cFileName[0] != '.')
		{	
			//判断文件是否是一个目录
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{	
				//进去查找文件
			}	
			else
			{	
				strcpy(szFileName, FileData.cFileName);
				pszExtensionName = PathFindExtension(szFileName);
				//判断文件是否是VMX文件格式
				if (pszExtensionName && !strcmp(pszExtensionName,".vmx"))
				{	
					//如果文件是vmx格式 就添加进结构
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

	//开始遍历所有文件
	hFind = FindFirstFile(szdestDirectory, &FileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{	
		//返回失败 因为源目录不存在
		//printf("error %d \n", GetLastError());
		return FALSE;
	}	

	for (;;)
	{	
		if (FileData.cFileName[0] != '.')
		{	
			//判断文件是否是一个目录
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{	
				strcpy(szVolumeDirectory, SymbolPath);
				strcat(szVolumeDirectory, "\\");
				strcat(szVolumeDirectory, FileData.cFileName);
				//进去查找VMX格式文件
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