#include "DisplayControl.h"

DisplayControl::DisplayControl()
{

}

void DisplayControl::initTFTs()
{
    if (!SPIFFS.begin()) {
        //Serial.println("SPIFFS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
    }
    //Serial.println("\r\nSPIFFS available!");
    
    // ESP32 will crash if any of the fonts are missing
    bool font_missing = false;
    if (SPIFFS.exists("/Verdana20.vlw")    == false) font_missing = true;
    if (SPIFFS.exists("/Verdana18.vlw")    == false) font_missing = true;
    if (SPIFFS.exists("/Verdana Bold14.vlw")    == false) font_missing = true;

    if (font_missing)
    {
        //Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
        while(1) yield();
    }
    else //Serial.println("\r\nFonts found OK.");

    pinMode(TFT1, OUTPUT);
    digitalWrite(TFT1, HIGH);
    
    pinMode(TFT2, OUTPUT);
    digitalWrite(TFT2, HIGH);
    
    // We need to 'init' both tfts
    // at the same time. so set both cs pins low
    digitalWrite(TFT1, LOW);
    digitalWrite(TFT2, LOW);
    
    tft.begin();
    tft.setRotation(3);  
    tft.loadFont(VERDANA_18);
    tft.fillScreen(TFT_BLACK);

    // Set both cs pins HIGH, or 'inactive'
    digitalWrite(TFT1, HIGH);
    digitalWrite(TFT2, HIGH);
}