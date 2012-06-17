/*
OASIS Control Software

This firmware controls the entire smart irrigation system based on an Arcuino 328 and a DS1307 RTC chip.
Version 0.1 has several control outputs and sensor inputs.

Outputs
  Solenoid
  SSR
  Buzzer or Vibrator
  LED Time Display with MAX7219

Inputs
  Water level sensor
  
The codes are not for commercial use in the scope of CCL. 
Coded for Arduino 1.0.1.

*/

//We always have to include the library
#include "LedControl.h"
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include <Time.h>
#include <TimeAlarms.h>

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */


 void water_level_detection(void);
 void Aerator_ON(void);
 void Aerator_OFF(void);
 void PUMP1_ON(void);
 void PUMP1_OFF(void);
 void PUMP2_ON(void);
 void PUMP2_OFF(void);
 
 
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 200;
unsigned long intensitydelay = 60;
time_t last_time;

void setup() {
  Serial.begin(38400);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  setSyncInterval(10);

  pinMode(2, OUTPUT);     // BUZZER
  pinMode(3, INPUT);      // WATER LEVEL SENSOR
  digitalWrite(3, HIGH);  // WATER LEVEL SENSOR PULL-UP
  pinMode(4, OUTPUT);     // SOLENOID CONTROL OUTPUT
  digitalWrite(4, LOW);
  pinMode(5, INPUT);      // WATER LEVEL SENSOR
  digitalWrite(5, HIGH);  // WATER LEVEL SENSOR PULL-UP
  pinMode(6, OUTPUT);     // PUMP 01 UP 200
  pinMode(7, OUTPUT);     // PUMP 02 UP 400
  pinMode(8, OUTPUT);     // PUMP 03 AIR 
  
 
if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      

  Alarm.alarmRepeat(19,40,0, PUMP1_ON);  // 8:30am every day
  Alarm.alarmRepeat(19,42,0, PUMP1_OFF);  // 5:45pm every day 
  Alarm.alarmRepeat(19,50,0, PUMP2_ON);  // 8:30am every day
  Alarm.alarmRepeat(19,52,0, PUMP2_OFF);  // 5:45pm every day 
  Alarm.alarmRepeat(6,30,0, PUMP2_ON);  // 8:30am every day
  Alarm.alarmRepeat(6,33,0, PUMP2_OFF);  // 5:45pm every day 
  Alarm.alarmRepeat(1,55,0, Aerator_ON);  // 8:30am every day
  Alarm.alarmRepeat(5,55,0, Aerator_OFF);  // 5:45pm every day 
  Alarm.alarmRepeat(20,0,0, Aerator_ON);  // 8:30am every day
  Alarm.alarmRepeat(0,0,0, Aerator_OFF);  // 5:45pm every day 

//  Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday 

 
//  Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
//  Alarm.timerOnce(10, OnceOnly);             // called once after 10 seconds 


  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
   
// digitalWrite(2, HIGH);
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  /* and clear the display */
  lc.clearDisplay(0);
  (500);
//  writeArduinoOn7Segment();
//  delay(500);
//  digitalWrite(2, LOW);
//  intensitycycle();
  lc.clearDisplay(0);
  lc.setIntensity(0,12);
  
  
  
}


void loop()
{
  //writeArduinoOn7Segment();
  //allzeros();
  //countDigits();
  time_t t;
  
//  Serial.println("7");
  
  if(Serial.available())
  {
     t = processSyncMessage();
     if(t >0)
     {
        RTC.set(t);   // set the RTC and the system time to the received value
        setTime(RTC.get());
//        setTime(t);          
   }
  }


  t = now();


  if(t!=last_time)
   {
    if(timeStatus() == timeNeedsSync)
      Serial.println("System Clock is not Synched to the RTC");
      
     digitalClockDisplay();
   }

     Alarm.delay(10);
     water_level_detection();

     last_time = t;
     

//   delay(100);
   
}

/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  lc.setChar(0,7,'a',false);
  lc.setRow(0,6,0x05);
  lc.setChar(0,5,'d',false);
  lc.setRow(0,4,0x1c);
  lc.setRow(0,3,B00010000);
  lc.setRow(0,2,0x15);
  lc.setRow(0,1,0x1D);
} 

void intensitycycle(){

  for(int j=0;j<3;j++){
    for(int i=0;i<16;i++){
      lc.setIntensity(0,i);
      delay(intensitydelay);
    }
    for(int i=0;i<16;i++){
      lc.setIntensity(0,15-i);
      delay(intensitydelay); 
    }
  }

}

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  for(int i=0;i<13;i++) {
    lc.setDigit(0,7,i,false);
    lc.setDigit(0,6,i+1,false);
    lc.setDigit(0,5,i+2,false);
    lc.setDigit(0,4,i+3,false);
    lc.setDigit(0,3,i+4,false);
    lc.setDigit(0,2,i+5,false);
    lc.setDigit(0,1,i+6,false);
    lc.setDigit(0,0,i+7,false);
    delay(delaytime);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}

void countDigits() {
  for(int a=0;a<10;a++) {
    for(int b=0;b<10;b++) {
      for(int c=0;c<10;c++) {
        for(int d=0;d<10;d++) {
          for(int e=0;e<10;e++) {
            for(int f=0;f<10;f++) {
              for(int g=0;g<10;g++) {
                for(int h=0;h<10;h++) {
                  lc.setDigit(0,7,a,false);
                  lc.setDigit(0,6,b,false);
                  lc.setDigit(0,5,c,false);
                  lc.setDigit(0,4,d,false);
                  lc.setDigit(0,3,e,false);
                  lc.setDigit(0,2,f,false);
                  lc.setDigit(0,1,g,false);
                  lc.setDigit(0,0,h,false);
                }
              }
            }
          }
        }
      }
    }
  }
}

void allzeros() {
  lc.setDigit(0,7,0,false);
  lc.setDigit(0,6,0,false);
  lc.setDigit(0,5,0,false);
  lc.setDigit(0,4,0,false);
  lc.setDigit(0,3,0,false);
  lc.setDigit(0,2,0,false);
  lc.setDigit(0,1,0,false);
  lc.setDigit(0,0,0,false);
}

void digitalClockSerial(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(weekday());
  Serial.println(); 
}


void digitalClockDisplay(){
  // digital clock display of the time

  printDoubleDigits(0,hour(),false);
  printDoubleDigits(1,minute(),true);
  printDoubleDigits(2,second(),true);
//  printDoubleDigits(3,0);
  
}

void printDoubleDigits(int column, int value, boolean dot){

  int tenth, nth;
  
  tenth = value/10;
  nth = value % 10;
    
  if(column >= 0){
    if(column < 4){  
    lc.setDigit(0,7-column*2,tenth,dot);
    lc.setDigit(0,7-column*2-1,nth,dot);
    }
  }
  
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*  code to process time sync messages from the serial port   */
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message

time_t processSyncMessage() {
  // return the time if a valid sync message is received on the serial port.
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      return pctime; 
    }  
  }
  return 0;
}


