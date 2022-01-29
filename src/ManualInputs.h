#ifndef MANUAL_INPUTS_H
#define MANUAL_INPUTS_H

#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>
#include <ArduinoJson.h>
#include "WS_Server.h"
#include "DisplayControl.h"

// #define MANUAL 0
// #define AUTO 1
// #define UPD 2
#define MANUAL_SPEED 0 // speed control via rotary knobs
#define MANUAL_POSITION 1 // az/el changes updated via rotary knobs
#define UDP 2 // az/el updated via UDP packets
#define SS_SWITCH 24
#define SS_NEOPIX 6
#define SEESAW_BASE_ADDR 0x36
#define BTN_INPUT 39
#define AZIMUTH 0
#define ELEVATION 1
#define ROLL_MOTOR 2

class ManualInputs
{
    private:
        
        byte controlMethod = MANUAL_POSITION;
        int prevAz=false;
        int prevEl=false;
        int prevRoll=false;
        int dir;

        int currAz;
        int currEl;
        int currRoll;

        bool udpActive;
        
        // ROTARY ENCODERS
        int32_t az_encoder_position;
        int32_t el_encoder_position;
        int prevEncPos;
        int encInvPos;
        uint8_t menuPos;
        uint16_t currPressedEnc = 999;
        bool manualControl;
        bool buttonPressed = false;
        bool rollControl = false;
        bool azEncPressed = false;
        bool elEncPressed = false;
        bool found_encoders[2] = {false, false};

        Adafruit_seesaw encoders[2];
        seesaw_NeoPixel encoder_pixels[2] = {
            seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800),
            seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800)};
        int32_t encoder_positions[2] = {0, 0};

        WS_Server * ws;
        DisplayControl * dc;

        
        void initRotaryEncoders();
        void initPushButton();
        void encoderCheck();
        void azEncoderCheck();
        void elEncoderCheck();

        void buttonCheck();
        void updateEncPos(uint8_t enc, int dir);
        void initRollControl(uint8_t whatEnc);
        void disableRollControl(uint8_t whatEnc);
        void initManualControl();
        void disableManualControl();
        void setEncPixelColorAll(uint32_t r, uint32_t g, uint32_t b);
        void setEncPixelColorSingle(uint8_t whatEnc, uint32_t r, uint32_t g, uint32_t b);
        void getCurrPosition();

    public:
        ManualInputs();
        ManualInputs(WS_Server * wsServer, DisplayControl * disCtrl);
        
        byte getControlMethod();
        void setControlMethod(byte cm);
        void setTargets(int az, int el, int roll);
        void setCurrPosition(int az, int el, int roll);
        
        void begin();
        void loop();
        ~ManualInputs();

};

#endif


