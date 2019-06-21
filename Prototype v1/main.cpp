#include"iostream"

#include"LeapListener.h"

int main()
{
	LeapListener alistener;
	Leap::Controller controller;
	// read
	bool lowResourceMode = controller.config().getBool("low_resource_mode_enabled");
	// write
	controller.config().setBool("low_resource_mode_enabled", false);
	bool batteryPowerSaver = controller.config().getBool("power_saving_battery");
	// write
	controller.config().setBool("power_saving_battery", false);
	controller.config().save();
	//controller.addListener(listener);
	controller.addListener(alistener);
	std::cin.get();
	controller.removeListener(alistener);
	return 0 ;
}