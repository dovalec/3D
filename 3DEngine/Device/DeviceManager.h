
#ifndef __DeviceManager__
#define __DeviceManager__

#include "../Config.h"

#define MAX_DEVICE_ARRAY 32

class DeviceManager {
public:
	DeviceManager();
public:
	NEWDEL_DECL

	enum DeviceType {
		CAMERA_TAKE_PIC = 0,
		CAMERA_PREVIEW_ON,
		CAMERA_PREVIEW_OFF,
		CAMERA_FLASH,
		VIBRATE,
		CAPTURE_SCREEN,
		EXIT_APP,
		DEVICE_NONE,
	};

	inline static DeviceManager * GetDeviceManager()
	{
		return GlobalHeap::GetDeviceManager();	
	}

	void SetDeviceState(DeviceType type, bool state);
	inline int * GetDeviceArray()
	{
		return mDeviceArray;
	}
	inline void ResetDeviceArray()
	{
		memset(mDeviceArray, 0, sizeof(mDeviceArray));
	}

protected:
	int mDeviceArray[MAX_DEVICE_ARRAY];
};


#endif //__DeviceManager__