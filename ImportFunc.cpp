#include "VmwareMgr.h"


VixHost_Connect_Func pVixHost_Connect = NULL;
VixJob_Wait_Func pVixJob_Wait = NULL;
Vix_ReleaseHandle_Func pVix_ReleaseHandle = NULL;
Vix_GetErrorText_Func pVix_GetErrorText = NULL;
VixHost_Disconnect_Func pVixHost_Disconnect = NULL;
VixHost_FindItems_Func pVixHost_FindItems = NULL;
Vix_GetProperties_Func pVix_GetProperties = NULL;
VixVM_Open_Func pVixVM_Open = NULL;
Vix_FreeBuffer_Func pVix_FreeBuffer = NULL;
VixVM_PowerOn_Func pVixVM_PowerOn = NULL;
VixVM_PowerOff_Func pVixVM_PowerOff = NULL;
VixVM_Clone_Func pVixVM_Clone = NULL;

int LoadVmDll() { 
	HMODULE hModule = NULL;

	hModule = LoadLibraryA("Vix64AllProductsDyn.dll");
	if (!hModule)
	{
		return FALSE;
	}

	pVixHost_Connect = (VixHost_Connect_Func)GetProcAddress(hModule, "VixHost_Connect");
	if (!pVixHost_Connect)
		return FALSE;

	pVixJob_Wait = (VixJob_Wait_Func)GetProcAddress(hModule, "VixJob_Wait");
	if (!pVixJob_Wait)
		return FALSE;

	pVix_ReleaseHandle = (Vix_ReleaseHandle_Func)GetProcAddress(hModule, "Vix_ReleaseHandle");
	if (!pVix_ReleaseHandle)
		return FALSE;

	pVix_GetErrorText = (Vix_GetErrorText_Func)GetProcAddress(hModule, "Vix_GetErrorText");
	if (!pVix_GetErrorText)
		return FALSE;

	pVixHost_Disconnect = (VixHost_Disconnect_Func)GetProcAddress(hModule, "VixHost_Disconnect");
	if (!pVixHost_Disconnect)
		return FALSE;

	pVixHost_FindItems = (VixHost_FindItems_Func)GetProcAddress(hModule, "VixHost_FindItems");
	if (!pVixHost_FindItems)
		return FALSE;

	pVix_GetProperties = (Vix_GetProperties_Func)GetProcAddress(hModule, "Vix_GetProperties");
	if (!pVix_GetProperties)
		return FALSE;

	pVixVM_Open = (VixVM_Open_Func)GetProcAddress(hModule, "VixVM_Open");
	if (!pVixVM_Open)
		return FALSE;

	pVix_FreeBuffer = (Vix_FreeBuffer_Func)GetProcAddress(hModule, "Vix_FreeBuffer");
	if (!pVix_FreeBuffer)
		return FALSE;


	pVixVM_PowerOn = (VixVM_PowerOn_Func)GetProcAddress(hModule, "VixVM_PowerOn");
	if (!pVixVM_PowerOn)
		return FALSE;

	pVixVM_PowerOff = (VixVM_PowerOff_Func)GetProcAddress(hModule, "VixVM_PowerOff");
	if (!pVixVM_PowerOff)
		return FALSE;

	pVixVM_Clone = (VixVM_Clone_Func)GetProcAddress(hModule, "VixVM_Clone");
	if (!pVixVM_Clone)
		return FALSE;

	return TRUE;
}