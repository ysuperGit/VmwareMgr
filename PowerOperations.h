#pragma once

//根据vmx文件路径 打开虚拟机
int luancherVmByFilePath(char* VmxPath);
//根据vmx文件路径 关闭虚拟机
int closeVmByVmxPath(char* VmxPath);
//查询虚拟机电源状态
VixPowerState queryPowerStateByVmxPath(char* VmxPath);