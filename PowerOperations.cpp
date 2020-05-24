#include "VmwareMgr.h"

//查询虚拟机电源状态 返回TRUE就是在运行  FALSE没在运行
VixPowerState queryPowerStateByVmxPath(char* VmxPath)
{
	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;
	VixPowerState powerState = 0;

	jobHandle = pVixVM_Open(hostHandle,
		VmxPath,
		NULL,					// VixEventProc *callbackProc,
		NULL);					// void *clientData);

	//获取Vmx句柄
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err))
	{	
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}	
	pVix_ReleaseHandle(jobHandle);

	//不等待 立即获取虚拟机状态
	err = pVix_GetProperties(vmHandle, VIX_PROPERTY_VM_POWER_STATE, &powerState, VIX_PROPERTY_NONE);
	if (VIX_OK != err) 
	{	
		return FALSE;
	}	

	pVix_ReleaseHandle(vmHandle);

// 	//只要不是关机状态 都当作是在运行
// 	if (powerState != VIX_POWERSTATE_POWERED_OFF) {	
// 		return TRUE;
// 	}	

	return powerState;
}	

//根据vmx文件路径 关闭虚拟机
int closeVmByVmxPath(char* VmxPath)
{

	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;

	jobHandle = pVixVM_Open(hostHandle,
		VmxPath,
		NULL,					// VixEventProc *callbackProc,
		NULL);					// void *clientData);

	//获取Vmx句柄
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err))
	{
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}

	pVix_ReleaseHandle(jobHandle);

	//关机
	jobHandle = pVixVM_PowerOff(vmHandle,
		VIX_VMPOWEROP_NORMAL,
		NULL, // *callbackProc,
		NULL); // *clientData);

	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err))
	{
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}
	pVix_ReleaseHandle(jobHandle);
	pVix_ReleaseHandle(vmHandle);

	return TRUE;
}

//根据vmx文件路径 打开虚拟机
int luancherVmByFilePath(char* VmxPath)
{	
	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;

	jobHandle = pVixVM_Open(hostHandle,
		VmxPath,
		NULL,					// VixEventProc *callbackProc,
		NULL);					// void *clientData);

	//获取Vmx句柄
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{	
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}	

	pVix_ReleaseHandle(jobHandle);

	//开机
	jobHandle = pVixVM_PowerOn(vmHandle,
		VMPOWEROPTIONS,
		VIX_INVALID_HANDLE,
		NULL,					// *callbackProc,
		NULL);					// *clientData);

	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{	
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}	

	pVix_ReleaseHandle(jobHandle);
	pVix_ReleaseHandle(vmHandle);

	return TRUE;
}	
