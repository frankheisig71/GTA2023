/*
 * HTMLSource.h
 *
 *  Created on: Jun 21, 2023
 *      Author: frank
 */

#ifndef TEST_HTML_H_
#define TEST_HTML_H_

#include <Arduino.h>

String siteHeader[] = {
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    "HTTP/1.1 200 OK",
    "Content-Type:text/html",
    "Connection: close",
    "",
    // Display the HTML web page
    "<!DOCTYPE html><html>",
    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">",
    "<link rel=\"icon\" href=\"data:,\">",
    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences
    "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}",
    ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;",
    "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}",
    ".button2 {background-color: #77878A;}</style></head>",
    "<body><h1>GTA Web Server</h1><br>"
};

int siteHeader_length = sizeof(siteHeader)/sizeof(siteHeader[0]);




#endif /* TEST_HTML_H_ */
