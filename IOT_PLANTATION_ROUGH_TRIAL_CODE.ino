//THIS IS A TRIAL CODE 




#include "Arduino.h"
#include "Servo.h"
#include "Buzzer.h"
#include "ESP8266.h"
#include "Joystick.h"
#include "LED.h"
#include "gfxfont.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "RGBLed.h"
#include "SoilMoisture.h"
#include "PiezoSpeaker.h"
#include "TMP102.h"


// Pin Definitions
#define BRUSHLESSMOTOR1_1_PIN_DATA  2
#define BRUSHLESSMOTOR2_2_PIN_DATA  3
#define BRUSHLESSMOTOR3_3_PIN_DATA  4
#define BUZZER_PIN_SIG  9
#define CAPACITIVETOUCH_PIN_SIG 11
#define JOYSTICK_1_PIN_SW 12
#define JOYSTICK_1_PIN_VRX  A10
#define JOYSTICK_1_PIN_VRY  A0
#define JOYSTICK_2_PIN_SW 13
#define JOYSTICK_2_PIN_VRX  A3
#define JOYSTICK_2_PIN_VRY  A13
#define LEDG_PIN_VIN  5
#define LEDR_PIN_VIN  6
#define NOKIALCD_PIN_CS 14
#define NOKIALCD_PIN_RST  16
#define NOKIALCD_PIN_DC 15
#define RGBLED_PIN_B  7
#define RGBLED_PIN_G  8
#define RGBLED_PIN_R  10
#define SOILMOISTURE_5V_PIN_SIG A11
#define THINSPEAKER_PIN_POS 17

//special thanks to circuit.io for providing this code 


// Global variables and defines
const int brushlessMotor1_1LowSpeed = 1000;  //Starting speed
const int brushlessMotor1_1FastSpeed = 2000; //Top speed
const int brushlessMotor2_2LowSpeed = 1000;  //Starting speed
const int brushlessMotor2_2FastSpeed = 2000; //Top speed
const int brushlessMotor3_3LowSpeed = 1000;  //Starting speed
const int brushlessMotor3_3FastSpeed = 2000; //Top speed
// ====================================================================
// vvvvvvvvvvvvvvvvvvvv ENTER YOUR WI-FI SETTINGS  vvvvvvvvvvvvvvvvvvvv
//
const char *SSID     = "WIFI-SSID"; // Enter your Wi-Fi name 
const char *PASSWORD = "PASSWORD" ; // Enter your Wi-Fi password
//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// ====================================================================
char* const host = "www.google.com";
int hostPort = 80;
//define Nokia LCD contrast and dimentions(in pixels)
#define LCD_CONTRAST 60
#define LCD_SIZE_COL 84
#define LCD_SIZE_ROW 48
int nokiaLcdCounter = 0;
#define rgbLed_TYPE COMMON_ANODE
unsigned int thinSpeakerHoorayLength          = 6;                                                      // amount of notes in melody
unsigned int thinSpeakerHoorayMelody[]        = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5}; // list of notes. List length must match HoorayLength!
unsigned int thinSpeakerHoorayNoteDurations[] = {8      , 8      , 8      , 4      , 8      , 4      }; // note durations; 4 = quarter note, 8 = eighth note, etc. List length must match HoorayLength!
// object initialization
Servo brushlessMotor1_1;
Servo brushlessMotor2_2;
Servo brushlessMotor3_3;
Buzzer buzzer(BUZZER_PIN_SIG);
ESP8266 wifi;
Joystick joystick_1(JOYSTICK_1_PIN_VRX,JOYSTICK_1_PIN_VRY,JOYSTICK_1_PIN_SW);
Joystick joystick_2(JOYSTICK_2_PIN_VRX,JOYSTICK_2_PIN_VRY,JOYSTICK_2_PIN_SW);
LED ledG(LEDG_PIN_VIN);
LED ledR(LEDR_PIN_VIN);
Adafruit_8x8matrix LedMatrixMini = Adafruit_8x8matrix();
Adafruit_PCD8544 nokiaLcd(NOKIALCD_PIN_DC,NOKIALCD_PIN_CS,NOKIALCD_PIN_RST);
RGBLed rgbLed(RGBLED_PIN_R,RGBLED_PIN_G,RGBLED_PIN_B,rgbLed_TYPE);
SoilMoisture soilMoisture_5v(SOILMOISTURE_5V_PIN_SIG);
PiezoSpeaker thinSpeaker(THINSPEAKER_PIN_POS);
TMP102 tmp102;


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    // WARNING! DO NOT CONNECT THE PROPELLER UNDER TEST!
    brushlessMotor1_1.attach(BRUSHLESSMOTOR1_1_PIN_DATA);
    brushlessMotor1_1.writeMicroseconds(brushlessMotor1_1LowSpeed);
    // WARNING! DO NOT CONNECT THE PROPELLER UNDER TEST!
    brushlessMotor2_2.attach(BRUSHLESSMOTOR2_2_PIN_DATA);
    brushlessMotor2_2.writeMicroseconds(brushlessMotor2_2LowSpeed);
    // WARNING! DO NOT CONNECT THE PROPELLER UNDER TEST!
    brushlessMotor3_3.attach(BRUSHLESSMOTOR3_3_PIN_DATA);
    brushlessMotor3_3.writeMicroseconds(brushlessMotor3_3LowSpeed);
    pinMode(CAPACITIVETOUCH_PIN_SIG, INPUT);
    wifi.init(SSID, PASSWORD);
    LedMatrixMini.begin(0x70); // pass in the address
    //Initialize Nokia instance
    nokiaLcd.begin(LCD_SIZE_COL, LCD_SIZE_ROW);
    nokiaLcd.setContrast(LCD_CONTRAST); //Adjust display contrast
    rgbLed.turnOff();              // Start with  LED Strip RGB turned off
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // A2212 Brushless Motor 1000KV (with 30A ESC) #1 - Test Code
    // The motor will rotate, statrting from low speed to high speed gradually.
    // WARNING! DO NOT CONNECT THE PROPELLER UNDER TEST!
    // In order to reverse the motor direction disconnect the power from the circuit and replace any 2 of the 3 blue wires between the motor and the ESC (marked: 30A OPTO)
    for(int brushlessMotor1_1RotationSpeed = brushlessMotor1_1LowSpeed; brushlessMotor1_1RotationSpeed <=  brushlessMotor1_1FastSpeed; brushlessMotor1_1RotationSpeed += 10)
    {
    brushlessMotor1_1.writeMicroseconds(brushlessMotor1_1RotationSpeed);  // 2. spin the motor at the rated speed. This number gradually changes during the 'for' loop
    Serial.println(brushlessMotor1_1RotationSpeed);
    delay(100);                              // 3. waits 100 milliseconds (0.1 sec). change the value in the brackets (100) for a longer or shorter delay in milliseconds.
    }
    for(int brushlessMotor1_1RotationSpeed = brushlessMotor1_1FastSpeed; brushlessMotor1_1RotationSpeed >=  brushlessMotor1_1LowSpeed; brushlessMotor1_1RotationSpeed -= 10)
    {
    brushlessMotor1_1.writeMicroseconds(brushlessMotor1_1RotationSpeed);  // 2. spin the motor at the rated speed. This number gradually changes during the 'for' loop
    Serial.println(brushlessMotor1_1RotationSpeed);
    delay(100);                              // 3. waits 100 milliseconds (0.1 sec). change the value in the brackets (100) for a longer or shorter delay in milliseconds.
    }
    brushlessMotor1_1.detach();                    // 4. release the brushless motor to conserve power and accidental operation.
    }
    else if(menuOption == '2') {
    // A2212 Brushless Motor 1000KV (with 30A ESC) #2 - Test Code
    // The motor will rotate, statrting from low speed to high speed gradually.
    // WARNING! DO NOT CONNECT THE PROPELLER UNDER TEST!
    // In order to reverse the motor direction disconnect the power from the circuit and replace any 2 of the 3 blue wires between the motor and the ESC (marked: 30A OPTO)
    for(int brushlessMotor2_2RotationSpeed = brushlessMotor2_2LowSpeed; brushlessMotor2_2RotationSpeed <=  brushlessMotor2_2FastSpeed; brushlessMotor2_2RotationSpeed += 10)
    {
    brushlessMotor2_2.writeMicroseconds(brushlessMotor2_2RotationSpeed);  // 2. spin the motor at the rated speed. This number gradually changes during the 'for' loop
    Serial.println(brushlessMotor2_2RotationSpeed);
    delay(100);                              // 3. waits 100 milliseconds (0.1 sec). change the value in the brackets (100) for a longer or shorter delay in milliseconds.
    }
    for(int brushlessMotor2_2RotationSpeed = brushlessMotor2_2FastSpeed; brushlessMotor2_2RotationSpeed >=  brushlessMotor2_2LowSpeed; brushlessMotor2_2RotationSpeed -= 10)
    {
    brushlessMotor2_2.writeMicroseconds(brushlessMotor2_2RotationSpeed);  // 2. spin the motor at the rated speed. This number gradually changes during the 'for' loop
    Serial.println(brushlessMotor2_2RotationSpeed);
    delay(100);                              // 3. waits 100 milliseconds (0.1 sec). change the value in the brackets (100) for a longer or shorter delay in milliseconds.
    }
    brushlessMotor2_2.detach();                    // 4. release the brushless motor to conserve power and accidental operation.
    }
    else if(menuOption == '3') {
    // A2212 Brushless Motor 1000KV (with 30A ESC) #3 - Test Code
    // The motor will rotate, statrting from low speed to high speed gradually.
    // WARNING! DO NOT CONNECT THE PROPELLER UNDER TEST!
    // In order to reverse the motor direction disconnect the power from the circuit and replace any 2 of the 3 blue wires between the motor and the ESC (marked: 30A OPTO)
    for(int brushlessMotor3_3RotationSpeed = brushlessMotor3_3LowSpeed; brushlessMotor3_3RotationSpeed <=  brushlessMotor3_3FastSpeed; brushlessMotor3_3RotationSpeed += 10)
    {
    brushlessMotor3_3.writeMicroseconds(brushlessMotor3_3RotationSpeed);  // 2. spin the motor at the rated speed. This number gradually changes during the 'for' loop
    Serial.println(brushlessMotor3_3RotationSpeed);
    delay(100);                              // 3. waits 100 milliseconds (0.1 sec). change the value in the brackets (100) for a longer or shorter delay in milliseconds.
    }
    for(int brushlessMotor3_3RotationSpeed = brushlessMotor3_3FastSpeed; brushlessMotor3_3RotationSpeed >=  brushlessMotor3_3LowSpeed; brushlessMotor3_3RotationSpeed -= 10)
    {
    brushlessMotor3_3.writeMicroseconds(brushlessMotor3_3RotationSpeed);  // 2. spin the motor at the rated speed. This number gradually changes during the 'for' loop
    Serial.println(brushlessMotor3_3RotationSpeed);
    delay(100);                              // 3. waits 100 milliseconds (0.1 sec). change the value in the brackets (100) for a longer or shorter delay in milliseconds.
    }
    brushlessMotor3_3.detach();                    // 4. release the brushless motor to conserve power and accidental operation.
    }
    else if(menuOption == '4') {
    // Buzzer - Test Code
    // The buzzer will turn on and off for 500ms (0.5 sec)
    buzzer.on();       // 1. turns on
    delay(500);             // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    buzzer.off();      // 3. turns off.
    delay(500);             // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }
    else if(menuOption == '5') {
    // Capacitive Touch Sensor Module - Test Code
    //Read Capacitive Touch Sensor. CapacitiveTouchVar will be '1' if the sensor will be touched
    bool CapacitiveTouchVar = digitalRead(CAPACITIVETOUCH_PIN_SIG);
    Serial.print(F("Capacitive Touch Sensor: ")); Serial.println(CapacitiveTouchVar);

    }
    else if(menuOption == '6') {
    // Logic Level Converter - Bi-Directional - Test Code
    //Send request for www.google.com at port 80
    wifi.httpGet(host, hostPort);
    // get response buffer. Note that it is set to 250 bytes due to the Arduino low memory
    char* wifiBuf = wifi.getBuffer();
    //Comment out to print the buffer to Serial Monitor
    //for(int i=0; i< MAX_BUFFER_SIZE ; i++)
    //  Serial.print(wifiBuf[i]);
    //search buffer for the date and time and print it to the serial monitor. This is GMT time!
    char *wifiDateIdx = strstr (wifiBuf, "Date");
    for (int i = 0; wifiDateIdx[i] != '\n' ; i++)
    Serial.print(wifiDateIdx[i]);

    }
    else if(menuOption == '7') {
    // PS2 X Y Axis Joystick Module #1 - Test Code
    // Read Joystick X,Y axis and press
    int joystick_1X =  joystick_1.getX();
    int joystick_1Y =  joystick_1.getY();
    int joystick_1SW =  joystick_1.getSW();
    Serial.print(F("X: ")); Serial.print(joystick_1X);
    Serial.print(F("\tY: ")); Serial.print(joystick_1Y);
    Serial.print(F("\tSW: ")); Serial.println(joystick_1SW);

    }
    else if(menuOption == '8') {
    // PS2 X Y Axis Joystick Module #2 - Test Code
    // Read Joystick X,Y axis and press
    int joystick_2X =  joystick_2.getX();
    int joystick_2Y =  joystick_2.getY();
    int joystick_2SW =  joystick_2.getSW();
    Serial.print(F("X: ")); Serial.print(joystick_2X);
    Serial.print(F("\tY: ")); Serial.print(joystick_2Y);
    Serial.print(F("\tSW: ")); Serial.println(joystick_2SW);

    }
    else if(menuOption == '9') {
    // LED - Basic Green 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
    ledG.dim(i);                      // 1. Dim Led 
    delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledG.off();                        // 3. turns off
    }
    else if(menuOption == '10') {
    // LED - Basic Red 5mm - Test Code
    // The LED will turn on and fade till it is off
    for(int i=255 ; i> 0 ; i -= 5)
    {
    ledR.dim(i);                      // 1. Dim Led 
    delay(15);                               // 2. waits 5 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    }                                          
    ledR.off();                        // 3. turns off
    }
    else if(menuOption == '11') {
    // Adafruit Mini 8x8 LED Matrix with I2C Backpack - Ultra Bright White - Test Code
    LedMatrixMini.setTextSize(1);
    LedMatrixMini.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
    LedMatrixMini.setTextColor(LED_ON);
    for (int i=0; i<4; i++) {
    LedMatrixMini.setRotation(i);
    for (int8_t x=0; x>=-36; x--) {
    LedMatrixMini.clear();
    LedMatrixMini.setCursor(x,0);
    LedMatrixMini.print("Circuito");
    LedMatrixMini.writeDisplay();
    delay(120);
    }
    }
    }
    else if(menuOption == '12') {
    // Graphic LCD 84x48 - Nokia 5110 - Test Code
    nokiaLcd.clearDisplay();             //Erase & clear display buffer
    nokiaLcd.setTextColor(BLACK);        //Set text color to black, background is white by default
    nokiaLcd.setTextSize(1);             //set text size
    nokiaLcd.println("circuito.io");     //print to screen
    nokiaLcd.println();                  
    nokiaLcd.setTextColor(WHITE, BLACK); //set text color to white and backgroung to black
    nokiaLcd.setTextSize(2);             //set text size
    nokiaLcd.println(nokiaLcdCounter++);          //print to screen
    nokiaLcd.display();                  //display on screen
    }
    else if(menuOption == '13') {
    // RGB Led Common Anode - Test Code
    // The RGB LED will turn PURPLE for 500ms(half a second) and turn off
    rgbLed.setRGB(160, 3, 255);    // 1. sets RGB LED color to purple. Change the values in the brackets to (255,0,0) for pure RED, (0,255,0) for pure GREEN and (0,0,255) for pure BLUE.
    delay(500);                         // 2. change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    rgbLed.setRGB(0, 0, 0);        // 3. turns RGB LED off (showing no color). Change the values in the brackets to alter the color.
    delay(500);                         // 4. change the value in the brackets (500) for a longer or shorter delay in milliseconds.  
    }
    else if(menuOption == '14') {
    // Soil Moisture Sensor - Test Code
    int soilMoisture_5vVal = soilMoisture_5v.read();
    Serial.print(F("Val: ")); Serial.println(soilMoisture_5vVal);

    }
    else if(menuOption == '15') {
    // Thin Speaker - Test Code
    // The Speaker will play the Hooray tune
    thinSpeaker.playMelody(thinSpeakerHoorayLength, thinSpeakerHoorayMelody, thinSpeakerHoorayNoteDurations); 
    delay(500);   
    }
    else if(menuOption == '16') {
    // SparkFun TMP102 - Digital Temperature Sensor Breakout - Test Code
    float tmp102TempC = tmp102.readCelsius();
    Serial.print(F("Temp: ")); Serial.print(tmp102TempC); Serial.println(F("[°C]"));

    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) A2212 Brushless Motor 1000KV (with 30A ESC) #1"));
    Serial.println(F("(2) A2212 Brushless Motor 1000KV (with 30A ESC) #2"));
    Serial.println(F("(3) A2212 Brushless Motor 1000KV (with 30A ESC) #3"));
    Serial.println(F("(4) Buzzer"));
    Serial.println(F("(5) Capacitive Touch Sensor Module"));
    Serial.println(F("(6) Logic Level Converter - Bi-Directional"));
    Serial.println(F("(7) PS2 X Y Axis Joystick Module #1"));
    Serial.println(F("(8) PS2 X Y Axis Joystick Module #2"));
    Serial.println(F("(9) LED - Basic Green 5mm"));
    Serial.println(F("(10) LED - Basic Red 5mm"));
    Serial.println(F("(11) Adafruit Mini 8x8 LED Matrix with I2C Backpack - Ultra Bright White"));
    Serial.println(F("(12) Graphic LCD 84x48 - Nokia 5110"));
    Serial.println(F("(13) RGB Led Common Anode"));
    Serial.println(F("(14) Soil Moisture Sensor"));
    Serial.println(F("(15) Thin Speaker"));
    Serial.println(F("(16) SparkFun TMP102 - Digital Temperature Sensor Breakout"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
          Serial.println(F("Now Testing A2212 Brushless Motor 1000KV (with 30A ESC) #1"));
        else if(c == '2') 
          Serial.println(F("Now Testing A2212 Brushless Motor 1000KV (with 30A ESC) #2"));
        else if(c == '3') 
          Serial.println(F("Now Testing A2212 Brushless Motor 1000KV (with 30A ESC) #3"));
        else if(c == '4') 
          Serial.println(F("Now Testing Buzzer"));
        else if(c == '5') 
          Serial.println(F("Now Testing Capacitive Touch Sensor Module"));
        else if(c == '6') 
          Serial.println(F("Now Testing Logic Level Converter - Bi-Directional"));
        else if(c == '7') 
          Serial.println(F("Now Testing PS2 X Y Axis Joystick Module #1"));
        else if(c == '8') 
          Serial.println(F("Now Testing PS2 X Y Axis Joystick Module #2"));
        else if(c == '9') 
          Serial.println(F("Now Testing LED - Basic Green 5mm"));
        else if(c == '10') 
          Serial.println(F("Now Testing LED - Basic Red 5mm"));
        else if(c == '11') 
          Serial.println(F("Now Testing Adafruit Mini 8x8 LED Matrix with I2C Backpack - Ultra Bright White"));
        else if(c == '12') 
          Serial.println(F("Now Testing Graphic LCD 84x48 - Nokia 5110"));
        else if(c == '13') 
          Serial.println(F("Now Testing RGB Led Common Anode"));
        else if(c == '14') 
          Serial.println(F("Now Testing Soil Moisture Sensor"));
        else if(c == '15') 
          Serial.println(F("Now Testing Thin Speaker"));
        else if(c == '16') 
          Serial.println(F("Now Testing SparkFun TMP102 - Digital Temperature Sensor Breakout"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}
