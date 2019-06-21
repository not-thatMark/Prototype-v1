#include "LeapListener.h"

//if a frame is detected(not matter what in it)
//device connected 
void LeapListener::onConnect(const Controller&controller) {
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_SWIPE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
}
void LeapListener::onFrame(const Controller & controller)
{	//some strings from the conviniency of coding and tracking
	std::string finger_names[5] = { "Thumb","Index","Middle","Ring","Pinky" };
	std::string number[11] = { "zero","one","two","three","four","five","six","seven","eight","nine","ten" };
	std::string bone_names[5] = { "Carpal","Meta","Proxi","Inter","Distal" };
	std::string state_names[4] = { "STATE_INVALID","STATE_START","STATE_UPDATE","STATE_END" };
	//basic objects and entities of the objects includes: Hands, Fingers, Pointable Objects, their matrixes, velocity,etc..
	Frame frame = controller.frame();
	HandList hands = frame.hands();
    Hand hand = hands[0];
	Hand hand2 = hands[1];
	FingerList fingers = frame.fingers();
	FingerList fingers1 = frame.hands()[0].fingers();
	PointableList pointables = frame.pointables();
	Pointable pointable = frame.pointables().frontmost();
	Finger finger = Finger(pointable);
	Vector pointingDirection = finger.direction();
	std::string fingert;
	std::string handType;
	Matrix basis = hand.basis();
	Matrix basis2 = hand2.basis();
	Vector xBasis = basis.xBasis;
	Vector xBasis2 = basis2.xBasis;
	Vector radius = hand.sphereCenter();
	Vector radius2 = hand2.sphereCenter();
	float gap = radius.x - radius2.x;
	int extendedFingers = 0;
	GestureList gestures = frame.gestures();
	Vector handspeed = hand.palmVelocity();
	Vector handNo = hand.palmNormal();
	float handNorY = hand.palmNormal().y;
	float circle = hand.sphereRadius();
	float radian_angle = Vector::yAxis().angleTo(handNo);
	Vector handDir = hand.direction();
	bool flag=true;
	int counter = 0;
	//first check how many hands we have in the sensor 
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); hl++)
	{	//if there is only one hand
		if (frame.hands().count() == 1)
		{
			
			//then how many fingers on the set
			for (int f = 0; f < frame.fingers().count(); f++)
			{
				Leap::Finger finger = frame.fingers()[f];
				if (finger.isExtended())
					extendedFingers++;
			}
		
			//no fingers found(fists)
			if (extendedFingers == 0)
			{
				if (handDir.y < 0 && circle<40)
				{
					std::cout<< " My " << std::endl;
				}
				if (handDir.y > 0 && circle<40)
				{
					std::cout << " Yours " << std::endl;
				}

			
			}
			//found 1 extended finger
			else if (extendedFingers == 1)
			{
			//check what is the finger
			/*	extendedFingers = 0;*/
				for (int f = 0; f < frame.fingers().count(); f++)
				{
					Leap::Finger finger = frame.fingers()[f];
					if (finger.isExtended() && finger.type() == 0)
						extendedFingers + 1;
					else if (finger.isExtended() && finger.type() == 1)
						extendedFingers = extendedFingers+2;
				}
				//its a thumb do the good and bad gestures
				if (extendedFingers == 1)
				{
					extendedFingers = 0;
					for (int f = 0; f < frame.fingers().count(); f++)
					{
						Leap::Finger finger = frame.fingers()[f];
						Leap::Bone bone = finger.bone(Leap::Bone::TYPE_DISTAL);
						Leap::Vector distalDirection = bone.basis().yBasis;
						if (finger.isExtended() && finger.type() == 0)
						{
							extendedFingers = 1;
						}
						if (extendedFingers == 1 && distalDirection.y>0.3)
							std::cout << "Thumb up" << std::endl;
						else if (extendedFingers == 1 && distalDirection.y<0)
							std::cout << "Thumb down" << std::endl;
					}

				}
				//its index do gesture I and you
				if(extendedFingers==3)
				{
					/*std::cout << pointingDirection.angleTo(Vector::yAxis()) << std::endl;*/
					
					if (pointingDirection.x <= 1 && pointingDirection.angleTo(Vector::yAxis()) <= 1)
					{
						std::cout << "Me or I" << std::endl;
					}
					else if (pointingDirection.x < 1 && pointingDirection.angleTo(Vector::yAxis())>1)
						std::cout << "You" << std::endl;

				}

			}
			//found 2 extended fingers
			else if (extendedFingers == 2)
			{
				
				////name
				for (int f = 0; f < frame.fingers().count(); f++)
				{
					Leap::Finger finger = frame.fingers()[f];
					if (finger.isExtended() && finger.type() == 0)
						extendedFingers++;
					if (finger.isExtended() && finger.type() == 1)
						extendedFingers++;
				}

				if (extendedFingers == 2)
				{
					std::cout << "Found it" << " " << extendedFingers << std::endl;

				}

			
			}
			////found 3 extended fingers
			else if (extendedFingers == 3)
			{
				extendedFingers = 0;
			//	//love
				for (int f = 0; f < frame.fingers().count(); f++)
				{
					Leap::Finger finger = frame.fingers()[f];
					if (finger.isExtended() && finger.type() == 4 && f == 2)
						extendedFingers++;
					if (finger.isExtended() && finger.type() == 0 && f == 2)
						extendedFingers++;
					if (finger.isExtended() && finger.type() == 1 && f == 2)
						extendedFingers++;
				}

				if (extendedFingers == 1)
					std::cout << "Love" << std::endl;
			}
			////found 4 extended fingers
			else if (extendedFingers == 4)
			{
			}
			////found 5 extended fingers
			else if (extendedFingers == 5)
			{
				//hi//hello/bye
				 if (handNorY <-0.6 && radian_angle>2.5)
				{
					std::cout << "Bye" << std::endl;
				}
				//	//how old
				else if (radian_angle >0.4 && handNorY<0 && radian_angle<2.54)
				{
					std::cout << "How old" << std::endl;
				}
			}
		}
		//if both hands on the sensor
		else if (frame.hands().count() == 2)
		{	//reset counter
			extendedFingers = 0;
		//then how many fingers on the set
			for (int f = 0; f < frame.fingers().count(); f++)
			{
				Leap::Finger finger = frame.fingers()[f];
				if (finger.isExtended())
					extendedFingers++;
			}
			////no fingers found(fists)
			if (extendedFingers == 0)
			{
				//family
				if (gap<=20)
					std::cout << "Family" << std::endl;
			}
			if (extendedFingers == 1)
			{
			}
			////found 2 extended fingers(1 fist)
			else if (extendedFingers == 2)
			{
			//	//how are you
				extendedFingers = 0;
			
				for (int f = 0; f < frame.fingers().count(); f++)
				{
					Leap::Finger finger = frame.fingers()[f];
					if (finger.isExtended()&& finger.type()==0)
						extendedFingers++;
					
				}
					if(extendedFingers!=0)
					std::cout << "How are you? " << std::endl;
				

			}
			////found 3 extended fingers(1 fist)
			else if (extendedFingers == 3){ std::cout << "Hands: " << frame.hands().count() << "Fingers: " << extendedFingers << std::endl; }
			////found 4 extended fingers(1 fist)
			else if (extendedFingers == 4){
				for (int f = 0; f < frame.fingers().count(); f++)
				{
					int extreExtend = 0;
					Leap::Finger finger = frame.fingers()[f];
					if ((finger.isExtended() && finger.type() == 0) && (finger.isExtended() && finger.type() == 1))
						extreExtend++;
				}
			if (extendedFingers != 0)
			{
				if (hand.pinchStrength() > 0.5& hand2.pinchStrength()>0.5)
					std::cout << "Sausage!!!!" << std::endl;
			}
			}
			//found 5 extended fingers(1 fist)
			else if (extendedFingers == 5){ std::cout << "Gestures not detected please try again"<< std::endl; }
			//found 6 extended fingers(1 fist)
			else if (extendedFingers == 6) { std::cout << "Gestures not detected please try again" << extendedFingers << std::endl; }
			//found 7 extended fingers(1 fist)
			else if (extendedFingers == 7) { std::cout << "Gestures not detected please try again" << extendedFingers << std::endl; }
			//found 8 extended fingers(1 fist)
			else if (extendedFingers == 8) { std::cout << "Gestures not detected please try again" << std::endl; }
			//found 9 extended fingers(1 fist)
			else if (extendedFingers == 9) { std::cout <<"Gestures not detected please try again" << extendedFingers << std::endl; }
			//all fingers found
			else if (extendedFingers == 10) {
				for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); hl++)
				{
					if (xBasis.x - xBasis2.x <-1.1)
						std::cout << "Clapped or Yeah" << std::endl;
				}
			}
		}
	}	
}

//initialization
void LeapListener::onInit(const Controller&controller)
{ 
	std::cout << "Initialized" << std::endl;
}

//device disconnected
void LeapListener::onDisconnected(const Controller&controller)
{
	std::cout << "Disconnected" << std::endl;
}
//program existed but still connected
void LeapListener::onExit(const Controller& controller)
{
	std::cout << "Exited" << std::endl;
}
//change device
void LeapListener::onDeviceChange(const Controller& controller)
{
	std::cout << "Device change " << std::endl;
}
