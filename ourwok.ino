
#include <math.h>
#include "ps2mouse.h"
#include "MI0283QT2.h"
/*
 * an arduino sketch to interface with a ps/2 mouse and display the 
 * mouse movement on an lcd shield.
 *
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */

#define BACKCOLOR 0x0000 // Black
#define PIXELCOLOR 0xFFFF  // White


/*
 * according to some code I saw, these functions will
 * correctly set the mouse clock and data pins for
 * various conditions.
 */

int xPos = 0,yPos = 0; //initializing the mouse positions to be used
  
bool rightChk = false;

MI0283QT2 lcd;  //creating an istance of the tft.



void setup()
{
  //Serial.begin(9600);
  lcd.init (4 ) ; // spi-clk = Fcpu / 4
  mouse_init();
  //lcd.clear ( RGB ( 255 , 255 , 255 ) ) ; // clear screen in white 
  lcd.setOrientation ( 270 ) ; // configure display orientation in degrees: 0,90,180,270 
  lcd.clear ( RGB ( 255 , 255 , 255 ) ) ; // clear screen in white
  lcd.led ( 50 ) ; // fixed brightness 
 // lcd.drawText(20,150,"Distance: ",1,RGB(0,0,0),RGB(255,255,255));
 
  //mouse_init();
  
  //point(xPos,yPos,xPrev,yPrev);
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;
  

  // Block runs only when system is not paused
  // takes reading from mouse
  // gives X,Y values
  if(!rightChk){
    
    mouse_write(0xeb);  /* give me data! */
    mouse_read();      /* ignore ack */
    mstat = mouse_read();
    mx = mouse_read();
    my = mouse_read();

    // ---->>> For Calculating Distance    
    
    /* Convert X, Y values to int as below in code,
     *  Use Math.sqrt and Math.pow for calculating distance between 2 points
     *  Store previous values of X,Y to continue when paused and started 
     *  Use formula for measuring distance between two points
     */
  
    xPos = xPos + int(mx);
    yPos = yPos + int(my);


    //Scaling the movment within the  size of the dosplay 240 X 320 pixels
    
    if(yPos <-320){yPos=-320;} 
    if(yPos>0){yPos=0;}
    if(xPos<0){xPos=0;}
    if(xPos>240){xPos=240;}

     
   //display position as a circle  
    lcd.drawCircle(xPos,-yPos,1,red);
    
 
  }
  
  mouse_write(0xeb);  
  mouse_read();
  mstat = mouse_read();


  // converting clicks variable to int
  int btnChk = int (mstat);
  

  // for int value of 10, right button is clicked
  if( btnChk == 10){
    //Right Button Clicked movement is paused, System Paused!,Left Click to Continue
    
    rightChk = true;
    delay(40);  
  }

  // for int value of 9, left button is clicked
  if(btnChk == 9){
    //movement continue
    rightChk = false;
    delay(40);  
  }

  // for int value of 12, Mouse Wheel is clicked
  //clear screen
  if(btnChk == 12){
  
    lcd.clear ( RGB ( 255 , 255 , 255 ) ) ; 
    delay(500);
  }

 
  }
 
