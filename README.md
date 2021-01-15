# Talking_Display
This library works with the DFPlayer Mini module, to generate speech output for numbers, dates, times and a number of special words. The library require an instance of SoftwareSerial or HardwareSerial for communication with the module. The library can be used with any hardware package. The following functions can be used:

**void begin(unsigned long baud = 9600)** Starts the instance, send reset command and wait for an answer

**void loop()**  Tthis function needs to be called within the main loop to reakt on messages from module

**void registerOnDone(void (\*callback)(uint16_t track))** Register a callback function which will be called if playing a track has finished

**void registerOnCard(void (\*callback)(uint8_t event))** Register a callback funktion which will be called if card will be inserted or removed

**void registerOnError(void (\*callback)(String msg))** Register a callback funktion which will be called if an error occured

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

## Vocabulary

To use the library you need a Micro SD Card with MP3 files containing the vocabulary. The vocabulary is in two folders. One named 00 containing the English vocabulary and one named 01 containing the German vocabulary. All the required MP3 files are contained in the library in a ZIP file named words.zip. The filenames, words and predefine names are listed in the following table:

|File from|File to|Word English|Word German|Constant|
|----------|----------|-----------------|-----------------|----------|
|0000.mp3|0099.mp3|Numbers 0 to 99|Zahlen 0 bis 99|-|
|0100.mp3|0100.mp3|Hundred|Hundert|WORD_HUNDREDS|
|0101.mp3|0109.mp3|100,200,300,...900|100,200,300,...900|-|
|0110.mp3|0110.mp3|Tousend|Tausend|WORD_TOUSEND|
|0111.mp3|0111.mp3|One tousend|Eintausend|-|
|0120.mp3|0120.mp3|Million|Millionen|WORD_MILLION|
|0121.mp3|0121.mp3|One Million|Eine Million|-|
|0130.mp3|0130.mp3|Billion|Milliarden|WORD_BILLION|
|0131.mp3|0131.mp3|One Billion|Eine Milliarde|-|
|0201.mp3|0201.mp3|It is|Es ist|WORD_IT_IS|
|0202.mp3|0202.mp3|O clock|Uhr|WORD_CLOCK|
|0203.mp3|0203.mp3|Is|Ist|WORD_IS|
|0204.mp3|0204.mp3|Second|Sekunden|WORD_SECOND|
|0205.mp3|0205.mp3|Dot|Komma|WORD_DOT|
|0206.mp3|0206.mp3|Temperature|Temperatur|WORD_TEMPERATURE|
|0207.mp3|0207.mp3|Outside Temperature|Aussentemperatur|WORD_OUTSIDE_TEMPERATURE|
|0208.mp3|0208.mp3|Degree|Grad|WORD_DEGREE|
|0209.mp3|0209.mp3|Humidity|Luftfeuchtigkeit|WORD_HUMIDITY|
|0210.mp3|0210.mp3|Percent|Prozent|WORD_PERCENT|
|0211.mp3|0211.mp3|Minus|Minus|WORD_MINUS|
|0212.mp3|0212.mp3|And|Und|WORD_AND|
|0213.mp3|0213.mp3|Celsius|Celsius|WORD_CELSIUS|
|0214.mp3|0214.mp3|Air pressure|Luftdruck|WORD_AIR_PRESSURE|
|0215.mp3|0215.mp3|Hectopascal|Hektopascal|WORD_HECTOPASCAL|
|0216.mp3|0216.mp3|Fahrenheit|Fahrenheit|WORD_FAHRENHEIT|
|0217.mp3|0217.mp3|Minutes|Minuten|WORD_MINUTES|
|0218.mp3|0218.mp3|Hours|Stunden|WORD_HOURS|
|0219.mp3|0219.mp3|Brilliance|Helligkeit|WORD_BRILLIANCE|
|0220.mp3|0220.mp3|Lux|Lux|WORD_LUX|
|0221.mp3|0221.mp3|am|Vormittag|WORD_AM|
|0222.mp3|0222.mp3|pm|Nachmittag|WORD_PM|
|0300.mp3|0306.mp3|Weekdays from Sunday to Saturday|Wochentage von Sonntag bis Samstag|WORD_WEEKDAY|
|0310.mp3|0322.mp3|Months from January to December|Monate von Januar bis Dezember|WORD_MONTH|
|0331.mp3|0362.mp3|First of to Thirtyfirst of|Erster bis Einundreißigster|WORD_DAY|
