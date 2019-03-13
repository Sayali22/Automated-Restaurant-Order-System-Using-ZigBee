//**********************************************************************************************************//
/************************************** I N C L U D E S ****************************************************/
//**********************************************************************************************************//
#include<p18f4550.h>
#define en PORTDbits.RD1
#define rw PORTDbits.RD2
#define rs PORTDbits.RD3
#define k1 PORTBbits.RB0
#define k2 PORTBbits.RB1
#define k3 PORTBbits.RB2
#define k4 PORTBbits.RB3
//**********************************************************************************************************//
/**************************************** Configuration Code *************************************************/
//**********************************************************************************************************//
extern void _startup (void);
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000

void _reset (void)
{
	_asm goto _startup _endasm
}

#pragma code
#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_ISR (void)
{
}

#pragma code
#pragma code _LOW_INTERRUPT_VECTOR = 0x1018 
void low_ISR (void)
{
}
#pragma code
//**********************************************************************************************************//
/**************************************** V A R I A B L E S *************************************************/
//**********************************************************************************************************//
	unsigned int i=0,j;
	unsigned char line1[]={" AUTOMATIC RESTORENT"};
	unsigned char line2[]={"     MENU ORDER     "};
	unsigned char line3[]={"       SYSTEM       "};
	unsigned char cmd1[]= {"TABLE NO:           "};
	unsigned char menu1[]= {"1:                 "};
	unsigned char menu2[]= {"2:        	    "};
	unsigned char menu3[]= {"3:                 "};
	unsigned char menu4[]= {"4:                 "};
	unsigned char menu5[]= {"5:                 "};
	unsigned char menu6[]= {"6:                 "};
	unsigned char menu7[]= {"7:                 "};
	unsigned char menu8[]= {"8:                 "};

	unsigned char ID[];
	unsigned char temp;
//**********************************************************************************************************//
/******************************** P R I V A T E  P R O T O T Y P E S ***************************************/
//**********************************************************************************************************//
void delay_ms (unsigned int time)
{
	unsigned int i, j;
    for (i = 0; i < time; i++)
    	for (j = 0; j < 135; j++);
}

void cmd(unsigned char c)
{
	PORTD=(unsigned char)(c & 0xF0);
	rs=0;
	rw=0;
	en=1;
	delay_ms(10);
	en=0;

	c=(unsigned char)(c<<4);
	PORTD=(unsigned char)(c & 0xF0);
	rs=0;
	rw=0;
	en=1;
	delay_ms(10);
	en=0;
}

void data(unsigned char c)
{
	PORTD=(unsigned char)(c & 0xF0);
	rs=1;
	rw=0;
	en=1;
	delay_ms(10);
	en=0;

	c=(unsigned char)(c<<4);
	PORTD=(unsigned char)(c & 0xF0);
	rs=1;
	rw=0;
	en=1;
	delay_ms(10);
	en=0;
}
void lcd_init(void)
{
	cmd(0x02);		// to return home
	cmd(0x28);		//to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	cmd(0x0C);		//display on cursor off
	cmd(0x01);
	cmd(0x80);
	cmd(0x06);		//entry mode
	delay_ms(100);
}

void UARTinit(void)
{
	unsigned char temp;
	SPBRG = 0xE1;
 	SPBRGH = 0x04;      // 0x0271 for 48MHz -> 19200 baud XTAL=20MHz, Fosc=48Mhz 
 	TXSTA = 0xA4;       // TX enable BRGH=1
 	RCSTA = 0xB0;       // continuous RX
 	BAUDCON = 0x08;     // BRG16 = 1
 	temp = RCREG;       // Empty buffer
 	temp = RCREG;       // Empty buffer
}

void transmit(char data)
{
    while(TXSTAbits.TRMT==0);		//wait tiiltransmit buffer in not empty
    TXREG = data;					// Transmit Data
}

unsigned char received(void)
{
	while(PIR1bits.RCIF==0); // Wait until RCIF gets low
	return RCREG; // Store data in Reception register
}
void keycall(void)
{
	while(1)
	{
		PORTB=0X7F;
		delay_ms(100);
		temp=PORTB;
		delay_ms(100);
		temp=temp&0x0f;
		if(temp==0x0E)
		{
			i='A';
			break;
		}
	 	if(temp==0x0D)
		{
			i='B';
			break;
		}
		if(temp==0x0B)
		{
			i='C';
			break;
		}
		if(temp==0x07)
		{
			i='D';
			break;
		}
	

		PORTB=0XBF;
		delay_ms(100);
		temp=PORTB;
		delay_ms(100);
		temp=temp&0x0f;

		if(temp==0x0E)
		{
			i='9';
			break;
		}
	 	if(temp==0x0D)
		{
			i='6';
			break;
		}
		if(temp==0x0B)
		{
			i='3';
			break;
		}
		if(temp==0x07)
		{
			i='=';
			break;
		}
	
		PORTB=0XDF;
		delay_ms(100);
		temp=PORTB;
		delay_ms(100);
		temp=temp&0x0f;

		if(temp==0x0E)
		{
			i='8';
			break;
		}
	 	if(temp==0x0D)
		{
			i='5';
			break;
		}
		if(temp==0x0B)
		{
			i='2';
			break;
		}
		if(temp==0x07)
		{
			i='0';
			break;
		}
	
		PORTB=0XEF;
		delay_ms(100);
		temp=PORTB;
		delay_ms(100);
		temp=temp&0x0f;

		if(temp==0x0E)
		{
			i='7';
			break;
		}
		if(temp==0x0D)
		{
			i='4';
			break;
		}
		if(temp==0x0B)
		{
			i='1';
			break;
		}
		if(temp==0x07)
		{
			i='*';
			break;
		}	
	}
}
//**********************************************************************************************************//
/******************************** P R I V A T E  P R O T O T Y P E S ***************************************/
//**********************************************************************************************************//

void main()
{
	TRISB=0x0F;
	TRISD=0x00;
	TRISC=0x80;
	TRISE=0xFF;
	TRISA=0x0F;	
	PORTB=0xFF;
	PORTC=0x00;
	PORTE=0x00;
	INTCON2bits.RBPU=0;
	ADCON1 = 0X0F;
	lcd_init();	
	UARTinit();

loop:	cmd(0x80);
	delay_ms(10);
	for(i=0;i<20;i++)
	{
		data(line1[i]);
	}
	cmd(0xC0);
	delay_ms(10);
	for(i=0;i<20;i++)
	{
		data(line2[i]);
	}
	cmd(0x94);
	delay_ms(10);
	for(i=0;i<20;i++)
	{
		data(line3[i]);
	}
	delay_ms(2000);
	cmd(0xD4);
	for(i=0;i<20;i++)
	{
		data(cmd1[i]);
		transmit(cmd1[i]);
	}
	cmd(0xDD);
	for (j=0;j<2;j++)
	{
		keycall();
		data(i);
		transmit(i);
		delay_ms(100);
	}
	while(1)
	{
		keycall();
		if(i=='=')
		{
			break;
		}
	}
//******************************************************//
	while(1)
	{	
		keycall();
		if(i=='1')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu1[i]);
				transmit(menu1[i]);
				delay_ms(100);
			}
		}	
		if(i=='2')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu2[i]);
				transmit(menu2[i]);
				delay_ms(100);
			}
		}	
		if(i=='3')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu3[i]);
				transmit(menu3[i]);
				delay_ms(100);
			}
		}	
		if(i=='4')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu4[i]);
				transmit(menu4[i]);
				delay_ms(100);
			}
		}	
		if(i=='5')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu5[i]);
				transmit(menu5[i]);
				delay_ms(100);
			}
		}	
		if(i=='6')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu6[i]);
				transmit(menu6[i]);
				delay_ms(100);
			}
		}	
		if(i=='7')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu7[i]);
				transmit(menu7[i]);
				delay_ms(100);
			}
		}	
		if(i=='8')
		{
			cmd(0X01);
			cmd(0x80);
			for (j=0;j<20;j++)
			{
				data(menu8[i]);
				transmit(menu8[i]);
				delay_ms(100);
			}
		}	
		if(i=='=')
		{
			cmd(0X01);
			cmd(0x80);
			data('D');
			data('O');
			data('N');
			data('E');
			delay_ms(3000);
			goto loop;
		}	

	}
}


