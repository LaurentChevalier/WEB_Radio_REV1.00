/* 
 *  Code From https://www.xtronical.com/i2sinternetradio/
 *  And From http://davidjwatts.com/youtube/espRadio/Web_Radio.ino
 *  */

#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

// Digital I/O used
#define I2S_DOUT      25  // DIN connection
#define I2S_BCLK      27  // Bit clock
#define I2S_LRC       26  // Left Right Clock

#define VolPin        34 // ADC pot



volatile int radioStation = 1;
volatile int previousRadioStation = -1;

volatile long interval = 1000; 
volatile long seconds = 0;
volatile long previousMillis = 0; 

volatile int adc=0;
volatile int i=0;

volatile int averagevol = 0;

 
Audio audio;

uint8_t Volume=5;
 
String ssid =     "ssid";
String password = "password";

// Few Radio Stations
char *host[8] = {"8.38.78.173","https://radios.rtbf.be/classic21-128.mp3","bbcmedia.ic.llnwd.net/stream/bbcmedia_6music_mf_p", "bbcmedia.ic.llnwd.net/stream/bbcmedia_radio1_mf_p", "bbcmedia.ic.llnwd.net/stream/bbcmedia_radio2_mf_p", "bbcmedia.ic.llnwd.net/stream/bbcmedia_radio4fm_mf_p", "bbcwssc.ic.llnwd.net/stream/bbcwssc_mp1_ws-eieuk", "media-ice.musicradio.com/ClassicFMMP3"};
char *stationName[8] = {"Classic 21", "Cornucopia Broadcasting", "BBC 6 Music", "BBC Radio 1", "BBC Radio 2", "BBC Radio 4", "BBC World Service", "Classic FM"};



void setup() {
    
    analogReadResolution(10);
    Serial.begin(9600);
    Serial2.begin(9600);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(Volume); // 0...21
     
    audio.connecttohost("https://radios.rtbf.be/classic21-128.mp3"); //  128k mp3

}
 
void loop()
{

audio.loop();
station_connect();
audio.setVolume(GetVolume());

}


uint8_t GetVolume()
{
 for (i=0; i < 1000; i++) {

   uint16_t VolumeSettingReading;
   uint8_t VolumeReading;
   VolumeSettingReading=analogRead(VolPin); 
   adc=VolumeSettingReading;
   VolumeReading=(map(VolumeSettingReading, 0, 1023, 0, 50));
   averagevol = averagevol + VolumeReading;
 }
 averagevol = averagevol/1000;
 //Serial.println(averagevol);

 if (i>=1000)
 {
  i=0;
 }
  //Serial.println(adc);
 return averagevol;
  
}

void station_connect () {
if (Serial2.available() > 0)  {
       // Serial.println("données recues");
    int incomingData = Serial2.read();
    //Read data from the serial port to decide what to play
    switch (incomingData) {
      case 1:
        radioStation = 1;
        Serial.println(stationName[radioStation]);
        break;

      case 2:
        radioStation = 2;
        Serial.println(stationName[radioStation]);
        break;

      case 3:
        radioStation = 3;
        Serial.println(stationName[radioStation]);
        break;

      case 4:
        radioStation = 4;
        Serial.println(stationName[radioStation]);
        break;
      case 5:
        radioStation = 5;
        Serial.println(stationName[radioStation]);
        break;
      case 6:
        radioStation = 6;
        Serial.println(stationName[radioStation]);
        break;
      case 7:
        radioStation = 7;
        Serial.println(stationName[radioStation]);
        break;
      default:
        //radioStation = 0;
        break;
    }
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) 
    {
    if(radioStation!=previousRadioStation)
    {
      audio.connecttohost(host[radioStation]);
      Serial.println("connected to radio");
      previousRadioStation = radioStation;
      seconds = 0;
    }else
    {
      seconds++;
    }
    previousMillis = currentMillis; 
    Serial.println("loop(): "+String(seconds) +" "+String(radioStation));  
  }
  
}
     
    
}
// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
