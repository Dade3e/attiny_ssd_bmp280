// ATMEL ATTINY 25/45/85 / ARDUINO
//
//                  +-\/-+
// Ain0 (D 5) PB5  1|    |8  Vcc
// Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
// Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//            GND  4|    |5  PB0 (D 0) pwm0
//                  +----+

//#include <TinyOzOLEDlimpfish.h>
#include <Tiny4kOLED.h>
#include "font16x32digits.h"
#include "7linedigital_font.h"

#include <TinyWireM.h>

#include <forcedClimate.h>

#include <avr/sleep.h> //Needed for sleep_mode
#include <avr/power.h>    // Power management

// ATtiny Pin 5 = SDA                   ATtiny Pin 6 = (D1) to LED2
// ATtiny Pin 7 = SCK                    ATtiny Pin 8 = VCC (2.7-5.5V)

// Create an instance of a climate sensor. The parameters I2C bus and I2C address are optional. For example:
// - ForcedClimate climateSensor = ForcedClimate(Wire, 0x76);
// - ForcedClimate climateSensor = ForcedClimate(TinyWireM, 0x77);
ForcedClimate climateSensor = ForcedClimate(TinyWireM, 0x76);

const byte SWITCH = 4; // pin 3 / PCINT4

void setup() {
  //analogReference(INTERNAL);      // use precise internal reference
  //ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA

  pinMode (SWITCH, INPUT);
  digitalWrite (SWITCH, HIGH);  // internal pull-up

  PCMSK  |= bit (PCINT4);  // want pin D4 / pin 3
  GIFR   |= bit (PCIF);    // clear any outstanding interrupts
  GIMSK  |= bit (PCIE);    // enable pin change interrupts

  climateSensor.begin();

  oled.begin(128, 32, sizeof(tiny4koled_init_128x32br), tiny4koled_init_128x32br);
  oled.setContrast(0x01);
  //oled.setInternalIref(0x8b);
  
  oled.clear();
}

//This runs each time the watch dog wakes us up from sleep
ISR(PCINT0_vect) {
  //watchdog_counter++;
}

void loop() {

  oled.on();
  
  displayTemp();

  oled.off();

  goToSleep ();
}

void goToSleep ()
  {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0;            // turn off ADC
  power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();                             
  sleep_disable();   
  power_all_enable();    // power everything back on
}  // end of goToSleep

void displayTemp() {
  climateSensor.begin();

  int32_t temp = climateSensor.getTemperatureCelcius();
  int32_t pres = climateSensor.getPressure();

  float tmp1 = temp/100;
  float tmp2 = temp%100;
  float tmp3 = tmp1+(tmp2/100);

  int p1 = pres/100;

  oled.clear();
  /*
  oled.setFont(FONT8X8MDOS);
  oled.setCursor(0, 2);
  oled.print("Temp:");
  oled.setFont(FONT16X32DIGITS);
  oled.setCursor(48, 0);
  oled.print(tmp3);*/

  
  oled.setFont(FONT16X32DIGITS);
  oled.setCursor(0, 0);
  oled.print(tmp3);
  oled.setFont(FONT7LINEDIGITAL);
  oled.print(" CELSIUS");

  delay(2000);

  /*oled.clear();
  oled.setFont(FONT8X8MDOS);
  oled.setCursor(0, 2);
  oled.print("Press:");
  oled.setFont(FONT16X32DIGITS);
  oled.setCursor(64, 0);
  oled.print(p1);

  delay(1000);*/
}
