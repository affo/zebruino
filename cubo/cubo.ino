const int LOOP_DELAY = 500;
// PIN
// pin dei sensori di prossimità
int presence1_pin, presence2_pin;
// pin dei led
int led1_pin, led2_pin, led_voice_pin;
// pin del microfono
int mic_pin;

// STATUS
int presence1_status, presence2_status;
// status del microfono
int mic_status;


void setup(){
	Serial.begin(9600);
	pinMode(presence1_pin, INPUT);
	pinMode(presence2_pin, INPUT);

	pinMode(led1_pin, OUTPUT);
	pinMode(led2_pin, OUTPUT);
	pinMode(led_voice_pin, OUTPUT);

	pinMode(mic_pin, INPUT);
}

void loop(){
	// prendo i valori dai sensori di presenza..
	presence1_status = digitalRead(presence1_pin);
	presence2_status = digitalRead(presence2_pin);

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
	// accendo il led verde in base a ciò che leggo dal microfono
	if(led1_status == HIGH && led2_status == HIGH){
		mic_status = analogRead(mic_pin);
		analogWrite(led_voice_pin, mic_status);
	}else{
		digitalWrite(led_voice_pin, LOW);
	}

	delay(LOOP_DELAY);
}
