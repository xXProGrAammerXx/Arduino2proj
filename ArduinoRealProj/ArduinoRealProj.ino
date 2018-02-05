#include "EnumThings.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

////Water Level Stuff/////
int readPin = A0;
int value;
bool waterLBL = false;
/*-----------------------*/
int offSet = 300;
/*----------------------*/
int downReadPin = A3;
int downValue;
bool waterLBDL = false;
/////////////////////////

////Servo stuff//////////
/*-------1st Servo---------*/
Servo servo;
int servoInitialPosition = 0;
int servoOffset = 0;
int servoPos = servoInitialPosition + servoOffset;
bool didServoTurn = true; // true = Closed & false = Opened | I dont know why i didn't an enum class but it's too late now
int turningSpeed = 5;
/*--------------------------*/

/*--------2nd servo---------*/
Servo servo2;
int servo2Pos = 5;
int turningSpeed2 = 5;
DayNight currentTime = DayNight::Day;
Servo2Status servo2status = Servo2Status::Close;
/*--------------------------*/
/////////////////////////

////LCD Stuff/////////////
LiquidCrystal_I2C lcd( 0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE );
LCDwriting writing = LCDwriting::None;
LCDwritingL2 writingl2 = LCDwritingL2::None;
LCDFS l1 = LCDFS::Neutral;
//////////////////////////

////Analog JoyStick Stuff////
int clickPin = 2;
int xPin = A1;
int yPin = A2;
int xValue;
int yValue;
bool isJSClicked;
bool isShutDown = false;
ON_Off status = ON_Off::ON;
Forced forced = Forced::Neutral;
/////////////////////////////

////LEDs Stuff/////////////////
int RGBbluePin = 4;
int RGBgreenPin = 7;
int RGBredPin = 8;
/*----------------------------*/
int yellowPin = 9;
int bluePin = 12;
//////////////////////////////

void setup()
{
	Serial.begin( 115200 );
	////WaterLevelSensoring Stuff///
	pinMode( readPin, INPUT );
	pinMode( downReadPin, INPUT );
	////////////////////////////////

	///////Servo Stuff/////////////
	/*----------------------------------Servo 1 Stuff----------------------------------------*/
	servo.attach( 3 );
	//======================================================================================//
	for ( servoPos = 5; servoPos >= 185; servoPos -= turningSpeed ) // goes from 185 degrees to 5 degrees
	{
		servo.write( servoPos ); // tell servo to go to position in variable 'servoPos'
		delay( 15 ); // waits 15ms for the servo to reach the position
	}
	//======================================================================================//
	/*--------------------------------------------------------------------------------------*/

	/*---------------------------------Servo 2 Stuff----------------------------------------*/
	servo2.attach( 5 );
	//======================================================================================//
	for ( servo2Pos = 5; servo2Pos <= 185; servo2Pos += turningSpeed2 )
	{ // goes from 5 degrees to 185 degrees in steps of 1 degree 
		servo2.write( servo2Pos ); // tell servo to go to position in variable 'servo2Pos'
		delay( 15 ); // waits 15ms for the servo to reach the position
	}
	//======================================================================================//
	/*--------------------------------------------------------------------------------------*/
	///////////////////////////////


	////Analog JoyStick Stuff///////////
	pinMode( clickPin, INPUT_PULLUP );
	pinMode( xPin, INPUT );
	pinMode( yPin, INPUT );
	////////////////////////////////////


	////LEDs Stuff///////////////////////
	pinMode( RGBbluePin, OUTPUT );
	pinMode( RGBgreenPin, OUTPUT );
	pinMode( RGBredPin, OUTPUT );
	digitalWrite( RGBredPin, HIGH );
	digitalWrite( RGBgreenPin, HIGH );
	digitalWrite( RGBbluePin, HIGH );

	pinMode( yellowPin, OUTPUT );
	pinMode( bluePin, OUTPUT );
	digitalWrite( yellowPin, HIGH );
	digitalWrite( bluePin, HIGH );
	///////////////////////////////////


	////LCD Stuff///////////////////
	lcd.begin( 16, 2 );
	lcd.setCursor( 0, 0 );
	//================================================================================================//
	for ( int i = 0; i <= 3; i++ )
	{
		lcd.write( "Loading." );
		delay( 375 );
		lcd.clear();
		lcd.write( "Loading.." );
		delay( 375 );
		lcd.clear();
		lcd.write( "Loading..." );
		delay( 375 );
		lcd.clear();
	} //this for loop gives the arduino a little bit of time to tha Arduino so that it could be stable
	//================================================================================================//
	lcd.setCursor( 4, 0 );
	lcd.write( "Welcome!" );
	delay( 2000 );
	lcd.clear();
	////////////////////////////////
}

void loop()
{
	/////////Analog JoyStick stuff///////////////
	xValue = analogRead( xPin );
	yValue = analogRead( yPin );
	isJSClicked = ( !digitalRead( clickPin ) );
	/////////////////////////////////////////////


	if ( status == ON_Off::ON )
	{
		////////On-Off Checks//////////////////////
		if ( yValue >= 700 )
		{
			status = ON_Off::OFF;
		}
		if ( xValue <= 100 )
		{
			forced = Forced::Close;
		}
		else if ( xValue >= 700 )
		{
			forced = Forced::Open;
		}
		else if ( isJSClicked == true )
		{
			forced = Forced::Neutral;
		}
		///////////////////////////////////////////

		////////Water Sensoring Stuff////////////
		value = analogRead( readPin );

		value -= offSet;
		/*=============================================*/
		if ( value <= 0 )
		{
			waterLBL = false;
		}
		else
		{
			waterLBL = true;
		}
		/*==============================================*/

		/*------------------------------------------------*/
		/*------------------------------------------------*/

		downValue = analogRead( downReadPin );
		downValue -= offSet;
		/*=============================================*/
		if ( downValue <= 0 )
		{
			waterLBDL = false;
		}
		else
		{
			waterLBDL = true;
		}
		/*==============================================*/
		///////////////////////////////////////////////////

		//////LEDs Stuff/////////////////////////////////////
		if ( forced == Forced::Neutral )
		{
			digitalWrite( RGBredPin, HIGH );
			digitalWrite( RGBgreenPin, LOW );
		}
		else
		{
			digitalWrite( RGBgreenPin, HIGH );
			digitalWrite( RGBredPin, LOW );
		}

		if ( currentTime == DayNight::Day )
		{
			digitalWrite( yellowPin, LOW );
			digitalWrite( bluePin, HIGH );
		}
		else if ( currentTime == DayNight::Night )
		{
			digitalWrite( yellowPin, HIGH );
			digitalWrite( bluePin, LOW );
		}
		////////////////////////////////////////////////////////

		////////////Servo Stuff + LCD writing/////////////////////
		if ( forced == Forced::Neutral )
		{
			if ( waterLBL == true && didServoTurn == false )
			{
				lcd.clear();
				lcd.setCursor( 0, 0 );
				lcd.write( "M1 : Closing" );
				if ( servo2status == Servo2Status::Close )
				{
					lcd.setCursor( 0, 1 );
					lcd.write( "M2 : Closed" );
					writingl2 = LCDwritingL2::Close;
				}
				else if ( servo2status == Servo2Status::Open )
				{
					lcd.setCursor( 0, 1 );
					lcd.write( "M2 : Opened" );
					writingl2 = LCDwritingL2::Open;
				}
				for ( servoPos = 5; servoPos <= 185; servoPos += turningSpeed )
				{ // goes from 5 degrees to 185 degrees in steps of 1 degree 
					servo.write( servoPos ); // tell servo to go to position in variable 'servoPos'
					delay( 15 ); // waits 15ms for the servo to reach the position
				}
				didServoTurn = true;
				lcd.clear();
			}
			else if ( waterLBDL == false && didServoTurn == true )
			{
				lcd.clear();
				lcd.setCursor( 0, 0 );
				lcd.write( "M1 : Opening" );
				if ( servo2status == Servo2Status::Close )
				{
					lcd.setCursor( 0, 1 );
					lcd.write( "M2 : Closed" );
					writingl2 = LCDwritingL2::Close;
				}
				else if ( servo2status == Servo2Status::Open )
				{
					lcd.setCursor( 0, 1 );
					lcd.write( "M2 : Opened" );
					writingl2 = LCDwritingL2::Open;
				}
				for ( servoPos = 185; servoPos >= 5; servoPos -= turningSpeed ) // goes from 185 degrees to 5 degrees
				{
					servo.write( servoPos ); // tell servo to go to position in variable 'servoPos'
					delay( 15 ); // waits 15ms for the servo to reach the position
				}
				didServoTurn = false;
				lcd.clear();
			}
		}
		else if ( forced == Forced::Close && didServoTurn == false )
		{
			lcd.clear();
			lcd.setCursor( 0, 0 );
			lcd.write( "M1 : Closing(F)" );
			if ( servo2status == Servo2Status::Close )
			{
				lcd.setCursor( 0, 1 );
				lcd.write( "M2 : Closed" );
				writingl2 = LCDwritingL2::Close;
			}
			else if ( servo2status == Servo2Status::Open )
			{
				lcd.setCursor( 0, 1 );
				lcd.write( "M2 : Opened" );
				writingl2 = LCDwritingL2::Open;
			}
			for ( servoPos = 5; servoPos <= 185; servoPos += turningSpeed )
			{ // goes from 5 degrees to 185 degrees in steps of 1 degree 
				servo.write( servoPos ); // tell servo to go to position in variable 'servoPos'
				delay( 15 ); // waits 15ms for the servo to reach the position
			}
			didServoTurn = true;
			lcd.clear();
		}
		else if ( forced == Forced::Open && didServoTurn == true )
		{
			lcd.clear();
			lcd.setCursor( 0, 0 );
			lcd.write( "M1 : Opening(F)" );
			if ( servo2status == Servo2Status::Close )
			{
				lcd.setCursor( 0, 1 );
				lcd.write( "M2 : Closed" );
				writingl2 = LCDwritingL2::Close;
			}
			else if ( servo2status == Servo2Status::Open )
			{
				lcd.setCursor( 0, 1 );
				lcd.write( "M2 : Opened" );
				writingl2 = LCDwritingL2::Open;
			}
			for ( servoPos = 185; servoPos >= 5; servoPos -= turningSpeed ) // goes from 185 degrees to 5 degrees
			{
				servo.write( servoPos ); // tell servo to go to position in variable 'servoPos'
				delay( 15 ); // waits 15ms for the servo to reach the position
			}
			didServoTurn = false;
			lcd.clear();
		}
		///////////////////////////////////////////////////////////


		////Servo2 Stuff + LCD Stuff///////////////////////////////////////
		if ( Serial.read() == '$' )
		{
			int temp;
			delay( 1 );
			temp = Serial.read();
			if ( temp == 49 )
			{
				currentTime = DayNight::Day;
			}
			else if ( temp == 48 )
			{
				currentTime = DayNight::Night;
			}
		}

		if ( currentTime == DayNight::Day && servo2status == Servo2Status::Open )
		{
			lcd.clear();
			lcd.setCursor( 0, 0 );

			/*------------------------------------------------------------------------------*/
			if ( didServoTurn == true && forced == Forced::Neutral )
			{
				lcd.write( "M1 : Closed" );
			}
			else if ( didServoTurn == false && forced == Forced::Neutral )
			{
				lcd.write( "M1 : Opened" );
			}
			else if ( forced == Forced::Close )
			{
				lcd.write( "M1 : Closed (F)" );
			}
			else if ( forced == Forced::Open )
			{
				lcd.write( "M1 : Opened (F)" );
			}
			/*------------------------------------------------------------------------------*/

			lcd.setCursor( 0, 1 );
			lcd.write( "M2 : Closing" );

			for ( servo2Pos = 5; servo2Pos <= 185; servo2Pos += turningSpeed2 )
			{ // goes from 5 degrees to 185 degrees in steps of 1 degree 
				servo2.write( servo2Pos ); // tell servo to go to position in variable 'servo2Pos'
				delay( 15 ); // waits 15ms for the servo to reach the position
			}

			lcd.clear();

			servo2status = Servo2Status::Close;
		}
		else if ( currentTime == DayNight::Night && servo2status == Servo2Status::Close )
		{
			lcd.clear();
			lcd.setCursor( 0, 0 );

			/*------------------------------------------------------------------------------*/
			if ( didServoTurn == true && forced == Forced::Neutral )
			{
				lcd.write( "M1 : Closed" );
			}
			else if ( didServoTurn == false && forced == Forced::Neutral )
			{
				lcd.write( "M1 : Opened" );
			}
			else if ( forced == Forced::Close )
			{
				lcd.write( "M1 : Closed (F)" );
			}
			else if ( forced == Forced::Open )
			{
				lcd.write( "M1 : Opened (F)" );
			}
			/*------------------------------------------------------------------------------*/

			lcd.setCursor( 0, 1 );
			lcd.write( "M2 : Opening" );

			for ( servo2Pos = 185; servo2Pos >= 5; servo2Pos -= turningSpeed2 ) // goes from 185 degrees to 5 degrees
			{
				servo2.write( servo2Pos ); // tell servo to go to position in variable 'servoPos'
				delay( 15 ); // waits 15ms for the servo to reach the position
			}
			servo2status = Servo2Status::Open;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////

		///////////////LCD Stuff//////////////////////////////////////////////////////////////////////////////////
		if ( ( didServoTurn == true && ( writing == LCDwriting::Open || writing == LCDwriting::None ) )
			|| ( didServoTurn == false && ( writing == LCDwriting::Close || writing == LCDwriting::None ) )
			|| ( ( forced == Forced::Close || forced == Forced::Open ) && l1 == LCDFS::Neutral )
			|| ( forced == Forced::Neutral && l1 == LCDFS::Forced )
			|| ( servo2status == Servo2Status::Close && ( writingl2 == LCDwritingL2::Open || writingl2 == LCDwritingL2::None ) )
			|| servo2status == Servo2Status::Open && ( writingl2 == LCDwritingL2::Close || writingl2 == LCDwritingL2::None ) )
		{
			lcd.clear();
			lcd.setCursor( 0, 0 );
			if ( didServoTurn == false )
			{
				if ( forced == Forced::Neutral )
				{
					lcd.write( "M1 : Opened" );
					writing = LCDwriting::Open;
					l1 = LCDFS::Neutral;
				}
				else if ( forced == Forced::Open )
				{
					lcd.write( "M1 : Opened (F)" );
					writing = LCDwriting::Open;
					l1 = LCDFS::Forced;
				}
			}
			else if ( didServoTurn == true )
			{
				if ( forced == Forced::Neutral )
				{
					lcd.write( "M1 : Closed" );
					writing = LCDwriting::Close;
					l1 = LCDFS::Neutral;
				}
				else if ( forced == Forced::Close )
				{
					lcd.write( "M1 : Closed (F)" );
					writing = LCDwriting::Close;
					l1 = LCDFS::Forced;
				}
			}

			lcd.setCursor( 0, 1 );
			if ( servo2status == Servo2Status::Close )
			{
				lcd.write( "M2 : Closed" );
				writingl2 = LCDwritingL2::Close;
			}
			else if ( servo2status == Servo2Status::Open )
			{
				lcd.write( "M2 : Opened" );
				writingl2 = LCDwritingL2::Open;
			}
		}
		///////////////////////////////////////////////////////////////
	}
	else if ( status == ON_Off::OFF )
	{
		lcd.clear();
		writing = LCDwriting::None;
		//shut down the RGB
		digitalWrite( RGBredPin, HIGH );
		digitalWrite( RGBgreenPin, HIGH );
		digitalWrite( RGBbluePin, HIGH );
		///////////////////
		if ( yValue <= 100 )
		{
			status = ON_Off::ON;
		}
	}

	/*<DEBUG>*/
	/*</DUBUG>*/
}