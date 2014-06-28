const int LOOP_DELAY = 1000;
// PIN
// pin dei led
int led1_pin = 2, led2_pin = 3, led_voice_pin = 5;

// STATUS
// status dei led
int led1_status, led2_status, led_voice_status;

// booleano (T/F) per capire lo stato
boolean read_mic = false;

//simuliamo la presenza in modo randomico
long rand_num = 0;

// fade
void fade(int pin, boolean in, int duration){
	int step = (5 * duration) / 255;
	int min = 0;
	int max = 255;
	if(in){
		for(; min <= max; min += 5){ 
			analogWrite(pin, min);
			// se non aspetti un po, l'accensione è istantanea (al tuo occhio)    
			delay(step);
		}
	}else{
		for(; max >= min; max -= 5){ 
			analogWrite(pin, min);
			// se non aspetti un po, l'accensione è istantanea (al tuo occhio)    
			delay(step);
		}
	}
}


void setup(){
	Serial.begin(9600);
	pinMode(led1_pin, OUTPUT);
	pinMode(led2_pin, OUTPUT);
	pinMode(led_voice_pin, OUTPUT);

	randomSeed(analogRead(0));
}

void loop(){
	if(!read_mic){

		rand_num = random(0, 10);
		if(rand_num < 5){
			digitalWrite(led1_pin, HIGH);
			led1_status = HIGH;

			Serial.println("RED_LED_1: ON");
		}else{
			digitalWrite(led1_pin, LOW);
			led1_status = LOW;

			Serial.println("RED_LED_1: OFF");
		}

		rand_num = random(0, 10);
		if(rand_num < 5){
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
			//ok ci siamo, facciamo blinkare le rosse
			for(int i = 0; i < 4; i++){
				delay(250);
				digitalWrite(led1_pin, LOW);
				digitalWrite(led2_pin, LOW);
				delay(250);
				digitalWrite(led1_pin, HIGH);
				digitalWrite(led2_pin, HIGH);
			}
			delay(500);
			// accendi il led verde, (fade in)
			fade(led_voice_pin, true, 2500);
			read_mic = true;

			Serial.println("GREEN_LED: ON (led1 on && led2 on)");			
		}else{
			digitalWrite(led_voice_pin, LOW);
			read_mic = false;

			Serial.println("GREEN_LED: OFF (led1 off || led2 off)");
		}

	}else{
		rand_num = random(0, 10);
		if(rand_num < 3){
			read_mic = false;

			Serial.println("GREEN_LED: REINIT (randomly)");
		}

		rand_num = random(0, 255);
		Serial.print("TALKING: ");
		Serial.println(rand_num);
		analogWrite(led_voice_pin, rand_num);
	}

	delay(LOOP_DELAY);
}
