# Talking_Display
This library works with the DFPlayer Mini module, to generate speech output for numbers, dates, times and a number of special words. The library require an instance of SoftwareSerial or HardwareSerial for communication with the module. The library can be used with any hardware package. The following functions can be used:

**void begin(unsigned long baud = 9600)** Starts the instance, send reset command and wait for an answer

**void loop()**  Tthis function needs to be called within the main loop to reakt on messages from module

**void registerOnDone(void (*callback)(uint16_t track))** Register a callback function which will be called if playing a track has finished

**void registerOnCard(void (*callback)(uint8_t event))** Register a callback funktion which will be called if card will be inserted or removed

**void registerOnError(void (*callback)(String msg))** Register a callback funktion which will be called if an error occured

**void setVolume(uint8_t volume)** Set the volume. Values from 0 to 30 are allowed

**void reset()** Reset the module

**void playFolderTrack(uint8_t folder, uint16_t track)** Play a track on a certain folder

**uint16_t getStatus()** Return moduls current status

**void setEnglish(boolean english)** Set or unset English for language

**void setWordTimeout(uint32_t timeout)** Set the timeout in seconds to wait for playing a track has finished

**void say(uint16_t track)** Say the word found on the given track. If english was set, folder 00 will be used otherwise folder 01

**void sayHundreds(uint16_t number)** Speaks positive three digit numbers from 0 to 999

**void sayInt(int32_t number)** Speaks any 32 bit integer. Range +/- 2147483647

**void sayFloat(float number, uint8_t decimals = 2)** Speaks a float number with 1 or two decimals. Range +/- 2147483647.99

**void sayTime(struct tm * s_time, boolean h24 = true, boolean seconds = false)** Speak the time specified in the tm structure. Using 24 hours and switching seconds off can be selected by parameter.

**void sayDate(struct tm * s_time, boolean weekday = true, boolean sayYear = true)** Speak the date specified by the tm structure. Switching weekday and year off can be selected by parameter
