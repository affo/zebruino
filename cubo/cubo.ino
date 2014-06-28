#include "utils.ino"

const int LOOP_DELAY = 500;
// PIN
// pin dei led
int led1_pin, led2_pin, led_voice_pin;

// STATUS
// status dei led
int led1_status, led2_status, led_voice_status;

// booleano (T/F) per capire lo stato
boolean read_mic = false;

//simuliamo la presenza in modo randomico
long rand_num = 0;


void setup(){
	Serial.begin(9600);
	pinMode(led1_pin, OUTPUT);
	pinMode(led2_pin, OUTPUT);
	pinMode(led_voice_pin, OUTPUT);

}

void loop(){
	if(!read_mic){

		rand_num = random(0, 1);
		if(rand_num < 0.5){
			digitalWrite(led1_pin, HIGH);
			led1_status = HIGH;
		}else{
			digitalWrite(led1_pin, LOW);
			led1_status = LOW;
		}

		rand_num = random(0, 1);
		if(rand_num < 0.5){
			digitalWrite(led2_pin, HIGH);
			led2_status = HIGH;
		}else{
			digitalWrite(led2_pin, LOW);
			led2_status = LOW;
		}

		// se tutti e due i led sono accesi
		// accendo il led verde
		if(led1_status == HIGH && led2_status == HIGH){
			// accendi il led verde
			green_led_on(led_voice_pin);
			read_mic = true;			
		}else{
			digitalWrite(led_voice_pin, LOW);
			read_mic = false;
		}

	}else{
		rand_num = int(random(0, 255));
		analogWrite(led_voice_pin, rand_num);
		read_mic = false;
	}

	delay(LOOP_DELAY);
}
