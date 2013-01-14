# Thermometer Project

The code for the [meoWS thermometer project](http://meoworkshop.org/a-pillbox-thermometer) is found here.  Also provided is the hex file which can be immediately flashed onto an Atmel ATTiny84.

The code has been compiled for the ATTiny84.  If you are using a different microcontroller, you will most likely have to change around some register configurations as well as compiling your modifications for your microcontroller of choice.

Pin 13 (ADC0) is the analog pin connected to the output of temperature sensor
Pins 6-12 (PA1-PA7) and pins 2,3 and 5 (PB0-PB2) are used to light LEDs

The ADC reference is set at Vcc and the math involved in converting ADC values to actual temperatures uses a 3 V reference.  This is because I powered my thermometer with a CR2032 watch battery which has a nominal voltage of 3 V.  If you choose to use another voltage be sure to change the reference voltage in the conversion code.

I recommend using a voltage regulator with your power supply.  I have had problems with the thermometer giving inaccurate results in temperatures below 0 C.  I suspect that the cold is affecting the battery voltage.

