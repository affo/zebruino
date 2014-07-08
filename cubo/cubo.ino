#define LOOP_DELAY 10

// PIN
// pin dei sensori di prossimità
int pin_prox_1 = 2;
int pin_prox_2 = 3;
// pin dei led
int pin_led = 9;
// pin del microfono
int pin_mic = A0;

//MIC vars
int mic_min = 1024; //set it to HIGHest
int mic_max = 0; //set it to LOWest

void reset_mic_vars(void){
	mic_min = 1024;
	mic_max = 0;
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
		// spengo il led centrale
		digitalWrite(pin_led, LOW);
	}else{

		// altrimenti
		// accendo il led in base a ciò che leggo dal microfono
		int status_mic = analogRead(pin_mic);

		if(status_mic > mic_max){
			mic_max = status_mic;
		}

		if(status_mic < mic_min){
			mic_min = status_mic;
		}

		int volume = mic_max - mic_min;
		if(volume > 0){
			// il microfono è a 10 bit (da 0 a 1023);
			// il led è a 8 bit (da 0 a 255);
			// per non fondere il led, normalizzo il valore ottenuto tramite proporzione:
			//		x : 255 = uscita_microfono : 1023
			// quindi
			// 		x = (255 / 1023) * uscita_microfono
			int status_led = (int) ((255 / (float)1023) * volume);

			//TODO remove
			Serial.print("(NOISE, MIC, LED) --->  ");
			Serial.print(noise);
			Serial.print(",\t");
			Serial.print(volume);
			Serial.print(",\t");
			Serial.println(status_led);

			// infine scrivo il valore ottenuto sul led  
			analogWrite(pin_led, status_led);
		}
	}

	delay(LOOP_DELAY);
}
