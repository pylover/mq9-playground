#include <nash.h>

// TODO: Understand it!
// https://programozdazotthonod.hu/2020/05/03/a-proper-guide-to-the-mq9-sensor/

#define LED	7
#define DO 8
#define R0 .80f

static int seconds = 0;

static int sample(int argc, char **argv, struct process *self) {
	if (self->signal == SIG_INT) {
		return EXIT_FAILURE;
	}
	int alarm = 0; 
	float sensor_volt; 
	float RS_gas; 
	float ratio; 
	//-Replace the name "R0" with the value of R0 in the demo of First Test -/ 
	int sensorValue = analogRead(A0); 
	sensor_volt = ((float)sensorValue / 1024) * 5.0; 
	RS_gas = (5.0 - sensor_volt) / sensor_volt; // Depend on RL on yor module 
	ratio = RS_gas / R0; // ratio = RS/R0 
	//------------------------------------------------------------/ 
	Serial.print("sensor_volt = "); 
	Serial.print(sensor_volt); 
	Serial.print(", RS_ratio = "); 
	Serial.print(RS_gas); 
	Serial.print(", Rs/R0 = "); 
	Serial.println(ratio); 
	alarm = digitalRead(DO); 
	Serial.println(alarm, DEC);
	digitalWrite(LED, alarm); 
	delay(1000);
	return ALIVE;
}

static int calibrate(int argc, char **argv, struct process *self) {
	if (self->signal == SIG_INT) {
		return EXIT_FAILURE;
	}
	float sensor_volt;  
	float RS_air;		//  Rs in clean air 
	float r0;			// R0 in 1000 ppm LPG 
	float sensorValue; 

	//Average   
	for(int x = 0; x < 100; x++) { 
		sensorValue = sensorValue + analogRead(A0); 
 	} 
	sensorValue = sensorValue/100.0; 

	//-----------------------------------------------/ 
	sensor_volt = (sensorValue/1024)*5.0; 
	RS_air = (5.0-sensor_volt)/sensor_volt; // Depend on RL on yor module 
	r0 = RS_air/9.9; // According to MQ9 datasheet table 
	Serial.print(seconds++); 
	Serial.print(" sensor_volt = "); 
	Serial.print(sensor_volt); 
	Serial.print("V, R0 = "); 
	Serial.println(r0); 
	delay(1000); 
	return ALIVE;
}


static struct executable programs[] = {
	{"help", nash_help},
	{"calibrate", calibrate},
	{"sample", sample},
	{NULL}
};


void setup() {
	nash_init(programs);
	pinMode(LED, OUTPUT); 
	pinMode(DO, INPUT); 
}


void loop() {
	nash_loop();
}

