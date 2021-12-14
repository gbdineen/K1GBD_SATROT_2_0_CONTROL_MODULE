#ifndef MANUAL_INPUTS_H
#define MANUAL_INPUTS_H

#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>
#include <ArduinoJson.h>
#include "WS_Server.h"

class ManualInputs
{
    private:
        #define SS_SWITCH 24
        #define SS_NEOPIX 6
        #define SEESAW_BASE_ADDR 0x36
        // ROTARY ENCODERS
        int32_t encoder_position;
        uint8_t menuPos;
        uint16_t currPressedEnc = 999;
        bool manualServo;
        bool rollControl = false;
        bool encPressed = false;
        bool found_encoders[2] = {false, false};

        Adafruit_seesaw encoders[2];
        seesaw_NeoPixel encoder_pixels[2] = {
            seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800),
            seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800)};
        int32_t encoder_positions[2] = {0, 0};

        WS_Server * ws;

        void setEncPixelColorAll(uint32_t r, uint32_t g, uint32_t b);
        void setEncPixelColorSingle(uint8_t whatEnc, uint32_t r, uint32_t g, uint32_t b);
        void initRollControl(uint8_t whatEnc);
        void disableRollControl(uint8_t whatEnc);
        void initManualControl();
        void disableManualControl();

    public:
        ManualInputs();
        ManualInputs(WS_Server * wsServer);

        void initRotaryEncoders();
        void encoderCheck();
        void updateEncPos(uint8_t enc);
        ~ManualInputs();

};

#endif


