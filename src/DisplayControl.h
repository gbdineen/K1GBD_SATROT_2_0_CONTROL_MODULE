#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>

#define TFT1 27 // TFT 1 SPI address
#define TFT2 14 // TFT 2 SPI address

// #define APPLE_GOTHIC_20 "AppleGothic20"
// #define VERDANA_20 "Verdana20"
// #define VERDANA_18 "Verdana18"
// #define VERDANA_16 "Verdana16"
// #define VERDANA_14 "Verdana14"
// #define VERDANA_BOLD_14 "Verdana Bold14"
// #define FONT_SIZE_PX 18 // Font size 1 is 8px,  Font size 2 is size 1x2 (16px)
// #define LINE_SPACING 5
// #define tft_W 320
// #define tft_H 240
// #define FONT_SIZE_ACTIVE 2


class DisplayControl
{

    private:

        const String VERDANA_20 = "Verdana20";
        const String VERDANA_18 = "Verdana18";
        const String VERDANA_16 = "Verdana16";
        const String VERDANA_14 = "Verdana14";
        const String VERDANA_BOLD_14 = "Verdana Bold14";
        const int FONT_SIZE_PX = 18;
        const int LINE_SPACING = 5;
        const int tft_W = 320;
        const int tft_H = 240;
        uint8_t fontPx = FONT_SIZE_PX;
        uint8_t fLines = round(tft_H/(fontPx+LINE_SPACING));
        uint8_t standardLineSize = fontPx+LINE_SPACING;

        TFT_eSPI tft = TFT_eSPI();
        // TFT_eSprite spr = TFT_eSprite(&tft); 
        // TFT_eSprite sprPos = TFT_eSprite(&tft); 
        // TFT_eSprite sprEl = TFT_eSprite(&tft); 
        // TFT_eSprite sprRoll = TFT_eSprite(&tft); 

    public:

        DisplayControl();
        ~DisplayControl();
        void begin();
        // void displayPosition();
        // void displayPositionLabels();

};

#endif
