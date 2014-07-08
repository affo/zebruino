#define LOOP_DELAY 10

// PIN
// pin dei sensori di prossimità
int pin_prox_1 = 2;
int pin_prox_2 = 3;
// pin dei led
int pin_led = 9;
// pin del microfono
int pin_mic = A0;

// rumore di fondo
int noise;

// last value in led
int last_range;

//returns the range of the signal
int range(int led_analog){
	led_analog++;
	if(led_analog <= 32) return 0;
	if(led_analog <= 64) return 1;
	if(led_analog <= 96) return 2;
	if(led_analog <= 128) return 3;
	if(led_analog <= 160) return 4;
	if(led_analog <= 192) return 5;
	if(led_analog <= 224) return 6;
	if(led_analog <= 256) return 7;
}

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
	pinMode(pin_prox_1, INPUT);
	pinMode(pin_prox_2, INPUT);
	pinMode(pin_led, OUTPUT);
	pinMode(pin_mic, INPUT);

	//prendi il primo valore di rumore
	noise = analogRead(pin_mic);
	last_range = 0;
}

void loop(){
	// prendo i valori dai sensori di presenza
	int status_prox_1 = digitalRead(pin_prox_1);
	int status_prox_2 = digitalRead(pin_prox_2);

	// TODO rimuovere!
	// per ora, non abbiamo il secondo sensore di prossimità.
	// lo settiamo a sempre attivo:
	status_prox_2 = LOW;

	if(status_prox_1 == HIGH || status_prox_2 == HIGH){
		// se uno dei sensori non è attivo
		// allora vado avanti a "registrare" il rumore di fondo:

		// spengo il led centrale
		digitalWrite(pin_led, LOW);
		// prendo il rumore
		noise = analogRead(pin_mic);
	}else{

		// altrimenti
		// accendo il led in base a ciò che leggo dal microfono
		int status_mic = analogRead(pin_mic);

		// tolgo il brusio limitando a 0
		status_mic -= noise;
		if(status_mic < 0) status_mic = 0;

		// il microfono è a 10 bit (da 0 a 1023);
		// il led è a 8 bit (da 0 a 255);
		// per non fondere il led, normalizzo il valore ottenuto tramite proporzione:
		//		x : 255 = uscita_microfono : 1023
		// quindi
		// 		x = (255 / 1023) * uscita_microfono

		int status_led = (int) ((255 / (float)1023) * status_mic);

		int curr_range = range(status_led);

		if(curr_range != last_range){
			//TODO remove
			Serial.print("(NOISE, MIC, LED) --->  ");
			Serial.print(noise);
			Serial.print(",\t");
			Serial.print(status_mic);
			Serial.print(",\t");
			Serial.println(status_led);

			last_range = curr_range;

			// infine scrivo il valore ottenuto sul led
			int write = (curr_range)  
			analogWrite(pin_led, (curr_range + 1)*32 - 16);
		}
	}

	delay(LOOP_DELAY);
}
