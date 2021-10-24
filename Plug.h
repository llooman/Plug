#ifndef Plug_H
#define Plug_H

//		 val     inverted
//		true	 true       val == !inverted = false
//		true	 false	    val == !inverted = true
//		false    true       val == !inverted = true
//		false    false      val == !inverted = false

#define PLUG_TIMER_COUNT 5
#define BUTTON_TIMER_SHORT 0
#define BUTTON_TIMER_LONG 1
#define PLUG_UPLOAD_BUTTON_TIMER 2
#define PLUG_UPLOAD_RELAIS_TIMER 3
#define PLUG_UPLOAD_LED_TIMER 4


class Plug
{
  public:
   	unsigned long 	timers[PLUG_TIMER_COUNT];

	int propId=0;
	int buttonPin=0;   // button high = unpressed
	int relaisPin=0;
	int ledPin=0;

	bool ledInverted = true;

	bool buttonUploaded = false;
	bool ledUploaded = false;
	bool relaisUploaded = false;


	bool buttonPressed = false;
	bool buttonPressedLong = false;

 

	int (*uploadFunc) (int id, long val, unsigned long timeStamp) = 0;
    void onUpload( int (*function)(int id, long val, unsigned long timeStamp) )
    {
    	uploadFunc = function;
    }
	bool buttonDown(){return ! digitalRead(buttonPin);}
 

	virtual ~Plug(){}
	Plug( )
	{
	}

	Plug(int id, int buttonPin, int relaisPin, int ledPin )
	{
		this->propId = id;
		this->buttonPin = buttonPin;
		this->relaisPin = relaisPin;
		this->ledPin = ledPin;
	}

	Plug(int id, int buttonPin, int relaisPin, int ledPin, bool ledInverted )
	{
		this->propId = id;
		this->buttonPin = buttonPin;
		this->relaisPin = relaisPin;
		this->ledPin = ledPin;
		this->ledInverted = ledInverted;
	}

 
	void initTimers(int count);
	bool isTime( int id);
	bool isTimerActive( int id );
	bool isTimerInactive( int id );
	void nextTimer( int id ){ nextTimerMillis(id, 60000L );}	
	void nextTimer( int id, int periode){ nextTimerMillis(id, periode * 1000L );}	
	void nextTimerMillis( int id, unsigned long periode);
	void timerOff( int id );

	void trace(const char* id);
	int setVal( int id, long val );
	int upload(int id);	
	void setup(void);	
	void setup(int state);	
	int state();
	void loop(void);
	bool active(void){return digitalRead(relaisPin); }
};


#endif
