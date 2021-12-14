#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h> 

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
        const int TFT1 = 27; // TFT 1 SPI address
        const int TFT2 = 14; // TFT 2 SPI address
        const int tft_W = 320;
        const int tft_H = 240;
        uint8_t fontPx = FONT_SIZE_PX;
        uint8_t fLines = round(tft_H/(fontPx+LINE_SPACING));
        uint8_t standardLineSize = fontPx+LINE_SPACING;

        TFT_eSPI tft = TFT_eSPI();
        TFT_eSprite spr = TFT_eSprite(&tft); 
        TFT_eSprite sprPos = TFT_eSprite(&tft); 
        TFT_eSprite sprEl = TFT_eSprite(&tft); 
        TFT_eSprite sprRoll = TFT_eSprite(&tft); 

        void initTFTs();

    public:

        DisplayControl();

};

#endif
