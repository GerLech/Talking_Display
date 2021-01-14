#include <Talking_Display.h>
#include <SoftwareSerial.h>
#include <time.h>

SoftwareSerial ss(4,5); // RX, TX
Talking_Display<SoftwareSerial> td(ss);

uint32_t tim;

void tdDone(uint16_t track) {
  Serial.print("Finnished play track ");
  Serial.println(track);
}

void tdCard(uint8_t event) {
  switch (event) {
    case MEDIUM_ONLINE : Serial.println("SD-Card online"); break;
    case MEDIUM_INSERTED : Serial.println("SD-Card inserted"); break;
    case MEDIUM_REMOVED : Serial.println("SD-Card removed"); break;
  }
}

void tdError(String msg) {
  Serial.println(msg);
}

void setup() {
  Serial.begin(74880);
  Serial.println();
  Serial.println("Start");
  td.begin();
  td.setVolume(30);
  td.registerOnDone(tdDone);
  td.registerOnError(tdError);
  td.registerOnCard(tdCard);
  
  tim = millis();
  Serial.println("Commands: E=english, G=german, I##=integer, F##=float,");
  Serial.println("W##=word, D=date1, d=date2, T=time1, t=time2");
}

void loop() {
  /*
  if ((millis()-tim) > 10000) {
    td.say(1,207);
    td.say(1,203);
    td.say(1,24);
    td.say(1,208);
    td.say(1,213);
    tim = millis();
  }
  */
  td.loop();
  if (Serial.available()) {
    time_t t1 = 607016004; //is Mon, 27 Mar 1989 15:33:24 
    time_t t2 = 1610602200; //is Thu, 14 Jan 2021 05:30:00 
    struct tm * s_time;
    String x = Serial.readString();
    Serial.println(x);
    char cmd = x[0];
    x = x.substring(1);
    switch (cmd) {
      case 'E' : td.setEnglish(true); break;
      case 'G' : td.setEnglish(false); break;
      case 'F' : td.sayFloat(x.toFloat(),1); break;
      case 'f' : td.sayFloat(x.toFloat(),2); break;
      case 'I' : td.sayInt(x.toInt()); break;
      case 'T' :  
         s_time = localtime(&t1);
         Serial.println("15:33:24");
         td.sayTime(s_time,true,true);
         break;
      case 't' : 
         s_time = localtime(&t2);
         Serial.println("05:30:00");
         td.sayTime(s_time,false,false);
         break;
      case 'D' : 
         s_time = localtime(&t1);
         Serial.println("Mon, 27 Mar 1989");
         td.sayDate(s_time,true);
         break;
      case 'd' : 
         s_time = localtime(&t2);
         Serial.println("14 Jan 2021");
         td.sayDate(s_time,false);
         break;
      case 'W' : td.say(x.toInt()); break;
    }
     
  }

}
