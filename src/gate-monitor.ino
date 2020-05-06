/*
 * Project gate-monitor
 * Description: monitor the gates!
 * Author: Donnie Barnes <djb@donniebarnes.com>
 * Date: May 6, 2020
 */

#include <LiquidCrystal_I2C_Spark.h>


void maingateHandler(const char *eventname, const char *data);
void clubgateHandler(const char *eventname, const char *data);
int mainstatus(String command);
int clubstatus(String command);

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display


void setup() {
    // listen for Input_1, the relay, to turn on or off from main gate ID and call a handler if it changes, same for clubhouse gate
    Particle.subscribe("main_gate_1", maingateHandler, MY_DEVICES);
    Particle.subscribe("clubhouse_gate_1", clubgateHandler, MY_DEVICES);

	Serial.begin(115200);
    lcd.init();  //initialize the lcd
    lcd.backlight();  //open the backlight 
  
    lcd.setCursor (0, 0 );            // go to the top left corner
    lcd.print("Main Gate: DUNNO"); // write this string on the top row
    lcd.setCursor (0, 1 );            // go to the third row
    lcd.print("CH Gate: DUNNO"); // pad with spaces for centering

}

void loop() {

    delay(1000);

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
         lcd.setCursor (0, 0);            // go to the 2nd row
         lcd.print("Main Gate: Open     "); // pad string with spaces for centering
    }
    if(command == "OFF")
    {
         lcd.setCursor (0, 0);            // go to the 2nd row
         lcd.print("Main Gate: Closed    "); // pad string with spaces for centering

    }
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
         lcd.setCursor (0, 1);            // go to the 2nd row
         lcd.print("CH Gate: Open     "); // pad string with spaces for centering
    }
    if(command == "OFF")
    {
         lcd.setCursor (0, 1);            // go to the 2nd row
         lcd.print("CH Gate: Closed    "); // pad string with spaces for centering

    }
}

