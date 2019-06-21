#pragma once
#include"Leap.h"
#include"LeapMath.h"
using namespace Leap;
class LeapListener:public Listener
{
public:
	virtual void onConnect(const Controller&);
	virtual void onFrame(const Controller&);
	
	
	void onInit(const Controller&);
	void onDisconnected(const Controller&);
	void onExit(const Controller & );
	void onDeviceChange(const Controller&);
	
};

