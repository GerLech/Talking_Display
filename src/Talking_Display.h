/*
||
|| @file Talking_Display.h
|| @version 1.0
|| @author Gerald Lechner
|| @contact lechge@gmail.com
||
|| The communication part was copied from library "DFPlayer_Mini_Mp3_by_Makuna"
|| Written by Michael C. Miller.
||
|| @description
|| | The library uses the MP3 player module DFPlayer Mini for output.
|| | The communication with module is handled in the library.
|| | No extra driver for DFPlayer is required. Only the pointer on
|| | a Serial Interface hardware or software is required.
|| | MP3 Files for English and German are supplied and need to be copied on a Micro-SD card.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/
#ifndef Talking_Display_h
#define Talking_Display_h

#include <Arduino.h>
#include <time.h>

//for card event
#define MEDIUM_ONLINE 0
#define MEDIUM_INSERTED 1
#define MEDIUM_REMOVED 2

//vocabulary
#define WORD_HUNDREDS 100
#define WORD_TOUSEND 110
#define WORD_MILLION 120
#define WORD_BILLION 130
#define WORD_WEEKDAY 300
#define WORD_MONTH 310
#define WORD_DAY 330

#define WORD_IT_IS 201
#define WORD_CLOCK 202
#define WORD_IS 203
#define WORD_SECOND 204
#define WORD_DOT 205
#define WORD_TEMPERATURE 206
#define WORD_OUTSIDE_TEMPERATURE 207
#define WORD_DEGREE 208
#define WORD_HUMIDITY 209
#define WORD_PERCENT 210
#define WORD_MINUS 211
#define WORD_AND 212
#define WORD_CELSIUS 213
#define WORD_AIR_PRESSURE 214
#define WORD_HECTOPASCAL 215
#define WORD_FAHRENHEIT 216
#define WORD_MINUTES 217
#define WORD_HOURS 218
#define WORD_BRILLIANCE 219
#define WORD_LUX 220
#define WORD_AM 221
#define WORD_PM 222

//error messages
enum DfMp3_Error
{
    // from device
    DfMp3_Error_Busy = 1,
    DfMp3_Error_Sleeping,
    DfMp3_Error_SerialWrongStack,
    DfMp3_Error_CheckSumNotMatch,
    DfMp3_Error_FileIndexOut,
    DfMp3_Error_FileMismatch,
    DfMp3_Error_Advertise,
    // from library
    DfMp3_Error_RxTimeout = 0x81,
    DfMp3_Error_PacketSize,
    DfMp3_Error_PacketHeader,
    DfMp3_Error_PacketChecksum,
    DfMp3_Error_General = 0xff
};

//class definition
template<class T_SERIAL> class Talking_Display {
public:
  //initializer
  Talking_Display(T_SERIAL& serial) :
      _serial(serial),
      _isOnline(false)
  {
  }
  //start working send reset command and wait for an answer
  bool begin(unsigned long baud = 9600)
  {
      uint16_t cmd,arg;
      bool ready = false;
      _serial.begin(baud);
      _serial.setTimeout(10000);
      _lastSend = millis();
      reset();
      while ((!ready)&&((millis()-_lastSend)<5000)) {
        if (_serial.available()) {
          listenForReply(0x00);
          ready = true;
        }
        delay(20);
      }
      return ready;
  }

  //call this function within the main loop to reakt on
  //messages from modul
  void loop()
  {
      while (_serial.available() >= DfMp3_Packet_SIZE)
      {
          listenForReply(0x00);
      }
  }

  //register a callback funktion which will be called
  //if playing a track has finished
  void registerOnDone(void (*callback)(uint16_t track))
  {
    _onDone = callback;
  }

  //register a callback funktion which will be called
  //if card will be inserted or removed
  void registerOnCard(void (*callback)(uint8_t event))
  {
    _onSource = callback;
  }

  //register a callback funktion which will be called
  //if an error occured
  void registerOnError(void (*callback)(String msg))
  {
    _onError = callback;
  }

  //set the volume vakues from 0 to 30 are allowed
  void setVolume(uint8_t volume)
  {
      sendPacket(0x06, volume);
  }

  //reset the module
  void reset()
  {
      sendPacket(0x0C, 0);
  }

  //play a track on a certain folder
  void playFolderTrack(uint8_t folder, uint16_t track)
  {
      uint16_t arg = (((uint16_t)folder) << 12) | track;
      sendPacket(0x14, arg);
  }

  //return moduls current status
  uint16_t getStatus()
  {
      drainResponses();
      sendPacket(0x42);
      return listenForReply(0x42);
  }

  //set or unset English for language
  void setEnglish(boolean english) {
    _english = english;
  }

  //set the timeoutin seconds to wait for playing a track has endet
  void setWordTimeout(uint32_t timeout) {
    _wordTimeout = timeout;
  }

  //say the word found on the given track
  //if english folder 00 will be used otherwise folder 01
  void say(uint16_t track) {
    uint8_t folder = _english?0:1;
    playFolderTrack(folder, track);
    waitForEnd(_wordTimeout);
  }

  //speaks three digit numbers
  void sayHundreds(uint16_t number) {
    uint16_t h = number / 100;
    uint16_t e = number%100;
    if (h>0) say(WORD_HUNDREDS+h);
    say(e);
  }

  //speaks any 32 bit integer
  void sayInt(int32_t number) {
   boolean minus = (number < 0);
   if (minus) number *= -1;
   uint16_t einer = number%1000;
   number /= 1000;
   uint16_t tausender = number%1000;
   number /= 1000;
   uint16_t millionen = number%1000;
   number /= 1000;
   uint16_t milliarden = number%1000;
   if (minus) say(WORD_MINUS);
   if (milliarden > 0) {
    if (milliarden == 1) {
      say(WORD_BILLION+1);
    } else  {
      sayHundreds(milliarden);
      say(WORD_BILLION);
    }
   }
   if (millionen > 0) {
    if (millionen == 1) {
      say(WORD_MILLION+1);
    } else  {
      sayHundreds(millionen);
      say(WORD_MILLION);
    }
   }
   if (tausender > 0) {
    if (tausender == 1) {
      say(WORD_TOUSEND+1);
    } else  {
      sayHundreds(tausender);
      say(WORD_TOUSEND);
    }
   }
   sayHundreds(einer);
  }

  //speaks a float number with 1 or two decimals
  //max value +/- 2147483647.99999
  void sayFloat(float number, uint8_t decimals = 2) {
    int32_t num = (int)number;
    float n = number - (float)num;
    Serial.println(n);
    int16_t dez = 0;
    if (decimals < 2) {
      dez = round(n*10);
    } else {
      dez = round(n*100);
    }
    sayInt(num);
    say(WORD_DOT);
    say(dez);
  }

  //say a time using 24 hours and switching seconds off can be done by parameter
  void sayTime(struct tm * s_time, boolean h24 = true, boolean seconds = false) {
     uint16_t ampm = WORD_AM;
     uint8_t hour = s_time->tm_hour;
     if ((hour > 11) && (!h24)) {
       ampm = WORD_PM;
       hour = hour - 12;
     }
     say(hour);
     if (_english) {
       delay(200);
     } else {
       say(WORD_CLOCK);
     }
     say(s_time->tm_min);
     if(!h24) say(ampm);
     if (seconds) {
       say(WORD_AND);
       say(s_time->tm_sec);
       say(WORD_SECOND);
     }
  }

  //say a date switching weekday and year off can be done by parameter
  //for english month will be followed by day
  //for german day will be followed by month
  void sayDate(struct tm * s_time, boolean weekday = true, boolean sayYear = true) {
    if (weekday) say(WORD_WEEKDAY + s_time->tm_wday);
    if (_english) {
      say(WORD_MONTH+s_time->tm_mon);
      say(s_time->tm_mday);
    } else {
      say(WORD_DAY+s_time->tm_mday);
      say(WORD_MONTH+s_time->tm_mon);
    }
    if (sayYear) {
      uint16_t year = s_time->tm_year + 1900;
      if (year < 2000) {
        say(year/100);
        say(WORD_HUNDREDS);
        say(year%100);
      } else {
        sayInt(year);
      }
    }
  }

private:


  static const uint16_t c_msSendSpace = 50;

  // 7E FF 06 0F 00 01 01 xx xx EF
  // 0	->	7E is start code
  // 1	->	FF is version
  // 2	->	06 is length
  // 3	->	0F is command
  // 4	->	00 is no receive
  // 5~6	->	01 01 is argument
  // 7~8	->	checksum = 0 - ( FF+06+0F+00+01+01 )
  // 9	->	EF is end code
  enum DfMp3_Packet
  {
      DfMp3_Packet_StartCode,
      DfMp3_Packet_Version,
      DfMp3_Packet_Length,
      DfMp3_Packet_Command,
      DfMp3_Packet_RequestAck,
      DfMp3_Packet_HiByteArgument,
      DfMp3_Packet_LowByteArgument,
      DfMp3_Packet_HiByteCheckSum,
      DfMp3_Packet_LowByteCheckSum,
      DfMp3_Packet_EndCode,
      DfMp3_Packet_SIZE
  };
  T_SERIAL& _serial;
  bool _isOnline;
  uint32_t _lastSend;
  uint16_t _lastSendSpace = 0;
  boolean _english = false;
  uint32_t _wordTimeout = 5;
  void(*_onDone)(uint16_t track) = NULL;
  void(*_onSource)(uint8_t event) = NULL;
  void(*_onError)(String message) = NULL;

  boolean waitForEnd(uint32_t timeout) {
    uint32_t tim = millis();
    uint16_t st;
    delay(100);
    do {
      delay(20);
      st=getStatus();
    }
    while ((st != 512) && ((millis()-tim) < (timeout*1000)));
    return (st == 512);
  }

  void drainResponses()
  {
      while (_serial.available() > 0)
      {
          listenForReply(0x00);
      }
  }


  void sendPacket(uint8_t command, uint16_t arg = 0, uint16_t sendSpaceNeeded = c_msSendSpace)
  {
      uint8_t out[DfMp3_Packet_SIZE] = { 0x7E,
          0xFF,
          06,
          command,
          00,
          static_cast<uint8_t>(arg >> 8),
          static_cast<uint8_t>(arg & 0x00ff),
          00,
          00,
          0xEF };

      setChecksum(out);

      // wait for spacing since last send
      while (((millis() - _lastSend) < _lastSendSpace))
      {
          // check for event messages from the device while
          // we wait
          loop();
          delay(1);
      }

      _lastSendSpace = sendSpaceNeeded;
      _serial.write(out, DfMp3_Packet_SIZE);

      _lastSend = millis();
  }

  bool readPacket(uint8_t* command, uint16_t* argument)
  {
      uint8_t in[DfMp3_Packet_SIZE] = { 0 };
      uint8_t read;

      // init our out args always
      *command = 0;
      *argument = 0;

      // try to sync our reads to the packet start
      do
      {
          // we use readBytes as it gives us the standard timeout
          read = _serial.readBytes(&(in[DfMp3_Packet_StartCode]), 1);

          if (read != 1)
          {
              // nothing read
              *argument = DfMp3_Error_RxTimeout;

              return false;
          }
      } while (in[DfMp3_Packet_StartCode] != 0x7e);

      read += _serial.readBytes(in + 1, DfMp3_Packet_SIZE - 1);
      if (read < DfMp3_Packet_SIZE)
      {
          // not enough bytes, corrupted packet
          *argument = DfMp3_Error_PacketSize;
          return false;
      }

      if (in[DfMp3_Packet_Version] != 0xFF ||
          in[DfMp3_Packet_Length] != 0x06 ||
          in[DfMp3_Packet_EndCode] != 0xef)
      {
          // invalid version or corrupted packet
          *argument = DfMp3_Error_PacketHeader;
          return false;
      }

      if (!validateChecksum(in))
      {
          // checksum failed, corrupted packet
          *argument = DfMp3_Error_PacketChecksum;
          return false;
      }

      *command = in[DfMp3_Packet_Command];
      *argument = ((in[DfMp3_Packet_HiByteArgument] << 8) | in[DfMp3_Packet_LowByteArgument]);

      return true;
  }

  uint16_t listenForReply(uint8_t command)
  {
      uint8_t replyCommand = 0;
      uint16_t replyArg = 0;
      bool exit = false;

      do
      {
          if (readPacket(&replyCommand, &replyArg))
          {
              if (command != 0 && command == replyCommand)
              {
                  return replyArg;
              }
              else
              {
                  switch (replyCommand)
                  {
                  case 0x3c: // usb done
                  case 0x3d: // micro sd done
                  case 0x3e: // flash done
                      if (_onDone) _onDone(replyArg);
                      break;
                  case 0x3F: //medium online
                      _isOnline = true;
                      if (_onSource) _onSource(MEDIUM_ONLINE);
                      break;
                  case 0x3A: //medium inserted
                      _isOnline = true;
                      if (_onSource) _onSource(MEDIUM_INSERTED);
                      break;
                  case 0x3B: //medium removed
                      _isOnline = false;
                      if (_onSource) _onSource(MEDIUM_REMOVED);
                      break;
                  case 0x40:
                      if (_onError)
                      {
                        String msg = "";
                        switch (replyArg) {
                          case  DfMp3_Error_Busy : msg = "Device busy"; break;
                          case  DfMp3_Error_Sleeping : msg = "Device sleeps" ; break;
                          case  DfMp3_Error_SerialWrongStack : msg="Wrong stack"; break;
                          case  DfMp3_Error_CheckSumNotMatch : msg="Checksum does not match"; break;
                          case  DfMp3_Error_FileIndexOut : msg="File index does not exist"; break;
                          case  DfMp3_Error_FileMismatch : msg="Wrong file type"; break;
                          case  DfMp3_Error_Advertise : msg="Advertise error"; break;
                          case  DfMp3_Error_RxTimeout : msg="Receive timeout"; break;
                          case  DfMp3_Error_PacketSize : msg="Wrong packet size"; break;
                          case  DfMp3_Error_PacketHeader : msg="Wrong packet header"; break;
                          case  DfMp3_Error_PacketChecksum : msg="Wrong packet checksum"; break;
                          default : msg="General error"; break;

                        }
                        _onError(msg);
                      }
                      exit = true;
                      break;

                  default:
                      // unknown/unsupported command reply
                      break;
                  }
              }
          }
          else
          {
              if (replyArg != 0)
              {
                  if (_onError) _onError("Illegal argument");
                  if (_serial.available() == 0)
                  {
                      exit = true;
                  }
              } else {
                exit = true;
              }
          }
      } while ((command != 0) && (!exit));

      return 0;
  }

  uint16_t calcChecksum(uint8_t* packet)
  {
      uint16_t sum = 0;
      for (int i = DfMp3_Packet_Version; i < DfMp3_Packet_HiByteCheckSum; i++)
      {
          sum += packet[i];
      }
      return -sum;
  }

  void setChecksum(uint8_t* out)
  {
      uint16_t sum = calcChecksum(out);

      out[DfMp3_Packet_HiByteCheckSum] = (sum >> 8);
      out[DfMp3_Packet_LowByteCheckSum] = (sum & 0xff);
  }

  bool validateChecksum(uint8_t* in)
  {
      uint16_t sum = calcChecksum(in);
      return (sum == static_cast<uint16_t>((in[DfMp3_Packet_HiByteCheckSum] << 8) | in[DfMp3_Packet_LowByteCheckSum]));
  }
};
#endif
