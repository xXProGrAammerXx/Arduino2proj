#include "EnumThings.h"
#include <SPI.h>
#include <RFID.h>

////RFID Sftuff//////////////////////////////
RFID RFIDReader( 10, 9 );
int UID[ 5 ];
int NightCard[ 5 ] = { 149,195,250,194,110 };
int DayCard[ 5 ] = { 208,226,169,27,128 };
RFIDStatus status = RFIDStatus::Day;
/////////////////////////////////////////////

void setup()
{
	Serial.begin( 115200 );
	SPI.begin();
	RFIDReader.init();
}

void loop()
{
	if ( RFIDReader.isCard() ) 
	{
		if ( RFIDReader.readCardSerial() )
		{
			for ( int i = 0; i <= 4; i++ )
			{
				UID[ i ] = RFIDReader.serNum[ i ];
			}
			if ( UID[ 0 ] == NightCard[ 0 ]
				&& UID[ 1 ] == NightCard[ 1 ]
				&& UID[ 2 ] == NightCard[ 2 ]
				&& UID[ 3 ] == NightCard[ 3 ]
				&& UID[ 4 ] == NightCard[ 4 ] )
			{
				status = RFIDStatus::Night;
			}
			else if ( UID[ 0 ] == DayCard[ 0 ]
				&& UID[ 1 ] == DayCard[ 1 ]
				&& UID[ 2 ] == DayCard[ 2 ]
				&& UID[ 3 ] == DayCard[ 3 ]
				&& UID[ 4 ] == DayCard[ 4 ] )
			{
				status = RFIDStatus::Day;
			}
		}
		if ( status == RFIDStatus::Night )
		{
			Serial.print( "$0" );
		}
		else if ( status == RFIDStatus::Day )
		{
			Serial.print( "$1" );
		}
		RFIDReader.halt();
	}
	delay( 1000 );
}
