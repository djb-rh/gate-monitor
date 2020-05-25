/*
 * Project gate-monitor
 * Description: monitor the gates!
 * Author: Donnie Barnes <djb@donniebarnes.com>
 * Date: May 6, 2020
 */

#include <LiquidCrystal_I2C_Spark.h>
#include <clickButton.h>
#include <neopixel.h>

int out = D7;

// use WS2811 in the Particle library because these (8mm diffused Adafruit 1734) are RGB addressed, whereas the other types are GRB
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(2, out, WS2811);

void maingateHandler(const char *eventname, const char *data);
void clubgateHandler(const char *eventname, const char *data);
int mainstatus(String command);
int clubstatus(String command);

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

int buttonZeroPin = D3;
int buttonOnePin = D4;

ClickButton buttonZero(buttonZeroPin, LOW, CLICKBTN_PULLUP);
ClickButton buttonOne(buttonOnePin, LOW, CLICKBTN_PULLUP);

int buttonZeroClicks = 0;
int buttonOneClicks = 0;

int buttonZeroToggle = 0;
int buttonOneToggle = 0;

void setup() {

    pinMode(buttonZeroPin, INPUT_PULLUP);
    pinMode(buttonOnePin, INPUT_PULLUP);

    buttonZero.debounceTime = 20;
    buttonZero.multiclickTime = 250;
    buttonZero.longClickTime = 1000;

    buttonOne.debounceTime = 20;
    buttonOne.multiclickTime = 250;
    buttonOne.longClickTime = 1000;

    pixels.begin();
    // blue is don't know
    pixels.setPixelColor(0, 0,0,255);
    pixels.setPixelColor(1, 0,0,255);
    // pixels.setBrightness(50);
    pixels.show();
    // listen for Input_1, the relay, to turn on or off from main gate ID and call a handler if it changes, same for clubhouse gate
    Particle.subscribe("main_gate_1", maingateHandler, MY_DEVICES);
    // Particle.subscribe("light_1", maingateHandler, MY_DEVICES);
    Particle.subscribe("clubhouse_gate_1", clubgateHandler, MY_DEVICES);
    // Particle.subscribe("light_1", clubgateHandler, MY_DEVICES);

    // do a request for the state and the above subscribes will get called when they see the request
    Particle.publish("getstate", "1", PRIVATE);

	Serial.begin(115200);
    lcd.init();  //initialize the lcd
    lcd.backlight();  //open the backlight 
  
    lcd.setCursor (0, 0 );            // go to the top left corner
    lcd.print("CH Gate: DUNNO"); // pad with spaces for centering
    // lcd.print("Light: DUNNO"); // write this string on the top row
    lcd.setCursor (0, 1 );            // go to the third row
    lcd.print("Main Gate: DUNNO"); // write this string on the top row

}

void loop() {

    buttonZero.Update();
    buttonOne.Update();

    if(buttonZero.clicks != 0) buttonZeroClicks = buttonZero.clicks;

    if (buttonZeroClicks == 1)
    {
        if (buttonZeroToggle) {
            // Particle.publish("light_relay_1", "1off", PRIVATE);
            Particle.publish("main_gate_relay_1", "1off", PRIVATE);
            buttonZeroToggle = 0;
        }
        else {
            // Particle.publish("light_relay_1", "1on", PRIVATE);
            Particle.publish("main_gate_relay_1", "1on", PRIVATE);
            buttonZeroToggle = 1;
        }
    }

    if(buttonOne.clicks != 0) buttonOneClicks = buttonOne.clicks;

    if (buttonOneClicks == 1)
    {
        if (buttonOneToggle) {
            // Particle.publish("light_relay_1", "1off", PRIVATE);
            Particle.publish("clubhouse_gate_relay_1", "1off", PRIVATE);
            buttonOneToggle = 0;
        }
        else {
            // Particle.publish("light_relay_1", "1on", PRIVATE);
            Particle.publish("clubhouse_gate_relay_1", "1on", PRIVATE);
            buttonOneToggle = 1;
        }
    }

    buttonZeroClicks = 0;
    buttonOneClicks = 0;
    // delay(500);

}

void maingateHandler(const char *eventname, const char *data)
{
    // pass the command that was sent to the main gate down to us
    mainstatus(data);
}

int mainstatus(String command)
{
    if(command == "ON")
    {
         lcd.setCursor (0, 1);            
         // lcd.print("Main Gate: Open     "); 
         lcd.print("Light: Open     ");
         // pixels.setPixelColor(0, 0,255,0);
         pixels.setPixelColor(0, 0,55,0);
         buttonZeroToggle = 1;
    }
    if(command == "OFF")
    {
         lcd.setCursor (0, 1);         
         lcd.print("Main Gate: Closed    "); 
         // lcd.print("Light: Closed    "); 
         // pixels.setPixelColor(0, 255,0,0);
         pixels.setPixelColor(0, 55,0,0);
         buttonZeroToggle = 0;

    }
    pixels.show();
}

void clubgateHandler(const char *eventname, const char *data)
{
    // pass the command that was sent to the clubhouse gate down to us
    clubstatus(data);
}

int clubstatus(String command)
{
    if(command == "ON")
    {
         lcd.setCursor (0, 0);          
         lcd.print("CH Gate: Open     "); 
         // pixels.setPixelColor(1, 0,255,0);
         pixels.setPixelColor(1, 0,55,0);
         buttonOneToggle = 1;
    }
    if(command == "OFF")
    {
         lcd.setCursor (0, 0); 
         lcd.print("CH Gate: Closed    ");
         // pixels.setPixelColor(1, 255,0,0);
         pixels.setPixelColor(1, 55,0,0);
         buttonOneToggle = 0;

    }
    pixels.show();
}

