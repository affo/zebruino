#include "utils.ino"

const int LOOP_DELAY = 500;
// PIN
// pin dei sensori di prossimità
int presence1_pin, presence2_pin;
// pin dei led
int led1_pin, led2_pin, led_voice_pin;
int led1_pin_IN, led2_pin_IN, led_voice_pin_IN;
// pin del microfono
int mic_pin;

// STATUS
int presence1_status, presence2_status;
// status dei led
int led1_status, led2_status, led_voice_status;
// status del microfono
int mic_status;

// booleano (T/F) per capire lo stato
boolean read_mic = false;


void setup(){
	Serial.begin(9600);
	pinMode(presence1_pin, INPUT);
	pinMode(presence2_pin, INPUT);

	pinMode(led1_pin, OUTPUT);
	pinMode(led2_pin, OUTPUT);
	pinMode(led_voice_pin, OUTPUT);

	pinMode(led1_pin_IN, INPUT);
	pinMode(led2_pin_IN, INPUT);
	pinMode(led_voice_pin_IN, INPUT);

	pinMode(mic_pin, INPUT);
}

void loop(){
	// prendo i valori dai sensori di presenza..
	presence1_status = digitalRead(presence1_pin);
	presence2_status = digitalRead(presence2_pin);
	// ... e dei led
	led1_status = digitalRead(led1_pin_IN);
	led2_status = digitalRead(led2_pin_IN);
	led_voice_status = digitalRead(led_voice_pin_IN);
	mic_status = digitalRead(mic_pin);

	Serial.println("presence1_status");
	Serial.println(presence1_status);
	Serial.println("presence2_status");
	Serial.println(presence2_status);
	Serial.println("led1_status");
	Serial.println(led1_status);
	Serial.println("led2_status");
	Serial.println(led2_status);
	Serial.println("green_led_status");
	Serial.println(led_voice_status);
	Serial.println("mic_status");
	Serial.println(mic_status);


	if(!read_mic){

		// se qualcuno è vicino accendo il led1 rosso
		// se non trovo nessuno, spengo
		if(presence1_status == HIGH){
			digitalWrite(led1_pin, HIGH);
		}else{
			digitalWrite(led1_pin, LOW);
		}

		// se qualcuno è vicino accendo il led2 rosso
		// se non trovo nessuno, spengo
		if(presence2_status == HIGH){
			digitalWrite(led2_pin, HIGH);
		}else{
			digitalWrite(led2_pin, LOW);
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
		mic_status = analogRead(mic_pin);
		//analogWrite(led_voice_pin, mic_status / 4);
	}

	delay(LOOP_DELAY);
}
