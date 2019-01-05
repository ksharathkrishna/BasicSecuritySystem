//Secuirity system using 8051 micro controller


#include<reg52.h>
#include<intrins.h>
typedef unsigned char tByte;
typedef unsigned int tWord;

typedef unsigned char tbyte;
typedef unsigned int tword;

sbit RS = P1^4;
sbit RW = P1^5;
sbit E = P1^6;
#define LCDData P0


sbit keyopen = P3^2;
sbit keyclose = P3^3;
sbit survel = P3^4;

sbit DAT = P0^0;
sbit CLK = P0^1;

sbit PUL = P3^7;
sbit relay = P1^7;

sbit buzz = P2^6;

sbit row0 = P0^0;
sbit row1 = P0^1;
sbit row2 = P0^2;
sbit row3 = P0^3;

sbit col0 = P1^3;
sbit col1 = P1^2;
sbit col2 = P1^1;
sbit col3 = P1^0;


tbyte row,col;

tbyte table[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x98}; 
tbyte code keys[4][4] = {{'0','1','2','3'},{'4','5','6','7'},{'8','9','e','r'},{'z','z','z','z'}};

void LCD_DispStr(tByte line_no, char* str);
void LCD_Init(void);
void LCD_Command(tByte command);
void LCD_Data(tByte databyte);
void enpulse(void);
void delay1(tByte val);
	
void LCD_DispStr(tByte line_no, char* str)
{
    tByte i=0;
    if(line_no==1)
        LCD_Command(0x80);
    else
        LCD_Command(0xc0);
    
    while(str[i]!='\0')
    {
        LCD_Data(str[i]); i++;
        if(i==16) break;
    }
}

void LCD_Init(void)
{
    LCD_Command(0x38);
    LCD_Command(0x0c);
    LCD_Command(0x01);
}

void LCD_Command(tByte command)
{
    RS=0;
    RW=0;
    LCDData = command;
    enpulse();
    delay1(50);
}

void LCD_Data(tByte databyte)
{
    RS=1;
    RW=0;
    LCDData = databyte;
    enpulse();
    delay1(50);
}

void enpulse(void)
{
    E=1;
    delay1(2);
    E=0;
    delay1(2);
}

void delay1(tByte val)
{
    tByte i;
    for(i=0;i<val;i++)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

	
void delay(tword x)
{
	tword i;
	while(x--)
		for(i=0;i<300;i++);
}

void write(tbyte x)
{
	tbyte i;
	for(i=0;i<8;i++)
	{
		if(x & (0x80>>i))
			DAT =1;
		else 
			DAT =0;
		CLK =0;
		CLK =1;
	}
}

tbyte read()
{
	tbyte value;
        while(1)
        {
            row0=0;row1=1;row2=1;row3=1;row=0;
            if(col0==0){col=0;break;}
            if(col0==0){col=1;break;}
            if(col0==0){col=2;break;}
            if(col0==0){col=3;break;}

            row0=1;row1=0;row2=1;row3=1;row=1;
            if(col0==0){col=0;break;}
            if(col0==0){col=1;break;}
            if(col0==0){col=2;break;}
            if(col0==0){col=3;break;}

            row0=1;row1=1;row2=0;row3=1;row=2;
            if(col0==0){col=0;break;}
            if(col0==0){col=1;break;}
            if(col0==0){col=2;break;}
            if(col0==0){col=3;break;}

            row0=1;row1=1;row2=1;row3=0;row=3;
            if(col0==0){col=0;break;}
            if(col0==0){col=1;break;}
            if(col0==0){col=2;break;}
            if(col0==0){col=3;break;}
        }
        delay(20);
    value = keys[row][col];
    return value;
}

tbyte check_password()
{
    tbyte flag = 1;
    tbyte temp,action,r;
		r =read();
		delay(100);
    if(r!='8'){
        flag = 0;
    }
		
    
    return flag;
}

sbit W3 = P0^7;
sbit W2 = P0^6;
sbit W1 = P0^5;
sbit W0 = P0^4;


tbyte clk = 50;
tbyte aclk = 50;
tword i;


void main()
{
	row0=row1=row2=row3 = 0;
	col0=col1=col2=col3 = 1;
	
	
    RS=RW=E=0;
    LCDData = 0;
    LCD_Init();
    LCD_DispStr(1,"welcome");
		while(1){

            if(!keyopen)
					    {
								tByte x =0;
								LCD_Init();LCD_DispStr(1,"enter passcode");
								x = check_password();
                if(x==1)
                    {
                    LCD_Init();LCD_DispStr(1,"unlocking");
                    while(1)
						{
                            W3=1;W2=0;W1=0;W0=0;delay(5);if(--clk==0) break;
                            W3=0;W2=1;W1=0;W0=0;delay(5);if(--clk==0) break;
                            W3=0;W2=0;W1=1;W0=0;delay(5);if(--clk==0) break;
                            W3=0;W2=0;W1=0;W0=1;delay(5);if(--clk==0) break;
					    }
											
											   
                }
			     else
                    {
                        LCD_Init();LCD_DispStr(1,"wrong passcode");
                    }
              }
						if(!keyclose){LCD_Init();LCD_DispStr(1,"locked");
							while(1)
									{
			     						W3=0;W2=0;W1=0;W0=1;delay(5);if(--aclk==0) break
		    							W3=0;W2=0;W1=1;W0=0;delay(5);if(--aclk==0) break;
										W3=0;W2=1;W1=0;W0=0;delay(5);if(--aclk==0) break;
										W3=1;W2=0;W1=0;W0=0;delay(5);if(--aclk==0) break;
                         }
						}
            if(!survel){LCD_Init();LCD_DispStr(1,"surveillance");
                      while(1)
                      {
                       if(PUL==1)  // if metal detector doesn't detect metal
                       {
                        while(1)  // bulb flicker
                        {
								LCD_Init();LCD_DispStr(1,"intrusion");LCD_DispStr(2,"detected");
                        
									for(i=0;i<200;i++){
											buzz=10000;
											P0 = 0xff;
							     		    delay(20);
											buzz=0;
                                            P0 = 0;
										    delay(10);
									}
                        }
                       }
                    if(keyopen==0)
                        break;
                }
							}
		}
}
