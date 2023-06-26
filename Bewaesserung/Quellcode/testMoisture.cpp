#include "testMoisture.h"

SSD1306 display(OLED_ADDR, OLED_SDA, OLED_SCL);

#define MEAS_LOOPS 50
#define SLEEP_DURATION_S 7200

void setup() {
  Serial.begin(115200);
  Serial.println("setup()");

  pinMode(MOISTURE_PIN, ANALOG);
  pinMode(VCC_PIN,      ANALOG);

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0,  "Messung");
  display.display();

  //Sensor einschalten
  pinMode(SENSOR_PIN, OUTPUT);
  digitalWrite(SENSOR_PIN, PIN_AN);
  delay(1000);

  //messen
  int feuchte = feuchteMessen();
  pinMode(SENSOR_PIN, INPUT);
  if(feuchte >= 0){
    if(feuchte < 75){
      WasserMarsch();
    }
  }
  Serial.end();
  deepSleep(SLEEP_DURATION_S);
}

void loop() {
  deepSleep(SLEEP_DURATION_S);
/*
  int x=0;
  int y=0;

  for(int i=0; i<50; i++){
    x += analogRead(MOISTURE_PIN);
    y += analogRead(VCC_PIN);
    delay(10);
  }
  x = (x / 50);
  y = (y / 50);

  display.clear();
  display.drawString(0,  0,  "Sensor: " + String(x));
  display.drawString(0, 18,  "Batterie: " + String(y));
  display.display();
*/
}
int feuchteMessen(void){
  int64_t x = 0;
  int64_t y = 0;
  //Werte 50 mal messen
  for(int i=0; i<MEAS_LOOPS; i++){
    x += analogRead(MOISTURE_PIN);
    y += analogRead(VCC_PIN);
    delay(10);
  }
  //Durchschnitt berechnen
  x = (x / MEAS_LOOPS);
  y = (y / MEAS_LOOPS);
  //Batteriespannung
  float u_vcc = (float)y / 699 + 0.98;
  float u_vcc_2 = u_vcc * u_vcc; //Quadrat
  //Term ausrechnen
  float b = (1.5 * u_vcc_2) - (10.9 * u_vcc) + 19.86;
  //Messspannung ausrechnen
  float u_moi = (float)x / 1333 + b;
  //Arbeitspunkt bestimmen
  float u_moi_min = (1.125 * u_vcc_2) - (7.175 * u_vcc) + 11.9; //nass
  float u_moi_max = (2.5 * u_vcc_2) - (17.5 * u_vcc) + 33; //trocken
  Serial.println("u_moi_min: " + String(u_moi_min, 2));
  Serial.println("u_moi_max: " + String(u_moi_max, 2));
  float full_scale = u_moi_max - u_moi_min;
  float mess_wert = u_moi - u_moi_min;
  Serial.println("full_scale: " + String(full_scale, 2));
  Serial.println("mess_wert: " + String(mess_wert, 2));
  //Prozente ausrechnen
  int percent = mess_wert / full_scale * 100;
  if (percent < 0) { percent = 0; }
  if (percent > 100) { percent = 100; }
  percent = 100 - percent;
  //anzeigen
  display.clear();
  display.drawString(0,  0,  "Sensor: " + String(u_moi, 2));
  display.drawString(0, 18,  "Batterie: " + String(u_vcc, 2));
  if (u_moi < 0.25){
    display.drawString(0, 36,  "Sensor tot");
    display.display();
    delay(2000);
    return(-1);
  } else if (u_vcc < 3.6){
    display.drawString(0, 36,  "Batt. zu niedrig");
    display.display();
    delay(2000);
    return(-1);
  } else {
    display.drawString(0, 36,  "Feuchte: " + String(percent) + "%");
  }
  display.display();
  delay(2000);
  return(percent);
}

void WasserMarsch(void){
  display.clear();
  display.drawString(0, 0,  "Wasser");
  display.display();
  pinMode(PUMPE_PIN, OUTPUT);
  digitalWrite(PUMPE_PIN, PIN_AN);
  delay(3000);
  pinMode(PUMPE_PIN, INPUT);
}


void deepSleep(int sec){
  display.clear();
  display.drawString(0, 0,  "Gehe schlafen");
  display.display();
  delay(2000);

  display.displayOff();
  display.end();

  pinMode(OLED_SDA, INPUT);
  pinMode(OLED_RST,INPUT);
  pinMode(OLED_SCL,INPUT);
  pinMode(PUMPE_PIN, INPUT);
  pinMode(SENSOR_PIN,INPUT);
  pinMode(MOISTURE_PIN,INPUT);
  pinMode(VCC_PIN,INPUT);

  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,   ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_OFF);

  esp_deep_sleep(uint64_t(sec) * 1000 * 1000);
}
