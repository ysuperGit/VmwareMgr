#pragma once

//����vmx�ļ�·�� �������
int luancherVmByFilePath(char* VmxPath);
//����vmx�ļ�·�� �ر������
int closeVmByVmxPath(char* VmxPath);
//��ѯ�������Դ״̬
VixPowerState queryPowerStateByVmxPath(char* VmxPath);