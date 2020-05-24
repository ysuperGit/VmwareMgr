#include "VmwareMgr.h"

//��ѯ�������Դ״̬ ����TRUE����������  FALSEû������
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

	//��ȡVmx���
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err))
	{	
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}	
	pVix_ReleaseHandle(jobHandle);

	//���ȴ� ������ȡ�����״̬
	err = pVix_GetProperties(vmHandle, VIX_PROPERTY_VM_POWER_STATE, &powerState, VIX_PROPERTY_NONE);
	if (VIX_OK != err) 
	{	
		return FALSE;
	}	

	pVix_ReleaseHandle(vmHandle);

// 	//ֻҪ���ǹػ�״̬ ��������������
// 	if (powerState != VIX_POWERSTATE_POWERED_OFF) {	
// 		return TRUE;
// 	}	

	return powerState;
}	

//����vmx�ļ�·�� �ر������
int closeVmByVmxPath(char* VmxPath)
{

	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;

	jobHandle = pVixVM_Open(hostHandle,
		VmxPath,
		NULL,					// VixEventProc *callbackProc,
		NULL);					// void *clientData);

	//��ȡVmx���
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err))
	{
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}

	pVix_ReleaseHandle(jobHandle);

	//�ػ�
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

//����vmx�ļ�·�� �������
int luancherVmByFilePath(char* VmxPath)
{	
	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;

	jobHandle = pVixVM_Open(hostHandle,
		VmxPath,
		NULL,					// VixEventProc *callbackProc,
		NULL);					// void *clientData);

	//��ȡVmx���
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{	
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}	

	pVix_ReleaseHandle(jobHandle);

	//����
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
