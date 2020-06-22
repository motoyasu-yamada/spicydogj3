#if defined(DOJA)
#define SYSTEM_SET_BACKLIGH_ON   PhoneSystem.setAttribute(PhoneSystem.DEV_BACKLIGHT,PhoneSystem.ATTR_BACKLIGHT_ON)
#define SYSTEM_SET_BACKLIGH_OFF  PhoneSystem.setAttribute(PhoneSystem.DEV_BACKLIGHT,PhoneSystem.ATTR_BACKLIGHT_OFF)
#define SYSTEM_SET_VIBRATOR_ON   PhoneSystem.setAttribute(PhoneSystem.DEV_VIBRATOR,PhoneSystem.ATTR_VIBRATOR_ON)
#define SYSTEM_SET_VIBRATOR_OFF  PhoneSystem.setAttribute(PhoneSystem.DEV_VIBRATOR,PhoneSystem.ATTR_VIBRATOR_OFF)

#elif defined(JPHONE)
#define SYSTEM_SET_BACKLIGH_ON  DeviceControl.getDefaultDeviceControl().setDeviceActive(DeviceControl.BACK_LIGHT,true)
#define SYSTEM_SET_BACKLIGH_OFF DeviceControl.getDefaultDeviceControl().setDeviceActive(DeviceControl.BACK_LIGHT,false)
#define SYSTEM_SET_VIBRATOR_ON  DeviceControl.getDefaultDeviceControl().setDeviceActive(DeviceControl.VIBRATION,true)
#define SYSTEM_SET_VIBRATOR_OFF DeviceControl.getDefaultDeviceControl().setDeviceActive(DeviceControl.VIBRATION,false)

#else
	#error unsupported configuration.
#endif
