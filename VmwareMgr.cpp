// VmwareMgr.cpp : 定义控制台应用程序的入口点。
#include "VmwareMgr.h"

VirtualMachineInformationMgr gVmInfoMgr = { 0 };
VmwareControl gVmCtl;
VixHandle hostHandle = VIX_INVALID_HANDLE;

VmwareControl::VmwareControl(){
}

VmwareControl::~VmwareControl(){
}

int InserVmInfo(char* szPath) {
	strcpy(gVmInfoMgr.VmInformatin[gVmInfoMgr.vmCount].szVmxFilePath, szPath);	//虚拟机文件路径
	gVmInfoMgr.VmInformatin[gVmInfoMgr.vmCount].used = TRUE;				
	gVmInfoMgr.vmCount++;
	return TRUE;
}	

//判断目录是否存在 
int IsFileExists(const char* Path) { 
	WIN32_FIND_DATA  FileData = { 0 };
	HANDLE hFind = NULL;
	int Result = 0;

	hFind = FindFirstFile(Path, &FileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		Result = TRUE;
		FindClose(hFind);
	}
	return Result;
}	

int interface_test() { 

	for (;;) {
		// 开机
		gVmCtl.openVms();
		Sleep(60000 * 60 * 7);
		// 关机
		gVmCtl.closeVms();
		Sleep(60000);
		printf("again .......\n");
	}
	gVmCtl.disconnectHost();

	return TRUE;
}	

int VmwareControl::connctHost()
{	
	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;
	
	//连接到VMHOST
	jobHandle = pVixHost_Connect(VIX_API_VERSION,
		VIX_SERVICEPROVIDER_VMWARE_WORKSTATION,
		HOSTNAME,
		HOSTPORT,
		USERNAME,
		PASSWORD,
		0,
		VIX_INVALID_HANDLE,
		NULL,
		NULL);

// 	jobHandle = pVixHost_Connect(VIX_API_VERSION,
// 		CONNTYPE,
// 		HOSTNAME,
// 		HOSTPORT,
// 		USERNAME,
// 		PASSWORD,
// 		0,
// 		VIX_INVALID_HANDLE,
// 		NULL,
// 		NULL);

	//根据jobHandle 获取VMHOST句柄
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &hostHandle, VIX_PROPERTY_NONE);
	//释放jobHandle
	pVix_ReleaseHandle(jobHandle);
	if (VIX_FAILED(err))
	{	
		fprintf(stderr, "Failed to connect to host (%s)\n", pVix_GetErrorText(err, NULL));
		return FALSE;
	}	

	return TRUE;
}	

int VmwareControl::disconnectHost() {
	pVixHost_Disconnect(hostHandle);
	return TRUE;
}

int VmwareControl::getConfig() {

	char szConfigPath[MAX_PATH] = { 0 };

	GetCurrentDirectoryA(MAX_PATH, szConfigPath);
	strcat_s(szConfigPath, MAX_PATH, "\\config.ini");
	// 获取base文件
	GetPrivateProfileString("options", "baseMachinePath", NULL, gVmInfoMgr.BaseMachinePath, MAX_PATH, szConfigPath);
	// 获取工作路径
	GetPrivateProfileString("options", "workFloder", NULL, gVmInfoMgr.CloneOfMechineFloder, MAX_PATH, szConfigPath);
	// 获取待clone数量
	gVmInfoMgr.nClone = GetPrivateProfileInt("options", "NumberOfClone", NULL, szConfigPath);

	return TRUE;
}	

//克隆虚拟机
int VmwareControl::cloneVms() {	

	char szDestFloder[MAX_PATH] = { 0 };
	char szDestVmxFile[MAX_PATH] = { 0 };
	int i = 0;

	connctHost();

	for (i = 1; i <= gVmInfoMgr.nClone; i++)  {	

		sprintf_s(szDestFloder, "%sClone%d", gVmInfoMgr.CloneOfMechineFloder, i);
		sprintf_s(szDestVmxFile, "%s\\Clone%d.vmx", szDestFloder, i);

		//检查目标目录是否存在
		if (!IsFileExists(szDestFloder)) {
			//不存在创建一个 创建失败返回失败
			if (!CreateDirectory(szDestFloder, NULL)) {	
				printf("CreateDirectory Failed!\n");
				disconnectHost();
				return FALSE;
			}	
			cloneVmLink(gVmInfoMgr.BaseMachinePath, szDestVmxFile);
		}	
		else {
			printf("it has skipped virtual machine% d because clone already exists!\n", i);
		}
	}	
	disconnectHost();
	return TRUE;
}	

//打开所有已经克隆的虚拟机
int VmwareControl::luancher_cloned_vms() {

	char szDestFloder[MAX_PATH] = { 0 };
	char szDestVmxFile[MAX_PATH] = { 0 };
	int i = 0;

	connctHost();
	for (i = 1; i <= gVmInfoMgr.nClone; i++) { 
		sprintf_s(szDestFloder, "%sClone%d", gVmInfoMgr.CloneOfMechineFloder, i);
		sprintf_s(szDestVmxFile, "%s\\Clone%d.vmx", szDestFloder, i);
		//检查目标目录是否存在
		if (!IsFileExists(szDestVmxFile)) {
			printf("Virtual machine %d does not exist start failure!\n", i);
			continue;
		}	
		luancherVmByFilePath(szDestVmxFile);
	}	
	disconnectHost();
	return TRUE;
}

// 打开所有虚拟机
int VmwareControl::openVms() { 
	int i = 0;
	for (i = 0; i < gVmInfoMgr.vmCount; i++) { 
		printf("The VirtualMachine FilePath : %s\n", gVmInfoMgr.VmInformatin[i].szVmxFilePath);
		luancherVmByFilePath(gVmInfoMgr.VmInformatin[i].szVmxFilePath);
	}	
	return TRUE;
}	

//关闭所有虚拟机
int VmwareControl::closeVms() {	
	int i = 0;
	for (i = 0; i < gVmInfoMgr.vmCount; i++) { 
		closeVmByVmxPath(gVmInfoMgr.VmInformatin[i].szVmxFilePath);
	}	
	return TRUE;
}	

int VmwareControl::vmInitialization() { 
	if (!gVmInfoMgr.interfaceState) { 
		// 加载vmx接口
		if (!LoadVmDll()) {	
			printf("LoadVmDll Failed\n");
			return FALSE;
		}	
		gVmInfoMgr.interfaceState = TRUE;
	}	
	// 获取配置文件信息
	if (!gVmInfoMgr.configFileState) {
		getConfig();
		gVmInfoMgr.configFileState = TRUE;
	}	

// 	SearchVmxFile();
	return TRUE;
}

DWORD WINAPI WorkThreadProc(LPVOID lpParam) {	
	//初始化数据结构
	if (!gVmCtl.vmInitialization()) {
		return FALSE;
	}	
 	for (;;) {	
// 		interface_test();
		Sleep(1000);
 	}	
	return TRUE;
}	

int ProcessString(char* szInput)
{	
	char* szTurnOff = "off all";
	char* szTurnOn = "on all";
	char* szTurnClone = "clone";
	char* szTurnBoot = "cboot";
	char* szTurnclear = "clear";

	if (!strcmp(szInput, szTurnOff)) { 
	//	VmCtl.closeVms();
	}	
	else if (!strcmp(szInput, szTurnOn)) { 
	//	VmCtl.openVms();
	}	
	else if (!strcmp(szInput, szTurnClone)) {			//克隆虚拟机
		if (!gVmCtl.cloneVms()) {
			printf("cloneVms has called failed!\n");
			return FALSE;
		}	
		printf("clone Successed!\n");
	}	
	else if (!strcmp(szInput, szTurnBoot)) {			//启动所有虚拟机
		if (!gVmCtl.luancher_cloned_vms()) { 
			printf("luancher_cloned_vms has called failed!\n");
			return FALSE;
		}	
		else { 
			printf("cboot Successed!\n");
		}	
	}	
	else if (!strcmp(szInput, szTurnclear)) {
	}

	return TRUE;
}	

int _tmain(int argc, TCHAR* argv[])
{	
	char szInput[MAX_PATH] = { 0 };
	DWORD ThreadId = 0;
	HANDLE hWorkThread = NULL;

	//启动工作线程
	hWorkThread = CreateThread(NULL, 0, WorkThreadProc, NULL, 0, &ThreadId);
	if (hWorkThread == NULL) {	
		printf("The WorkThreadProc has Created Failed\n");
		getchar();
		return 0;
	}	

	do 
	{
		printf("Please enter the command!\n");
		scanf("%s", szInput);
		ProcessString(szInput);
	} while (TRUE);

	return 0;
}	

