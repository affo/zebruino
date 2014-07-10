#define LOOP_DELAY 10

// PIN
// pin dei sensori di prossimità
int pin_prox_1 = 2;
int pin_prox_2 = 3;
// pin dei led
int pin_led_1 = 12;
int pin_led_2 = 13;
int pin_led = 9;
// pin del microfono
int pin_mic = A0;

// rumore di fondo
int noise;

int last_prox_1;
int last_prox_2; 

// funzione creata per non permettere che il led vada in "flickering"
// ritorna 4 valori predefiniti al posto dei 256 possibili.
// i valori sono equamente distribuiti.
int rangify(int led_analog){
	led_analog++;
	if(led_analog <= 64) return 32;
	if(led_analog <= 128) return 96;
	if(led_analog <= 192) return 160;
	if(led_analog <= 256) return 224;
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
	pinMode(pin_led_1, OUTPUT);
	pinMode(pin_led_2, OUTPUT);
	pinMode(pin_mic, INPUT);

	noise = analogRead(pin_mic);
	last_prox_1 = HIGH;
	last_prox_2 = HIGH;
}

void loop(){
	// prendo i valori dai sensori di presenza
	int status_prox_1 = digitalRead(pin_prox_1);
	int status_prox_2 = digitalRead(pin_prox_2);

	// accendo i led in base ai sensori di prossimità
	if(status_prox_1 == HIGH && last_prox_1 == HIGH){
		last_prox_1 = HIGH;
		digitalWrite(pin_led_1, LOW);
	}else if(status_prox_1 == LOW && last_prox_1 == LOW){
		last_prox_1 = LOW;
		digitalWrite(pin_led_1, HIGH);
	}

	if(status_prox_2 == HIGH && last_prox_2 == HIGH){
		last_prox_2 = HIGH;
		digitalWrite(pin_led_2, LOW);
	}else if(status_prox_2 == LOW && last_prox_2 == LOW){
		last_prox_2 = LOW;
		digitalWrite(pin_led_2, HIGH);
	}

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

		//TODO remove
		Serial.print("(NOISE, MIC, LED) --->  ");
		Serial.print(noise);
		Serial.print(",\t");
		Serial.print(status_mic);
		Serial.print(",\t");
		Serial.println(status_led);

		// "rangifizzo" il valore del led per evitare il "flickeraggio".
		// infine scrivo il valore ottenuto sul led
		analogWrite(pin_led, rangify(status_led));
	}

	delay(LOOP_DELAY);
}
