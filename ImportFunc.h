#pragma once



typedef VixHandle(__stdcall* VixHost_Connect_Func)(int apiVersion,
	VixServiceProvider hostType,
	const char *hostName,
	int hostPort,
	const char *userName,
	const char *password,
	VixHostOptions options,
	VixHandle propertyListHandle,
	VixEventProc *callbackProc,
	void *clientData);


typedef VixError(__stdcall* VixJob_Wait_Func)(VixHandle jobHandle, VixPropertyID firstPropertyID, ...);
typedef void(__stdcall* Vix_ReleaseHandle_Func)(VixHandle handle);
typedef const char* (__stdcall* Vix_GetErrorText_Func)(VixError err, const char *locale);
typedef void(__stdcall* VixHost_Disconnect_Func)(VixHandle hostHandle);
typedef VixHandle(__stdcall* VixHost_FindItems_Func)(VixHandle hostHandle,
	VixFindItemType searchType,
	VixHandle searchCriteria,
	int32 timeout,
	VixEventProc *callbackProc,
	void *clientData);
typedef VixError(__stdcall* Vix_GetProperties_Func)(VixHandle handle,
	VixPropertyID firstPropertyID, ...);
typedef VixHandle(__stdcall* VixVM_Open_Func)(VixHandle hostHandle,
	const char *vmxFilePathName,
	VixEventProc *callbackProc,
	void *clientData);
typedef void(__stdcall* Vix_FreeBuffer_Func)(void *p);



typedef VixHandle(__stdcall* VixVM_PowerOn_Func)(VixHandle vmHandle,
	VixVMPowerOpOptions powerOnOptions,
	VixHandle propertyListHandle,
	VixEventProc *callbackProc,
	void *clientData);

typedef VixHandle(__stdcall* VixVM_PowerOff_Func)(VixHandle vmHandle,
	VixVMPowerOpOptions powerOffOptions,
	VixEventProc *callbackProc,
	void *clientData);

typedef VixHandle (_stdcall* VixVM_Clone_Func)(VixHandle vmHandle,
	VixHandle snapshotHandle,
	VixCloneType cloneType,
	const char *destConfigPathName,
	int options,
	VixHandle propertyListHandle,
	VixEventProc *callbackProc,
	void *clientData);




extern VixHost_Connect_Func pVixHost_Connect;
extern VixJob_Wait_Func pVixJob_Wait;
extern Vix_ReleaseHandle_Func pVix_ReleaseHandle;
extern Vix_GetErrorText_Func pVix_GetErrorText;
extern VixHost_Disconnect_Func pVixHost_Disconnect;
extern VixHost_FindItems_Func pVixHost_FindItems;
extern Vix_GetProperties_Func pVix_GetProperties;
extern VixVM_Open_Func pVixVM_Open;
extern Vix_FreeBuffer_Func pVix_FreeBuffer;
extern VixVM_PowerOn_Func pVixVM_PowerOn;
extern VixVM_PowerOff_Func pVixVM_PowerOff;
extern VixVM_Clone_Func pVixVM_Clone;



int LoadVmDll();
