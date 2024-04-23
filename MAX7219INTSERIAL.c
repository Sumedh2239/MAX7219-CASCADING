/*AUTHOR :- SUMEDH KHEDEKAR    

CODE USING 7219
	
DATE-11-04-2024	
DOT LOGIC IMPLEMENTED
DATE 19-04-2024 CB  LEFT*/
	
#include<stdio.h>
#include<reg52.h>
#include"INTRINS.H"
sbit CLK=P2^2;
sbit LOAD=P2^1;
sbit DIN=P2^0;
//sbit mybit=ACC^7;
unsigned char i,p,k;
unsigned char Noofwindows=3;//Each window is of 8 digits
unsigned char address1[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};//for addresses of digits 0-7
unsigned char Array1[11]={0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x73};//used for storing the recived values viz string @1.2345.678.....WITHOUT DP
unsigned char Array4[11]={0xFE,0xB0,0xEd,0xF9,0xB3,0xDB,0xDf,0xF0,0xFf,0xF3};//used for storing the recived values viz string @1.2345.678.....WITH DP
unsigned char mdata1[24]={0xF0,0x30,0x6d,0x79, 0x33,0x5b,0x5f,0x70, 0x5b,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70, 0x7F,0x73,0x79,0x33, 0x5b,0x5f,0x70,0x7F};//used for storing the recived values viz string @1.2345.678.....WITHOUT DP
unsigned char idata Array2[50],Array3[50];
bit positionN=0,at_flag=0,Hooter_flag=0,Start=0;
unsigned char MYDATA;
void sendData(unsigned char , unsigned char);
void WriteAddrData(unsigned char , unsigned char);
void WriteAddrData1(unsigned char address[] , unsigned char mdata[]);
void initMAX7219();
void TRANSMIT();
void SPI_write(unsigned char mdata);
void displayDigit(unsigned char, unsigned char);
void msdelay(unsigned int milliseconds);
void recieve_string();
unsigned char recieve();
void dot_logic();
#define MAX7219_NO_OP 0x00
#define MAX7219_REG_DECODE_MODE 0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCAN_LIMIT  0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAY_TEST 0x0F

// Function to send data to MAX7219
void sendData(unsigned char address, unsigned char mdata) {
    // Select MAX7219
       LOAD = 0;
    // Send address and data
                    WriteAddrData(address,mdata);    	            
// Deselect MAX7219
    LOAD = 1;
_nop_();
	_nop_();
_nop_();
	_nop_();
}
void sendData1(unsigned char address, unsigned char mdata) {
	
    // Select MAX7219
 LOAD = 0;
    // Send address and data

                    for(i=0;i<Noofwindows;i++)
	{
                    WriteAddrData(address,mdata);
	}                          
// Deselect MAX7219
    LOAD = 1;
_nop_();
	_nop_();
_nop_();
	_nop_();
}
void WriteAddrData(unsigned char address,unsigned char mdata)
{
                     SPI_write(address);
	                   _nop_();
											SPI_write(mdata);
}
void WriteAddrData1(unsigned char address[],unsigned char mdata[])
{
	unsigned char j;
for(i=0;i<8;i++)
       {
        LOAD=0;
for(j=0;j<Noofwindows;j++)
								{
								SPI_write(address[i]);
									_nop_();
									SPI_write(mdata[i+(8*j)]);
								}
        LOAD=1;
            }  
										
_nop_();
	_nop_();
_nop_();
	_nop_();
}
// Function to initialize MAX7219
void initMAX7219() {
	CLK=0;
	//LOAD=0;1242024
	LOAD=0;
	DIN=0;
	TMOD = 0x20;
	SCON = 0x50;
	TH1  = 0xE8;
	//TL1  = 0xE8;
	IE = 0x90;
	TR1  = 1;
	 
  Start=1;
    // Decode mode: no decoding
    sendData1(MAX7219_REG_DECODE_MODE, 0x00); 
	// Intensity: 15 (adjust as needed)
    sendData1(MAX7219_REG_INTENSITY, 0x0F);
    // Scan limit: all digits
	sendData1(MAX7219_REG_SCAN_LIMIT, 0x07);//0X07 FOR 8  	
	// Normal operation
    sendData1(MAX7219_REG_SHUTDOWN, 0x01);//0x00 for  shutdown  
		 // Disable display test
    sendData1(MAX7219_REG_DISPLAY_TEST, 0x00);
}
// Function to transmit a byte via SPI
void SPI_write(unsigned char m1data) {
  unsigned char i;
    // Loop through each bit in the data byte
    for (i = 0; i < 8; i++) {
	
			// Set the DIN pin according to the current bit (MSB first)
        DIN = (m1data & 0x80) ? 1 : 0;
        // Shift data byte left by one bit
        m1data <<= 1;
        // Pulse the clock
        CLK = 0;  // Set clock pin high//HERE IT WILL BE LOST FIRST BIT OF CONSECIUTIVE
        // Insert a delay if necessary to meet SPI timing requirements
        // e.g., for a 11.052MHz crystal, a delay of a few microseconds might be sufficient
        _nop_();
			_nop_();
			_nop_();
			_nop_();
					CLK = 1;  // Set clock pin low
    }
}
void displayDigit(unsigned char digit, unsigned char position) {
    // Send data to corresponding digit
    sendData(position + 1, digit);
}

void main() 
{
    // Initialize MAX7219
	initMAX7219();
  while(1)
	{
		TR1=1;
	MYDATA=recieve();
		if(MYDATA=='@')
	{
		recieve_string();
		TR1=0;
    dot_logic();
		TH1=0XE8;
		TR1=1;
//TRANSMIT();
		
  	WriteAddrData1(address1,Array3);
		
 	 // WriteAddrData1(address1,mdata1);
   }	
	
//	WriteAddrData1(address1,mdata1);
	}
}
void dot_logic()
{   	
unsigned char p,k;	
p=0;
for(k=0;k<=29;k++)
{
if(Array2[p+1]=='.')
{
   Array3[k]=Array4[Array2[p]-0x30];
   p+=2;/////----->POINTS TO NEXT VALUE NOT D.P.
}
else
{
  Array3[k]=Array1[Array2[p]-0x30];
  p++;
}
}
}	
void msDelay(unsigned int milliseconds) {
 unsigned char j;
    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 125; j++) {
            // Adjust the inner loop count to suit your clock frequency
            // For a 12MHz crystal, 120 loops approximately give 1ms delay
        }
    }
}
void recieve_string()
{

for(i=0;i<=29;i++)
	{
		Array2[i]=recieve();
	}
}
unsigned char recieve()
{
while(!RI);
	RI=0;
	return SBUF;

}
void TRANSMIT()
{
for(i=0;i<=23;i++)
	{
	SBUF=Array3[i];
		while(TI==0);
		TI=0;
	}
		
}
