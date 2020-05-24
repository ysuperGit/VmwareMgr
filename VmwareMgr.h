#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include "vix.h"
#include "ImportFunc.h"
#include "PowerOperations.h"
#include "SearchVmxFile.h"
#include "Clone.h"

#pragma comment (lib,"Shlwapi.lib")

#define  CONNTYPE    VIX_SERVICEPROVIDER_VMWARE_WORKSTATION
#define  HOSTNAME ""
#define  HOSTPORT 0
#define  USERNAME ""
#define  PASSWORD ""
#define VM_HANDLE_ARRAY_SIZE 32
#define  VMPOWEROPTIONS   VIX_VMPOWEROP_LAUNCH_GUI   // Launches the VMware Workstaion UI
													 // when powering on the virtual machine.

extern VixHandle hostHandle;

typedef struct _VirtualMachineInformation
{
	BYTE used;								//��ǰ�ṹ�Ƿ���������
	char szVmxFilePath[MAX_PATH];			//������ļ�·��
	VixPowerState PowerState;				//�������Դ״̬

}VirtualMachineInformation, *PVirtualMachineInformation;

typedef struct _VirtualMachineInformationMgr { 
	VirtualMachineInformation VmInformatin[VM_HANDLE_ARRAY_SIZE];	//��ϸ��Ϣ
	int vmCount;													//��ǰ���������
	int interfaceState;												//�ӿڼ���״̬
	int configFileState;											//�����ļ�����״̬
	int nClone;
	
	char BaseMachinePath[MAX_PATH];
	char CloneOfMechineFloder[MAX_PATH];

}VirtualMachineInformationMgr, *PVirtualMachineInformationMgr;

class VmwareControl
{	
public:
	VmwareControl();
	~VmwareControl();


	int VmwareControl::connctHost();
	int VmwareControl::disconnectHost();

	int VmwareControl::vmInitialization();
	int VmwareControl::getConfig();
	int VmwareControl::openVms();
	int VmwareControl::closeVms();

	int VmwareControl::cloneVms();
	int VmwareControl::luancher_cloned_vms();
public:

private:

};

int InserVmInfo(char* szPath);