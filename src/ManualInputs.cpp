#include "ManualInputs.h"


ManualInputs::ManualInputs()
{
    
}

ManualInputs::ManualInputs(WS_Server * wsServer)
{
    this->ws = wsServer;

	auto TgPtr = std::bind(&ManualInputs::setTargets, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	ws->setTargetsCallback(TgPtr);

	auto PoPtr = std::bind(&ManualInputs::setCurrPosition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	ws->setPositionCallback(PoPtr);
}

void ManualInputs::begin()
{
	initRotaryEncoders();
	initPushButton();
}

void ManualInputs::initRotaryEncoders() {
	
  //Serial.println("Looking for seesaws!");
  for (uint8_t enc=0; enc<sizeof(found_encoders); enc++) {
    if (! encoders[enc].begin(SEESAW_BASE_ADDR + enc) ||  // See if we can find encoders on this address 
        ! encoder_pixels[enc].begin(SEESAW_BASE_ADDR + enc)) {
     Serial.print("Couldn't find encoder #");
     Serial.println(enc);
    } else {
      //Serial.print("Found encoder + pixel #");
      //Serial.println(enc);
      
      uint32_t version = ((encoders[enc].getVersion() >> 16) & 0xFFFF);
      if (version != 4991){
       // Serial.print("Wrong firmware loaded? ");
       Serial.println(version);
      //  while(1) delay(10);
      }
      //Serial.println("Found Product 4991");
  
      encoders[enc].pinMode(SS_SWITCH, INPUT_PULLUP); // use a pin for the built in encoder switch
      encoder_positions[enc] = encoders[enc].getEncoderPosition(); // get starting position
      //Serial.println("Turning on interrupts");
      delay(10);
      encoders[enc].setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
      encoders[enc].enableEncoderInterrupt();
      encoder_pixels[enc].setBrightness(255); // set to bright af
      encoder_pixels[enc].setPixelColor(0, 255, 0, 0);
      encoder_pixels[enc].show();
      
      found_encoders[enc] = true;
    }
  }
}

void ManualInputs::initPushButton() {
  pinMode(BTN_INPUT,INPUT);
  //pinMode(RST_INPUT,INPUT_PULLUP);
}

void ManualInputs::encoderCheck() {

  for (uint8_t enc=0; enc<sizeof(found_encoders); enc++) { 
    if (found_encoders[enc] == false) continue;
    int32_t new_position = encoders[enc].getEncoderPosition();

	if (encoder_positions[enc] != new_position) {  // Check for encoder dial movement

	//new_position= -(new_position);  // Make sure clockwise turns are positive. Default is opposite for some reason. 
    

	Serial.print("\n");
	Serial.print("=====================================>encoder_position: "); Serial.println(encoder_position);
	Serial.print("=====================================>new_position: "); Serial.println(new_position);
	
		
		//digitalWrite(TFT1, HIGH);
		//digitalWrite(TFT2, LOW);
		encoder_positions[enc] = new_position;
		if (new_position>encoder_position && new_position!=0)
		{
			dir=0;
			if (controlMethod==MANUAL_SPEED)
			{
				if (new_position>0)
				{
					encoders[enc].setEncoderPosition(1);
				}
			}
			else if (controlMethod==MANUAL_POSITION)
			{
				if (new_position<1)
				{
					encoders[enc].setEncoderPosition(1);
				}
				else
				{
					if (enc==AZIMUTH_SERVO)
					{
						if (new_position>358)
						{
							encoders[enc].setEncoderPosition(-359);	
						}
					}
					else if (enc==ELEVATION_SERVO)
					{
						if (new_position>89)
						{
							encoders[enc].setEncoderPosition(-90);	
						}
					}
				}
			}
			updateEncPos(enc, dir);
	  	}
		else if (new_position<encoder_position && new_position!=0)
		{
			dir=1;
			if (controlMethod==MANUAL_SPEED)
			{
				if (new_position<0)
				{
					encoders[enc].setEncoderPosition(-1);
				}
			}
			else if (controlMethod==MANUAL_POSITION)
			{
				if (new_position<1)
				{
					encoders[enc].setEncoderPosition(0);
				}
				else
				{
					if (enc==AZIMUTH_SERVO)
					{
						if (new_position<1)
						{
							encoders[enc].setEncoderPosition(0);	
						}
					}
					else if (enc==ELEVATION_SERVO)
					{
						if (new_position<1)
						{
							encoders[enc].setEncoderPosition(0);	
						}
					}
				}
			}
			updateEncPos(enc, dir);
		}
		else if (new_position==0)
		{
			dir = 2;
			updateEncPos(enc, dir);
		}
	}
    if (! encoders[enc].digitalRead(SS_SWITCH)) {
      
      if (!encPressed) {
        encPressed=true;
        if (enc!=currPressedEnc && !rollControl) {
          currPressedEnc=enc;
          rollControl=true;
       } else if (enc==currPressedEnc) {
          rollControl=false;
        }
        Serial.print("currPressedEnc "); Serial.print(enc); Serial.print(", rollControl "); Serial.println(rollControl);
        ws->broadcastToClient("pressed"); 
      }
    } else if (encoders[enc].digitalRead(SS_SWITCH)) {
      if (rollControl && enc==currPressedEnc) {
        initRollControl(enc);
        encPressed=false;
      } else if (!rollControl && enc==currPressedEnc) {
        disableRollControl(enc);
        currPressedEnc=999;
        encPressed=false;
      }
    }
     encoder_position=new_position;
  } 
}

void ManualInputs::buttonCheck()
{
	uint8_t btnState = digitalRead(BTN_INPUT);
	//uint8_t rstBtnState = digitalRead(RST_INPUT);
  
	// Check silver button for manual servo control
	if (btnState==HIGH)
	{
		if (controlMethod==MANUAL_POSITION)
		{
			setControlMethod(MANUAL_SPEED);
		}
   }
	else if (btnState==LOW)
	{
		// if (manualControl)
		// {
		if (controlMethod==MANUAL_SPEED)
		{
			setControlMethod(MANUAL_POSITION);
		}
		//disableManualControl();
		// if (!prevAz || !prevEl|| !prevRoll)
		// {
		// 	for (uint8_t enc=0; enc<sizeof(found_encoders); enc++)
		// 	{ 
		// 		encoders[enc].setEncoderPosition(0); // Set encoders to zero so as to jerk the antenna around
		// 	}
		// }
		// else
		// {
		// 	encoders[0].setEncoderPosition(prevAz);
		// 	encoders[1].setEncoderPosition(prevEl);
		// }
      		
		//}
	}
}

void ManualInputs::updateEncPos(uint8_t enc, int dirLoc) {

    StaticJsonDocument<200> encObj;
    if (controlMethod==MANUAL_SPEED)
    {

		
		int servoSpeed;

		if (enc==0)
		{
			servoSpeed=9;
		}
		else if (enc==1)
		{
			servoSpeed=9;
		}
		
		encObj["Subject"] = "MANUAL_SPEED";
		encObj["Servo"] = enc;
		encObj["Speed"] = servoSpeed;
		encObj["RollControl"] = rollControl;
		encObj["Direction"] = dirLoc;
	}
	else if (controlMethod==MANUAL_POSITION)
	{
		encObj["Subject"] = "MANUAL_POSITION";
		encObj["Azimuth"] = encoders[0].getEncoderPosition();
		encObj["Elevation"] = encoders[1].getEncoderPosition();;
	}	
    String encStr;
    serializeJson(encObj, encStr);
    Serial.print("ManualInputs::updateEncPos "); Serial.println(encStr);
    ws->broadcastToClient(encStr); 
	//prevEncPos=new_position;
}

void ManualInputs::initRollControl(uint8_t whatEnc) {
    
  setEncPixelColorSingle(whatEnc,0,0,255);

//   digitalWrite(TFT2, LOW);
//   String txt = "Roll control active on knob ";
//   uint16_t tw = tft.textWidth(txt);
//   uint16_t x = (tft.width()-tw)/2;
//   uint16_t h = tft.height()-fontPx;
//   int16_t y = tft.height()/2;
//   spr.deleteSprite();
//   spr.createSprite(tft.width(), fontPx+10);
//   spr.fillSprite(TFT_BLUE);
//   spr.loadFont(VERDANA_BOLD_14);
//   spr.setTextColor(TFT_SKYBLUE);
//   spr.setTextDatum(CC_DATUM);
//   spr.drawString(txt+whatEnc,tft.width()/2, fontPx/2);
//   spr.pushSprite(0,h);
//   digitalWrite(TFT2, HIGH);
  rollControl=true;

}

void ManualInputs::disableRollControl(uint8_t whatEnc) {

  if (controlMethod==MANUAL_SPEED) {
    setEncPixelColorSingle(whatEnc,0,255,0);
  } else {
    setEncPixelColorSingle(whatEnc,255,0,0);
  }

//   // Send command to Client to stop moving Roll servo
//   StaticJsonDocument<100> doc;
//   // create an object
//   JsonObject object = doc.to<JsonObject>();
//   object["Subject"] = "disablerollcontrol";
//   serializeJson(doc, jsonString); // serialize the object and save teh result to teh string variable.
//   //Serial.println( jsonString ); // print the string for debugging.
//   webSocket.broadcastTXT(jsonString); // send the JSON object through the websocket
//   jsonString = ""; // clear the String.

//   digitalWrite(TFT2, LOW);
//   String txt = "Roll control inactive";
//   uint16_t tw = tft.textWidth(txt);
//   uint16_t x = (tft.width()-tw)/2;
//   uint16_t h = tft.height()-fontPx;
//   int16_t y = tft.height()/2;
//   spr.deleteSprite();
//   spr.createSprite(tft.width(), fontPx+10);
//   spr.fillSprite(TFT_SKYBLUE);
//   spr.loadFont(VERDANA_BOLD_14);
//   spr.setTextColor(TFT_NAVY);
//   spr.setTextDatum(CC_DATUM);
//   spr.drawString(txt,tft.width()/2, fontPx/2);
//   spr.pushSprite(0,h);
//   digitalWrite(TFT2, HIGH);
  rollControl=false;

}

void ManualInputs::initManualControl() {
  
  //if (!manualServo) {
    
	setEncPixelColorAll(0,255,0);

	setControlMethod(MANUAL_SPEED);
	
	// StaticJsonDocument<200> obj;
	// obj["Subject"] = "controlmethod";
  //  obj["ControlMethod"] = 0; // 0 = MANUAL as defined in PositionControl on the tracker side
	// String str;
  //   serializeJson(obj, str);
    
	// ws->broadcastToClient(str); 



    // digitalWrite(TFT1, LOW);
    // String txt = "Manual servo axctive";
    // uint16_t tw = tft.textWidth(txt);
    // uint16_t x = (tft.width()-tw)/2;
    // uint16_t h = tft.height()-fontPx;
    // int16_t y = tft.height()/2;
    // spr.deleteSprite();
    // spr.createSprite(tft.width(), fontPx+10);
    // spr.fillSprite(TFT_GREEN);
    // spr.loadFont(VERDANA_BOLD_14);
    // spr.setTextColor(TFT_DARKGREEN);
    // spr.setTextDatum(CC_DATUM);
    // spr.drawString(txt,tft.width()/2, fontPx/2);
    // spr.pushSprite(0,h);
    // digitalWrite(TFT1, HIGH);
    manualControl=true;
  //}
}

void ManualInputs::disableManualControl() {
  //if (manualServo) {

  setEncPixelColorAll(255,0,0);

  setControlMethod(MANUAL_POSITION);

	// StaticJsonDocument<200> obj;
	// obj["Subject"] = "controlmethod";
  // obj["ControlMethod"] = 1; // 1 = AUTO as defined in PositionControl on the tracker side
	// String str;
  // serializeJson(obj, str);
  // ws->broadcastToClient(str); 

    // digitalWrite(TFT1, LOW);
    // String txt = "Manual servo inactive";
    // uint16_t tw = tft.textWidth(txt);
    // uint16_t x = (tft.width()-tw)/2;
    // uint16_t h = tft.height()-fontPx;
    // int16_t y = tft.height()/2;
    // spr.deleteSprite();
    // spr.createSprite(tft.width(), fontPx+10);
    // spr.fillSprite(TFT_RED);
    // spr.loadFont(VERDANA_BOLD_14);
    // spr.setTextColor(0x6800);
    // spr.setTextDatum(MC_DATUM);
    // spr.drawString(txt,tft.width()/2, fontPx/2);
    // spr.pushSprite(0,h);
    // digitalWrite(TFT1, HIGH);
    
    manualControl=false;
  //}
}

void ManualInputs::setEncPixelColorAll(uint32_t r, uint32_t g, uint32_t b) {
  for (uint8_t enc=0; enc<=1; enc++) {
    encoder_pixels[enc].setPixelColor(0,r,g,b);
    encoder_pixels[enc].setBrightness(255);
    encoder_pixels[enc].show();
  }
}

void ManualInputs::setEncPixelColorSingle(uint8_t whatEnc, uint32_t r, uint32_t g, uint32_t b) {
  encoder_pixels[whatEnc].setPixelColor(0,r,g,b);
  encoder_pixels[whatEnc].setBrightness(255);
  encoder_pixels[whatEnc].show();
}

byte ManualInputs::getControlMethod()
{
	return this->controlMethod;
}

void ManualInputs::setControlMethod(byte cm)
{
	
	this->controlMethod=cm;
	Serial.print("======== setControlMethod: "); Serial.println(cm);
	
	if (cm==MANUAL_SPEED)
	{
		setEncPixelColorAll(0,255,0);	
		for (uint8_t enc=0; enc<sizeof(found_encoders); enc++)
		{ 
			encoders[enc].setEncoderPosition(0); // Set encoders to zero so as to jerk the antenna around
		}
	}
	else if (cm==MANUAL_POSITION)
	{
		setEncPixelColorAll(255,0,0);
		// Serial.println("");
		// Serial.println("====== PREVIOUS TARGETS ======");
		// Serial.print("prevAZ: "); Serial.print(prevAz); Serial.print(" prevEl: "); Serial.print(prevEl); Serial.print(" prevRoll: "); Serial.print(prevRoll); 
		
		getCurrPosition();
		encoders[0].setEncoderPosition(currAz);
		encoders[1].setEncoderPosition(currEl);
		// if (!prevAz || !prevEl|| !prevRoll)
		// {
		// 	for (uint8_t enc=0; enc<sizeof(found_encoders); enc++)
		// 	{ 
		// 		encoders[enc].setEncoderPosition(0); // Set encoders to zero so as to jerk the antenna around
		// 	}
		// }
		// else
		// {
			
		// 	encoders[0].setEncoderPosition(prevAz);
		// 	encoders[1].setEncoderPosition(prevEl);
		// }
	}

	StaticJsonDocument<200> obj;
	obj["Subject"] = "controlmethod";
	obj["ControlMethod"] = cm;
	String str;
	serializeJson(obj, str);
	ws->broadcastToClient(str);
	Serial.println(str);
}

void ManualInputs::setTargets(int az, int el, int roll)
{

	Serial.print("\n----- [Setting Targets 4] -----]\n");
	
	
	this->prevAz = -az; // Negative because of course
	this->prevEl = -el;
	this->prevRoll = roll;
}

void ManualInputs::getCurrPosition()
{
	
	
	//ws->getCurrPosition();
	StaticJsonDocument<200> obj;
	obj["Subject"] = "get-position";
	String str;
	serializeJson(obj, str);
	ws->broadcastToClient(str);
	Serial.println(str);
}

void ManualInputs::setCurrPosition(int az, int el, int roll) 
{
	currAz = az;
    currEl = el;
    currRoll = roll;
	Serial.print("setCurrPosition --------> ");
	Serial.print("currAz: "); Serial.print(currAz);
	Serial.print(" currEl: "); Serial.print(currEl);
	Serial.print(" currRoll "); Serial.println(currRoll);
}

void ManualInputs::loop()
{
	encoderCheck();
	buttonCheck();
}

ManualInputs::~ManualInputs()
{
}