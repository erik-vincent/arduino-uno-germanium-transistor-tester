#include <Adafruit_ADS1X15.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

float computeMilliVolts(int16_t counts);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

// These constants won't change. They're used to give names to the pins used:
// constants won't change. They're used here to set pin numbers:
const int emitterPin = 2;           // arduino pin the emitter pin of the transistor is on
const int basePin = 3;              // arduino pin the base pin of the transistor is on
const int collectorPin = 5;         // arduino pin the collector pin of the transistor is on
const int collectorResistorPin = 6; // arduino pin the collector resistor is on
const int baseResistorPin = 7;      // arduino pin the base resistor is on

// variables will change:
int collectorPinState = 0;  // variable for reading the collector pin status
int transistorType = 0;     // Type 0 is NPN, Type 1 is PNP

void setup() {
  int16_t adc0, adc1;               // variables to hold the 16-bit ADC reading
  float collector_milliVolts = 0.0; // variable to hold the collector voltage in milli-volts
  float rail_milliVolts = 0.0;      // variable to hold the rail voltage in milli-volts
  float leak_milliVolts = 0.0;      // variable to hold leakage voltage in milli-volts
  float leak_uA = 0.0;              // variable to hold leakage current in micro-amps
  float baseCurrent_uA = 0.0;       // variable to hold the base current in micro-amps
  float gain = 0.0;                 // variable to hold the gain (which includes the leakage)
  float trueGain = 0.0;             // variable to hold the true gain (subtracting leakage)

  // below are the results of resistance of the resistors used on this board.
  int16_t collector_resistor = 997; // collector resistor 1K is really 0.997K
  long base_resistor = 1204000;     // base resistor 1.2M is really 1.204M
  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  // In this step, we will set the emitter pin to GND and the base pin
  // to 5 volts. So, we need to set the emitter and base pins to output
  // mode, so we can connect them to those power rails. The collector
  // pin will need to be set to input, as we will use this to see if the
  // transistor is an NPN or PNP germanium transistor
  pinMode(emitterPin, OUTPUT);
  pinMode(basePin, OUTPUT);
  pinMode(collectorPin, INPUT);
  
  digitalWrite(emitterPin, LOW);  // set the emitter pin to GND
  digitalWrite(basePin, HIGH);    // set the base pin to 5V

  delay(50);  // wait a moment

  // see what the voltage is on the collector pin
  collectorPinState = digitalRead(collectorPin);

  // based off if voltage was detected on the collector pin...
  if (collectorPinState == HIGH) {
    // if we saw the voltage from the base pin jump over to the collector pin,
    // it's a PNP transistor (type 1)
    transistorType = 1;
  } else {
    // otherwise, the voltage from the base pin did not jump to the collector
    // pin, it's an NPN transistor (type 0)
    transistorType = 0;

    // if it is a silicon PNP transistor, this will have the same effect as an NPN
    // so it doesn't work.
  }

  delay(50);  // wait a moment

  // reset the emitter and base pins and set them back to floating
  digitalWrite(emitterPin, LOW);
  pinMode(emitterPin, INPUT);
  digitalWrite(basePin, LOW);
  pinMode(basePin, INPUT);

  delay(50);  // wait a moment

  // Now that we know if we have an NPN or PNP transistors, we can begin to read leak
  // of the transistor.
  // if it is a PNP transistor, emitter goes to 5V and collector goes through the
  // collector resistor to ground.
  // if it is an NPN transistor, emitter goes to ground and collector goes through the
  // collector resistor to 5V.
  if(transistorType == 1) {
    // Transistor is PNP
    pinMode(emitterPin, OUTPUT);          // Set emitter pin to output so that we can...
    digitalWrite(emitterPin, HIGH);       // Emitter goes straight to 5V
    pinMode(collectorResistorPin, OUTPUT);
    digitalWrite(collectorResistorPin, LOW); // Collector goes to ground via collector resistor
  } else {
    // Transistor is NPN
    pinMode(emitterPin, OUTPUT);         // Set emitter pin to output so that we can...
    digitalWrite(emitterPin, LOW);       // Emitter goes straight to ground
    pinMode(collectorResistorPin, OUTPUT);
    digitalWrite(collectorResistorPin, HIGH); // Collector goes to 5V via collector resistor
  }

  // read in voltages from the 16-bit ADC
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);

  // rail voltage goes into ADC0 and collector voltage goes into ADC1
  rail_milliVolts = computeMilliVolts(adc0);
  collector_milliVolts = computeMilliVolts(adc1);

  // ohms law, V / R = I. Because we want current in uA and the volts are in milliVolts,
  // we need to multiply by 1000.0. This will give us the current from the base pin.
  // we have not applied base current at this point, but will do so later on.
  baseCurrent_uA = (rail_milliVolts * 1000.0) / base_resistor;

  if(transistorType == 1) {
    // Transistor is PNP

    // in case we get any weird jitter on the collector pin
    if(collector_milliVolts <= 0.1) {
      collector_milliVolts = 0.0;
    }

    // the leakage has caused an amount of collector voltage that we need to record and subtract
    // from the gain, when we get to that point.
    leak_milliVolts = collector_milliVolts;

    // ohms law, V / R = I. Because we want current in uA and the volts are in milliVolts,
    // we need to multiply by 1000.0. This will give us the current from the collector pin.
    leak_uA = (leak_milliVolts * 1000.0) / collector_resistor;

    // Output our findings thus far out the serial port
    Serial.println("PNP Transistor");
    Serial.print("PWR Rail:   ");
    Serial.print(rail_milliVolts);
    Serial.println("mV");
    Serial.print("Collector:  ");
    Serial.print(leak_milliVolts);
    Serial.println("mV");
    Serial.print("Leak: ");
    Serial.print(leak_uA);
    Serial.println("uA");
    Serial.print("Base: ");
    Serial.print(baseCurrent_uA);
    Serial.println("uA");

    delay(50);  // wait a moment

    // now we set the base pin to output and apply a small, but pre-calculated, current over it.
    pinMode(baseResistorPin, OUTPUT);
    digitalWrite(baseResistorPin, LOW); // Base goes to ground via base resistor because it is PNP

    // read in voltages from the 16-bit ADC
    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);

    // rail voltage goes into ADC0 and collector voltage goes into ADC1
    rail_milliVolts = computeMilliVolts(adc0);
    collector_milliVolts = computeMilliVolts(adc1);

    // gain is collector-voltage / collector-resistance / base-current
    // so if we have 750 millivolts on the collector after applying 4uA of current to the base pin,
    // and we are using a 1K resistor for the collector resistor, the our gain calculation would look like this:
    // gain = (750mV / 1000) / 1000 ohms / (4uA / 1,000,000)
    // gain = 0.75V / 1000 ohms / 0.000004A
    // gain = 187.5
    gain = (collector_milliVolts / 1000.0) / collector_resistor / (baseCurrent_uA / 1000000.0);

    // however, this isn't the true story. The leakage voltage also goes over the collector resistor, so we must subtract
    // the leakage voltage from the collector voltage. So, if we had a leakage voltage of 100mV (which with a 1K collector
    // resistor is 100uA of leakage current), we don't actually have 750 millivolts on the collector after applying the
    // 4uA of current to the base pin, but rather 750mV - 100mV = 650mV, so the true gain would calculate as:
    // gain = (650mV / 1000) / 1000 ohms / (4uA / 1,000,000)
    // gain = 0.65V / 1000 ohms / 0.000004A
    // gain = 162.5
    trueGain = ((collector_milliVolts - leak_milliVolts) / 1000.0) / collector_resistor / (baseCurrent_uA / 1000000.0);

    // output our findings out the serial port
    Serial.print("Gain: ");
    Serial.print(gain);
    Serial.println(" hfe");
    Serial.print("True Gain: ");
    Serial.print(trueGain);
    Serial.println(" hfe");

    // clear the OLED display
    display.clearDisplay();

    display.setTextSize(1);               // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);  // Draw white text
    display.setCursor(0,0);               // Start at top-left corner
    display.println(F("PNP Transistor")); // Start printing to screen
    // we will continue the rest later
    
  } else {
    // Transistor is NPN

    // the leakage has caused an amount of collector voltage that we need to record and subtract
    // from the gain, when we get to that point. Because the leakage is in reference to the rail
    // voltage, as this is an NPN transistor, we must subtract from the rail voltage to get leak
    // voltage
    leak_milliVolts = rail_milliVolts - collector_milliVolts;

    // in case we get any weird jitter on the collector pin
    if(leak_milliVolts < 0.1) {
      leak_milliVolts = 0.0;
    }

    // ohms law, V / R = I. Because we want current in uA and the volts are in milliVolts,
    // we need to multiply by 1000.0. This will give us the current from the collector pin.
    leak_uA = (leak_milliVolts / collector_resistor) * 1000.0;
    
    Serial.println("NPN Transistor");
    Serial.print("PWR Rail:   ");
    Serial.print(rail_milliVolts);
    Serial.println("mV");
    Serial.print("Collector:  ");
    Serial.print(leak_milliVolts);
    Serial.println("mV");
    Serial.print("Leak: ");
    Serial.print(leak_uA);
    Serial.println("uA");
    Serial.print("Base: ");
    Serial.print(baseCurrent_uA);
    Serial.println("uA");

    delay(50); // wait a moment

    // now we set the base pin to output and apply a small, but pre-calculated, current over it.
    pinMode(baseResistorPin, OUTPUT);
    digitalWrite(baseResistorPin, HIGH); // Base goes to 5V via base resistor because it is NPN

    // read in voltages from the 16-bit ADC
    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);

    // rail voltage goes into ADC0 and collector voltage goes into ADC1
    rail_milliVolts = computeMilliVolts(adc0);
    collector_milliVolts = computeMilliVolts(adc1);

    // because the collector voltage is in reference to the rail voltage, we have to subtract from the rail voltage
    // gain is collector-voltage / collector-resistance / base-current
    // so if we have 750 millivolts on the collector after applying 4uA of current to the base pin,
    // and we are using a 1K resistor for the collector resistor, the our gain calculation would look like this:
    // gain = (750mV / 1000) / 1000 ohms / (4uA / 1,000,000)
    // gain = 0.75V / 1000 ohms / 0.000004A
    // gain = 187.5
    gain = ((rail_milliVolts - collector_milliVolts) / 1000.0) / collector_resistor / (baseCurrent_uA / 1000000.0);

    // however, this isn't the true story. The leakage voltage also goes over the collector resistor, so we must subtract
    // the leakage voltage from the collector voltage. So, if we had a leakage voltage of 100mV (which with a 1K collector
    // resistor is 100uA of leakage current), we don't actually have 750 millivolts on the collector after applying the
    // 4uA of current to the base pin, but rather 750mV - 100mV = 650mV, so the true gain would calculate as:
    // gain = (650mV / 1000) / 1000 ohms / (4uA / 1,000,000)
    // gain = 0.65V / 1000 ohms / 0.000004A
    // gain = 162.5
    trueGain = (((rail_milliVolts - collector_milliVolts) - leak_milliVolts) / 1000.0) / collector_resistor / (baseCurrent_uA / 1000000.0);

    Serial.print("Gain: ");
    Serial.print(gain);
    Serial.println(" hfe");
    Serial.print("True Gain: ");
    Serial.print(trueGain);
    Serial.println(" hfe");

    // clear the OLED display
    display.clearDisplay();

    display.setTextSize(1);               // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);  // Draw white text
    display.setCursor(0,0);               // Start at top-left corner
    display.println(F("NPN Transistor")); // Start printing to screen
    // we will continue the rest later
  }

  // Now display our gain and leakage findings
  display.print(F("Gain: "));
  display.print(trueGain);
  display.println(F(" hfe"));
  display.print(F("Leak: "));
  display.print(leak_uA);
  display.println(F("uA"));
  display.display();

  // Then reset all pins back to input mode
  pinMode(emitterPin, INPUT);
  pinMode(basePin, INPUT);
  pinMode(collectorPin, INPUT);
  pinMode(collectorResistorPin, INPUT);
  pinMode(baseResistorPin, INPUT);

  // So that we can then set the pins back to floating
  digitalWrite(emitterPin, LOW);
  digitalWrite(basePin, LOW);
  digitalWrite(collectorPin, LOW);
  digitalWrite(collectorResistorPin, LOW);
  digitalWrite(baseResistorPin, LOW);
}

void loop() {
  
  // wait 500 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(500);
}

/**************************************************************************/
/*!
    @brief  Returns true if conversion is complete, false otherwise.

    @param counts the ADC reading in raw counts

    @return the ADC reading in milli-volts
*/
/**************************************************************************/
float computeMilliVolts(int16_t counts) {
  uint8_t bitShift = 0;            ///< bit shift amount
  return counts * 1000.0 * (6.144f / (32768 >> bitShift));
}
