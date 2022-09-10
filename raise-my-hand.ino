// ESP32 with RFM95W
// -> deep sleep ESP32
// power consumption whilst deep sleep 4 uA 

#include <time.h>
#include <SPI.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

#define LORA_SS 5
#define LORA_RST 15
#define LORA_DI0 4
#define LORA_MISO 19
#define LORA_MOSI 23 // 2
#define LORA_SCK 18 // 2

RTC_DATA_ATTR short i = 0; // survives deep slepp

void setup() {
  Serial.begin(115200);
  delay(100);

  //SPI.begin(18,19,23,5);
}

void loop() {

  Serial.println("Wake Up");
  delay(5000);
  
   // VERY IMPORTANT for Deep Sleep Mode
  // Without switching PinMode to INPUT_PULLUP power consumption is 200uA higher!! 
  unsigned short gpios[] = {LORA_SS,LORA_SCK,LORA_MISO,LORA_MOSI};
  for( unsigned short n = 0; n < sizeof(gpios); n++ ){pinMode(gpios[n], INPUT_PULLUP);}

  pinMode(LORA_RST, INPUT_PULLUP);  // 400uA less in sleep mode
  delay(100);
  goToSleep(5, "DEEP");
}

void goToSleep(short seconds_to_sleep, char sleepMode[5]){
  sprintf(cTmp,"Sleeping %s for %i seconds",sleepMode, seconds_to_sleep);
  Serial.println(cTmp);
  delay(200); /* The previous command needs some time to complete */
  esp_sleep_enable_timer_wakeup(seconds_to_sleep * uS_TO_S_FACTOR);
  if (sleepMode == "LIGHT") {esp_light_sleep_start();}
  else { esp_deep_sleep_start(); }
}
