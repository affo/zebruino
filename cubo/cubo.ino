const int LOOP_DELAY = 3000;
// PIN
// pin dei led
int led1_pin = 2, led2_pin = 3, led_voice_pin = 4;

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

			Serial.println("RED_LED_1: ON");
		}else{
			digitalWrite(led1_pin, LOW);
			led1_status = LOW;

			Serial.println("RED_LED_1: OFF");
		}

		rand_num = random(0, 1);
		if(rand_num < 0.5){
			digitalWrite(led2_pin, HIGH);
			led2_status = HIGH;

			Serial.println("RED_LED_2: ON");
		}else{
			digitalWrite(led2_pin, LOW);
			led2_status = LOW;

			Serial.println("RED_LED_2: OFF");
		}

		// se tutti e due i led sono accesi
		// accendo il led verde
		if(led1_status == HIGH && led2_status == HIGH){
			// accendi il led verde
			digitalWrite(led_voice_pin, HIGH);
			read_mic = true;

			Serial.println("GREEN_LED: ON (led1 on && led2 on)");			
		}else{
			digitalWrite(led_voice_pin, LOW);
			read_mic = false;

			Serial.println("GREEN_LED: OFF (led1 off || led2 off)");
		}

	}else{
		//rand_num = int(random(0, 255));
		//analogWrite(led_voice_pin, rand_num);
		rand_num = random(0, 1);
		if(rand_num < 0.5){
			digitalWrite(led_voice_pin, HIGH);
			read_mic = false;

			Serial.println("GREEN_LED: OFF (randomly)");
		}
	}

	delay(LOOP_DELAY);
}
