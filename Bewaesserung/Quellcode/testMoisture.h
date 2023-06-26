/*
 * testMoisture.h
 *
 *  Created on: Mar 21, 2023
 *      Author: frank
 */

#ifndef TESTMOISTURE_H_
#define TESTMOISTURE_H_

#include <Arduino.h>
#include <Wire.h>
#include <SSD1306.h>


#define MOISTURE_PIN  36
#define VCC_PIN       37
#define SENSOR_PIN    22
#define PUMPE_PIN     19

#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define OLED_ADDR 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define PIN_AN  LOW
#define PIN_AUS HIGH


int  feuchteMessen(void);
void deepSleep(int sec);
void WasserMarsch(void);

#endif /* TESTMOISTURE_H_ */
