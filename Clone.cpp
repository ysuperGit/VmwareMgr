#include "VmwareMgr.h"

int cloneVmLink(char* szBaseVmxPath, char* szClonePath)
{	
	VixError    err;
	VixHandle   jobHandle = VIX_INVALID_HANDLE;
	VixHandle   vmHandle = VIX_INVALID_HANDLE;
	char* pstr = NULL;

	jobHandle = pVixVM_Open(hostHandle,
		szBaseVmxPath,
		NULL,					// VixEventProc *callbackProc,
		NULL);					// void *clientData);

	//»ñÈ¡Vmx¾ä±ú
	err = pVixJob_Wait(jobHandle, VIX_PROPERTY_JOB_RESULT_HANDLE, &vmHandle, VIX_PROPERTY_NONE);
	pVix_ReleaseHandle(jobHandle);

	if (VIX_FAILED(err)) { 
		pstr = (char*)pVix_GetErrorText(err, NULL);
		return FALSE;
	}	
	// Create a clone of this virtual machine.
	jobHandle = pVixVM_Clone(vmHandle,
		VIX_INVALID_HANDLE,				// snapshotHandle
		VIX_CLONETYPE_LINKED,			// cloneType
		szClonePath,					// destConfigPathName
		0,								// options,
		VIX_INVALID_HANDLE,				// propertyListHandle
		NULL,							// callbackProc
		NULL);							// clientData
	err = pVixJob_Wait(jobHandle,
		VIX_PROPERTY_JOB_RESULT_HANDLE,
		&vmHandle,
		VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) { 
		// Handle the error...
		pVix_ReleaseHandle(jobHandle);
		return FALSE;
	}	
	pVix_ReleaseHandle(jobHandle);

	return TRUE;
}	