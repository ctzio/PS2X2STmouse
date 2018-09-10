/*
 * Playstation 2 joypad to Atari ST Mouse converter project by Christos Tziotzis.
 * This project is based on the PS2 controller Arduino Library v1.8 by Bill Porter
 * and as such it follows its GPL v3 Licencing.
 * 
 * Some copy pasting follows which is of course just as valid!:
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*  
*/

//Copy paste from the example

#include <PS2X_lib.h>  //for v1.6

#define x2 7
#define x1 6
#define y2 5
#define y1 4
#define lb 2
#define rb 1

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;
int mouseX,mouseY, fromCenterX, fromCenterY;
int velocityX,velocityY, delayX, delayY;

void setup(){
 Serial.begin(57600);

 //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
 error = ps2x.config_gamepad(10,12,11,8, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }

 
  //Setup Atari Joystick Pins

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);

digitalWrite(1, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  

  
}

void loop(){


   
 if(error == 1) //skip loop if no controller found
  return; 
  
 
 //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  //read ps2 joypad values and calculate velocity.
  
      mouseX = ps2x.Analog(PSS_LX);
      mouseY = ps2x.Analog(PSS_LY);

      fromCenterX = abs(mouseX-128);
      delayX = 130-fromCenterX;
      Serial.println(delayX);
      fromCenterY = abs(mouseY-128);
      delayY = 130-fromCenterY;
     

     
      
//read X and Square as left and right mouse button
      
      if(ps2x.Button(PSB_BLUE)){
       digitalWrite(lb, LOW);
       
      }   
      else {
        digitalWrite(lb, HIGH);

      }
      if(ps2x.Button(PSB_RED)){
       digitalWrite(rb, LOW);
     }   
      else {
        digitalWrite(rb, HIGH);
      }

//the ps2 controller outputs a value to the Atari mouse, we convert it to square pulses here. 128 is center.
     if (mouseX>128) {
      digitalWrite(x1,LOW);
      delay(delayX);
      digitalWrite(x2,LOW);
      digitalWrite(x1,HIGH);
      delay(delayX);
      digitalWrite(x2,HIGH);
     }


    if (mouseX==128) {
      digitalWrite(x2,HIGH);
      delay(2);
      digitalWrite(x1,HIGH);
     }


   if (mouseX<128) {
      digitalWrite(x2,LOW);
      delay(delayX);
      digitalWrite(x1,LOW);
      digitalWrite(x2,HIGH);
      delay(delayX);
      digitalWrite(x1,HIGH);
     }
   if (mouseY<128) {
      digitalWrite(y2,LOW);
      delay(delayY);
      digitalWrite(y1,LOW);
      digitalWrite(y2,HIGH);
      delay(delayY);
      digitalWrite(y1,HIGH);
     }
   if (mouseY>128) {
      digitalWrite(y1,LOW);
      delay(delayY);
      digitalWrite(y2,LOW);
      digitalWrite(y1,HIGH);
      delay(delayY);
      digitalWrite(y2,HIGH);
      }
   if (mouseY==128) {
      digitalWrite(y1,HIGH);
      delay(2);
      digitalWrite(y2,HIGH);  
   }

   //this is the end. It really took me much too long to get it working. Logic bugs are really the worse to locate. 
   //You know I could make it work with the Amiga also but I don't think I will ;). Have fun!

}
     
 

     
