#include "DeviceManager.h"

NEWDEL_IMPL(DeviceManager)
DeviceManager::DeviceManager()
{
	ResetDeviceArray();
}

void DeviceManager::SetDeviceState(DeviceType type, bool state)
{
	mDeviceArray[type] = (int)state;
}