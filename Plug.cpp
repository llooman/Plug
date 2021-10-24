#include "Arduino.h"
#include "Plug.h"

//#define DEBUG

void Plug::initTimers(int count)
{
	for(int i=0; i<count; i++){
		timers[i]=0L;
	}

  	nextTimer(PLUG_UPLOAD_BUTTON_TIMER, 1); 
	nextTimer(PLUG_UPLOAD_RELAIS_TIMER, 2); 
	nextTimer(PLUG_UPLOAD_LED_TIMER, 3); 
}

void Plug::loop()
{
	if( isTime(PLUG_UPLOAD_RELAIS_TIMER)){
         upload(propId );  
    } else if( isTime(PLUG_UPLOAD_BUTTON_TIMER)){
        upload(propId+1); 
    }  
    // } else if( isTime(PLUG_UPLOAD_LED_TIMER)){
	// 	 upload(propId+2); 
    // }


	if( buttonDown()){

		if(!buttonPressed){

			nextTimerMillis(BUTTON_TIMER_SHORT, 100);
			nextTimerMillis(BUTTON_TIMER_LONG, 1000);

			buttonPressed = true;
		}

	} else if(buttonPressed){

		if( isTime(BUTTON_TIMER_SHORT)
		 && ! isTime(BUTTON_TIMER_LONG)
		){
			digitalWrite(relaisPin, !digitalRead(relaisPin));
		}

		if( isTime(BUTTON_TIMER_LONG)
		){
			buttonPressedLong = true;
		}
		
		buttonPressed = false;
		timerOff(BUTTON_TIMER_SHORT);
		timerOff(BUTTON_TIMER_LONG);
	}
 

	if( relaisUploaded !=  digitalRead(relaisPin)
	){
		nextTimer(PLUG_UPLOAD_RELAIS_TIMER, 0);
	}
	if( buttonUploaded != buttonDown()
	){
		nextTimer(PLUG_UPLOAD_BUTTON_TIMER, 0);
	}
	// if( ledUploaded != digitalRead(ledPin)
	// ){
 	// 	// nextTimer(PLUG_UPLOAD_LED_TIMER, 0);
	// }
}

int Plug::upload(int id)
{
	if(uploadFunc==0 ) return 0;
 
	if(id==propId ){

		relaisUploaded = digitalRead(relaisPin);
		uploadFunc(id, relaisUploaded, 0 );
		nextTimer(PLUG_UPLOAD_RELAIS_TIMER); 

	} else if( id==(propId+1) ){

		buttonUploaded = buttonDown();
		uploadFunc(id,  buttonUploaded, 0 );
		nextTimer(PLUG_UPLOAD_BUTTON_TIMER);

	} else if( id==(propId+2) ){

		ledUploaded = digitalRead(ledPin)!= ledInverted;
		// uploadFunc(id, ledUploaded, 0 );
		nextTimer(PLUG_UPLOAD_LED_TIMER);
	}

	return 0;
}
 
void Plug::setup( int state )
{
	pinMode(relaisPin , OUTPUT);
	digitalWrite(relaisPin , state);

	pinMode(ledPin , OUTPUT);
	digitalWrite(ledPin , ledInverted?HIGH:LOW);

	if(buttonPin!=0)
	{
		pinMode(buttonPin, INPUT_PULLUP);
	}
}
int Plug::state( void )
{
	return digitalRead(relaisPin)?1:0;
}
//    void setup(int buttonPin, int relaisPin, int ledPin )
void Plug::setup(  )
{
	pinMode(relaisPin , OUTPUT);
	digitalWrite(relaisPin , LOW);

	pinMode(ledPin , OUTPUT);
	digitalWrite(ledPin , ledInverted?HIGH:LOW);

	if(buttonPin!=0)
	{
		pinMode(buttonPin, INPUT_PULLUP);
	}
}

int Plug::setVal( int id, long val ) //bool sending, int id,
{
	// Serial.print("plug.set-");Serial.println(id);

	if(id==propId ){
		
		digitalWrite(relaisPin , val>0?HIGH:LOW);

	} else if( id==(propId+1) ){

	} else if( id==(propId+2) ){

		// digitalWrite(ledPin , val!=ledInverted?HIGH:LOW);   // LED inverted control
	}

 
	return 0;
}


bool Plug::isTime( int id){
	if(timers[id] == 0L) return false;

	if( millis() > timers[id]) return true;
	return (timers[id] -  millis() ) >  0x0fffffff;  
}
bool Plug::isTimerActive( int id ){
	return timers[id] > 0;
}
bool Plug::isTimerInactive( int id ){
	return timers[id] == 0;
}
void Plug::timerOff( int id ){
	timers[id]=0;
}

void Plug::nextTimerMillis( int id, unsigned long periode){

	if(periode<0) periode=0;
	timers[id] = millis() + periode;
	if(timers[id]==0) timers[id]=1;
}

void Plug::trace(const char* id)
{
	Serial.print(F("@ "));
	Serial.print(millis()/1000);
	Serial.print(F(" "));Serial.print(id);
	Serial.print(F(", button="));	Serial.print(!digitalRead(buttonPin));
	Serial.print(F(", relais="));	 Serial.print(digitalRead(relaisPin));
	Serial.print(F(", button="));	Serial.print( buttonPressed );
	Serial.print(F(", led="));	 Serial.print(   digitalRead(ledPin) != ledInverted);

	Serial.println();
	Serial.flush();
}
