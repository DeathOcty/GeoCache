// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
Name:       ArraySensor.ino
Created:	9/7/2018 1:30:27 PM
Author:     DESKTOP-9CQ4NRH\Ethan Areizaga
*/

// create LED structure containing cathode pin, anode pin, frequency tone, discharge period, 
// and calibration ratio.
#define SPEAKER 13
#define BTN 2
#define Meter 0
#define LEDCount 5
#define CANT_WAKE_UP goto WAKE_ME_UP_INSIDE;
struct LED {
	long period;
	long ratio;
	int cathode;
	int anode;
	int frequency;
	
};
long timedelay = 0;
LED lights[5];

// The setup() function runs once each time the micro-controller starts
void setup()
{


	Serial.begin(115200);
	Serial.println("Start");

	pinMode(BTN, INPUT_PULLUP);
	for (int i = 0; i < LEDCount; i++)
	{
		
		lights[i].anode = 3 + i;
		lights[i].cathode = 4 + i;
		lights[i].frequency = i*200;
		lights[i].period = 0.0;
		lights[i].ratio = 1.0;
	}

}

// Add the main program code into the continuous loop() function
void loop()
{
	long result = map(analogRead(Meter), 0, 1023, 0, 250000);
	for (int i = 0; i < LEDCount; i++)
	{

		pinMode(lights[i].cathode, OUTPUT);
		pinMode(lights[i].anode, OUTPUT);
		digitalWrite(lights[i].cathode, HIGH);
		digitalWrite(lights[i].anode, LOW);
		lights[i].period = 0.0;
	}
	for (int i = 0; i < LEDCount; i++)
	{

		pinMode(lights[i].cathode, INPUT);
		pinMode(lights[i].anode, OUTPUT);
		digitalWrite(lights[i].anode, LOW);
		
	}
	long starttime = micros(); 
	
	long lastLed;
	{
		int dischargedleds = 0;
		while (dischargedleds != 5) {
			for (int i = 0; i < LEDCount; i++)
			{
				if (lights[i].period == 0) {
					if (digitalRead(lights[i].cathode) == LOW) {
						lights[i].period = micros() - starttime;
						dischargedleds += 1;
						lastLed = lights[i].period;
					}
				}
			}
		}
	}
	int frequencysum = 0;
	for (int i = 0; i < LEDCount; i++)
	{
		/*lights[i].ratio += (0.01 * ((lastLed / lights[i].period) - lights[i].ratio));*/
		if (lights[i].period * lights[i].ratio > result) {
			pinMode(lights[i].cathode, OUTPUT);
			pinMode(lights[i].anode, OUTPUT);
			digitalWrite(lights[i].cathode, LOW);
			digitalWrite(lights[i].anode, HIGH);
			lights[i].frequency += frequencysum;
		}

	}
	if (frequencysum != 0) {
		tone(SPEAKER, frequencysum);
	}
	else {
		noTone(SPEAKER);
	}
	if (micros() - timedelay > 500) {
		if (/*digitalRead(BTN) == LOW*/ true) {
			Serial.print("Period(");
			for (int i = 0; i < LEDCount; i++)
			{
				Serial.print(lights[i].period);
				Serial.print(", ");
			}
			Serial.print(") \nRatio(");
			for (int i = 0; i < LEDCount; i++)
			{
				Serial.print(lights[i].ratio);
				Serial.print(", ");
			}
			Serial.print(")\n");
			timedelay = micros();
		}
	}
}



// execute LED for() loop
	// perform LED ratio calibration (last step in Rubric)
	// if LED discharge period multiplied by its ratio, is greater than the mapped discharge period
		// set LED to on state (CATHODE OUTPUT LOW, ANODE OUTPUT HIGH)
		// add LED's tone to the frequency sum
// start or stop playing the summed frequency tone (tone if sum != 0 or no tone if sum == 0)
// if the button is pressed (de-bounce is not desired)
	// print log message once a second
// end of loop()
