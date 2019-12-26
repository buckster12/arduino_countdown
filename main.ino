#include <SevSeg.h>
#include <iarduino_RTC.h>
//#include <Time.h>

SevSeg sevseg; //Instantiate a seven segment object
iarduino_RTC time(RTC_DS1302, A3, A1, A2);

#define MAX_NUMBER_STRINGS 4
#define MAX_STRING_SIZE 4
#define PATTERN_CHANGE_TIME 30000
unsigned long timer = millis() - PATTERN_CHANGE_TIME;
byte testStringsPos = 0;

void setup() {
  Serial.begin(9600);
  time.begin();
//  time.settime(0,12,21,26,12,19,4);  // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник

  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins); 
  // If your display is common anode type, please edit comment above line and uncomment below line
  // sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10);
}

long Godn(int yy1, int yy2)
{ long jj,bb;
  bb=0;
  for(jj=yy1;jj<yy2;jj++){
    bb+=365;
    if(IsLeapG(jj)==1) bb+=1;
  }
  return(bb);
}// end Godn

//Day of the Year
long rbdug(int d,int m, int y)
{ long a,r[13];
  r[1] = 0; r[2] = 31; r[3] = 59; r[4] = 90;
  r[5] = 120; r[6] = 151; r[7] = 181; r[8] = 212;
  r[9] = 243; r[10]= 273; r[11]= 304; r[12]= 334;
  a=r[m]+d;
  if((IsLeapG(y)==1)&&(m>2)) a+=1;
  return(a);
}//end rbdug

void loop() {
  static unsigned long timer = millis();
  static int deciSeconds = 0;
  static int currentValue = 0;
 
  // Cycle to the next string every one second
  if (millis() > (timer + PATTERN_CHANGE_TIME)) {

    char * diffnumberdays = diffdays(
      atoi(time.gettime("d")),
      atoi(time.gettime("m")),
      atoi(time.gettime("y")),
      1,
      2,
      20
    );
    sevseg.setChars(diffnumberdays);
//    sevseg.setNumber(1);
    testStringsPos++;
    if (testStringsPos >= MAX_NUMBER_STRINGS) testStringsPos = 0;
    timer = millis();
  }
  sevseg.refreshDisplay(); // Must run repeatedly
}

// https://stackoverflow.com/questions/13932909/difference-between-two-dates-in-c
// get count of days between two dates
char * diffdays(int firstday, int firstmonth, int y1, int secondday, int secondmonth, int y2) {
  int num = 0;

  int suma = rbdug(secondday,secondmonth,y2) - rbdug(firstday,firstmonth,y1);
  if(y1 != y2){
    if(y1 < y2){
      suma+=Godn(y1,y2);
    }else{
      suma-=Godn(y2,y1);
    }
  }
//  return(suma);
  
  char cstr[16];
  return itoa(suma, cstr, 10);
}


//is leap year in Gregorian
long IsLeapG(int yr){
  if(((((yr%400)==0)||((yr%100)!=0))&&((yr%4)==0))){
    return(1);
  }else{
    return(0);
  }
}
