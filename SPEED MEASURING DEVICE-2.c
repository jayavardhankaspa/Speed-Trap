#include<reg51.h>

///////////////////////////functions//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void cct_init(void);
void delay(int);
void lcdinit(void);
void WriteCommandToLCD(int);
void WriteDataToLCD(char);
void ClearLCDScreen(void);
void InitTimer0(void);
void UpdateTimeCounters(void);// use to count the seconds
void DisplaytoLCD(unsigned int);
void UpdateTimeCounters1(void);//this to just count seconds without incrementing the seconds of the seccounter varaible
void Displaystring(void); // to diisplay the string no vehicle
void FloatToLCD(float);


////////////////////////////////////ports///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


sbit RS = P1^0;
sbit E  = P1^1;
sbit start = P3^2; //switched for start line and end line
sbit end = P3^1;


/////////////////////////////variables////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


unsigned int usecCounter = 0;
unsigned int msCounter   = 0;
unsigned int secCounter  = 0;

unsigned int time   = 0;
float speed=0.00;



/////////////////main program//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void main(void)
{
   cct_init(); 
	 lcdinit();              
   InitTimer0();  
	
	while(1)
	{
		
		if (start==0 && end==1)// vehicle starts its journey by entering at the first line
		{
			
	    if(msCounter==0)
				{
					DisplaytoLCD(secCounter-1);
					
				}
					
			UpdateTimeCounters();
		}
		
		else if ( start==0 && end==0)//vehicle reached finish line and hence the speed is caliculated here
		{			
		  
			if(msCounter==0)
				{
					speed=((float)15/(float)secCounter);
					FloatToLCD(speed);	
				}
				UpdateTimeCounters1();
		}
		else//no vehicle for all the combinations
		{			
			if(msCounter==0)
				{
			    Displaystring();
				}
			UpdateTimeCounters1();// function to make the letters visible fora second and then to refresh
		}
		
	}
}




///////////////////////////all modified programs///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FloatToLCD(float fval)
{
	
	 int value;
	char digit;
	ClearLCDScreen();// to clear the scree at the end of eah second and to display the text for 1 sec and then to refresh
	WriteDataToLCD('S');
	WriteDataToLCD('P');
	WriteDataToLCD('E');
	WriteDataToLCD('E');
	WriteDataToLCD('D');
	WriteDataToLCD(':');
  value= fval * 100; // to make the flaot forexample 4.07 to 407 and then dispaly it as charater
	digit = (value/100)%10;
	WriteDataToLCD('0'+digit);				         //Display '4'
	WriteDataToLCD('.');	
	digit = (value/10)%10;                      // Display '.'
	WriteDataToLCD('0'+digit);				         //Display '5'
  digit = (value)%10;
	WriteDataToLCD('0'+digit);				         //Display '7'
	return;
}



void Displaystring(void)
{
	ClearLCDScreen();
  WriteDataToLCD('N');
	WriteDataToLCD('O');
	WriteDataToLCD('-');
	WriteDataToLCD('V');
	WriteDataToLCD('E');
	WriteDataToLCD('H');
	WriteDataToLCD('I');
	WriteDataToLCD('C');
	WriteDataToLCD('L');
	WriteDataToLCD('E');
}




void UpdateTimeCounters1(void)
{
	if (msCounter==1000)
	{
		msCounter=0;
	}
}




void cct_init(void)
{
	P0 = 0x00;  
	P1 = 0x00;
	P2 = 0x00;
}


////////////////////////////////////////////////////// all the timers programs/////////////////////////////////////////////////////////////////////////////////////////////



void InitTimer0(void)
{
	TMOD &= 0xF0;    // Clear 4bit field for timer0
	TMOD |= 0x02;    // Set timer0 in mode 2
	
	TH0 = 0x05;      // 250 usec reloading time
	TL0 = 0x05;      // First time value
	
	ET0 = 1;         // Enable Timer0 interrupts
	EA  = 1;         // Global interrupt enable
	
	TR0 = 1;         // Start Timer 0
}





void Timer0_ISR (void) interrupt 1     // It is called after every 250usec
{
	usecCounter = usecCounter + 250;   // Count 250 usec
	
	if(usecCounter==1000)              // 1000 usec means 1msec 
	{
		msCounter++;
		usecCounter = 0;
	}
TF0 = 0;     // Clear the interrupt flag
}


void UpdateTimeCounters(void)
{
	if (msCounter==1000)
	{
		secCounter++;
		msCounter=0;
	}
}


///////////////////////////////////////////////// all the lcd functions///////////////////////////////////////////////////////////////////



void lcdinit(void)
{
  ///////////// Reset process from datasheet /////////
     delay(15000);

	 P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
	 P2 |= (0x30&0xF0);    // Write 0x3
	
	 E  = 1;               // => E = 1
	 delay(150);
	 E  = 0;               // => E = 0
	 delay(150);

     delay(4500);

	 P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
	 P2 |= (0x30&0xF0);    // Write 0x3
	
	 E  = 1;               // => E = 1
	 delay(150);
	 E  = 0;               // => E = 0
	 delay(150);
		delay(300);

	 P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
	 P2 |= (0x30&0xF0);    // Write 0x3
	
	 E  = 1;               // => E = 1
	 delay(150);
	 E  = 0;               // => E = 0
	 delay(150);

     delay(650);

	 P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
	 P2 |= (0x20&0xF0);    // Write 0x2
	
	 E  = 1;               // => E = 1
	 delay(150);
	 E  = 0;               // => E = 0
	 delay(150);

	 delay(650);

   WriteCommandToLCD(0x28);    //function set
   WriteCommandToLCD(0x0c);    //display on,cursor off,blink off
   WriteCommandToLCD(0x01);    //clear display
   WriteCommandToLCD(0x06);    //entry mode, set increment
}

void ClearLCDScreen(void)
{
	WriteCommandToLCD(0x01);   // Clear screen command
	delay(1000);
}


void DisplaytoLCD(unsigned int s )   // Displaying intezer
{
	ClearLCDScreen();
	WriteDataToLCD('T');
	WriteDataToLCD('I');
	WriteDataToLCD('M');
	WriteDataToLCD('E');
	WriteDataToLCD(':');
	WriteDataToLCD( (s/10)+0x30 );
	WriteDataToLCD( (s%10)+0x30 );
}



void WriteDataToLCD(char t)

{
   RS = 1;             // This is data

   P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
   P2 |= (t&0xF0);     // Write Upper nibble of data

   E  = 1;             // => E = 1
   delay(150);
   E  = 0;             // => E = 0
   delay(150);

   P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
   P2 |= ((t<<4)&0xF0);// Write Lower nibble of data

   E  = 1;             // => E = 1
   delay(150);
   E  = 0;             // => E = 0
   delay(150);
}


void WriteCommandToLCD(int z)
{
   RS = 0;             // This is command

   P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
   P2 |= (z&0xF0);     // Write Upper nibble of data

   E  = 1;             // => E = 1
   delay(150);
   E  = 0;             // => E = 0
   delay(150);

   P2 &= 0x0F;		   // Make P2.4 to P2.7 zero
   P2 |= ((z<<4)&0xF0);// Write Lower nibble of data

   E  = 1;             // => E = 1
   delay(150);
   E  = 0;             // => E = 0
   delay(150);
}

void delay(int a)
{
   int i;
   for(i=0;i<a;i++);   //null statement
}

