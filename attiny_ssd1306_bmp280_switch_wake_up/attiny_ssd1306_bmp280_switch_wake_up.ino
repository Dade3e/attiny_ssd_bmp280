// ATMEL ATTINY 25/45/85 / ARDUINO
//
//                  +-\/-+
// Ain0 (D 5) PB5  1|    |8  Vcc
// Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
// Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
//            GND  4|    |5  PB0 (D 0) pwm0
//                  +----+

//#include <TinyOzOLEDlimpfish.h>
#include <TinyOzOLED.h>
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

  OzOled.init();  //initialze OLED display
  
  OzOled.clearDisplay();           //clear the screen and set start position to top left corner
  OzOled.setNormalDisplay();       //Set display to Normal mode

  OzOled.sendCommand(0xA1);        // set Orientation
  OzOled.sendCommand(0xC8);
}

//This runs each time the watch dog wakes us up from sleep
ISR(PCINT0_vect) {
  //watchdog_counter++;
}

void loop() {

  OzOled.setPowerOn();
  OzOled.clearDisplay();
  displayTemp();
  delay(2000);
  OzOled.setPowerOff();

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
  int32_t temp = climateSensor.getTemperatureCelcius(true);
  float tmp1 = temp/100;
  float tmp2 = temp%100;
  char tmp[8];
  dtostrf(tmp1, 2, 1, tmp);
  OzOled.printBigNumber(tmp, 0, 0, 5);
  dtostrf(tmp2, 2, 0, tmp);
  OzOled.printBigNumber(tmp, 9, 0, 5);
}
