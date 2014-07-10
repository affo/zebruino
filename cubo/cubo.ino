#define LOOP_DELAY 10
#define PROX_MEASUREMENT_SPAN 20
#define MIC_MEASUREMENT_SPAN 10
#define FADE_DURATION 2000
#define MIC_THRESHOLD 20

#define NO_LEDS_MIC 4
#define NO_RANGES 8 // multiplo di NO_LEDS_MIC

#define TALKING true

// PIN
// pin dei sensori di prossimità
int pin_prox_1 = 8;
int pin_prox_2 = 9;
// pin dei led
int pin_led_1 = 10; // semi-analog
int pin_led_2 = 11; // semi-analog
int pin_led_4ever = 3; // semi-analog
int pin_leds_mic[NO_LEDS_MIC] = {4, 5, 6, 7}; // digital
// pin del microfono
int pin_mic = A0;

// rumore di fondo
int noise;

// togliere il flickering dai sensori di prossimità
int last_prox_1[PROX_MEASUREMENT_SPAN], index_1 = 0;
int last_prox_2[PROX_MEASUREMENT_SPAN], index_2 = 0;
int mic_values[MIC_MEASUREMENT_SPAN], index_mic = 0;

// status dei led
boolean led_1_on = false;
boolean led_2_on = false;
boolean led_4ever_on = false;

void add_prox_1(int val){
	last_prox_1[index_1] = val;
	index_1++;
	if(index_1 == PROX_MEASUREMENT_SPAN){
		index_1 = 0;
	}
} 

void add_prox_2(int val){
	last_prox_2[index_2] = val;
	index_2++;
	if(index_2 == PROX_MEASUREMENT_SPAN){
		index_2 = 0;
	}
}

void add_mic(int val){
	mic_values[index_mic] = val;
	index_mic++;
	if(index_mic == MIC_MEASUREMENT_SPAN){
		index_mic = 0;
	}
}

int get_prox_1(void){
	int result = HIGH;
	for(int i = 0; i < PROX_MEASUREMENT_SPAN; i++){
		result = result && !last_prox_1[i];
		if(!result){
			break;
		}
	}
	return result;
}

int get_prox_2(void){
	int result = HIGH;
	for(int i = 0; i < PROX_MEASUREMENT_SPAN; i++){
		result = result && !last_prox_2[i];
		if(!result){
			break;
		}
	}
	return result;
}

int get_mic_val(void){
	int result = 0;
	for(int i = 0; i < MIC_MEASUREMENT_SPAN; i++){
		result += mic_values[i];
	}
	return (int) (result / (float)MIC_MEASUREMENT_SPAN);
}

// funzione creata per non permettere che il led vada in "flickering"
// ritorna un range di appartenenza.
// i valori sono equamente distribuiti.
int rangify(int val){
	//soglia minima per accendersi
	int thresh = (int) ((255 / (float)1023) * MIC_THRESHOLD);
	if(val < thresh) return 0;

	int step = 256 / NO_RANGES;
	for(int i = 1; i <= NO_RANGES; i++){
		if(val <= step * i) return i;
	}

	return NO_RANGES;
}

// funzione che scrive il valore del microfono
// sui 4 led. Più sarà alto il valore letto,
// più led si accenderanno
void write_led(int range){
	int no_led_group = NO_RANGES / NO_LEDS_MIC;
	// accendo quelli che servono
	for(int i = 0; i < range; i += no_led_group){
		for(int j = i; j < no_led_group && j < range; j++){
			digitalWrite(pin_leds_mic[j], HIGH);
		}
	}

	//spengo gli altri
	for(int i = range; i < NO_LEDS_MIC; i += no_led_group){
		for(int j = i; j < no_led_group && j < NO_LEDS_MIC; j++){
			digitalWrite(pin_leds_mic[j], LOW);
		}
	}
}

// fade
void fade(int pin, boolean in, int duration){
	int step = (int) ((5 * duration) / (float)255);
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
			analogWrite(pin, max);
			// se non aspetti un po, l'accensione è istantanea (al tuo occhio)    
			delay(step);
		}
	}
}


void setup(){
	Serial.begin(9600);
	pinMode(pin_prox_1, INPUT);
	pinMode(pin_prox_2, INPUT);
	pinMode(pin_led_4ever, OUTPUT);
	pinMode(pin_led_1, OUTPUT);
	pinMode(pin_led_2, OUTPUT);
	pinMode(pin_mic, INPUT);

	for(int i = 0; i < NO_LEDS_MIC; i++){
		pinMode(pin_leds_mic[i], OUTPUT);
	}

	noise = analogRead(pin_mic);

	for(int i = 0; i < PROX_MEASUREMENT_SPAN; i++){
		last_prox_1[i] = HIGH;
		last_prox_2[i] = HIGH;
	}

	for(int i = 0; i < MIC_MEASUREMENT_SPAN; i++){
		mic_values[i] = 0;
	}
}

void loop(){
	// prendo i valori dai sensori di presenza
	int status_prox_1 = digitalRead(pin_prox_1);
	int status_prox_2 = digitalRead(pin_prox_2);
	add_prox_1(status_prox_1);
	add_prox_2(status_prox_2);
	status_prox_1 = get_prox_1();
	status_prox_2 = get_prox_2();


	// accendo i led in base ai sensori di prossimità
	if(status_prox_1){
		// accendo solo se è spento
		if(!led_1_on){
			fade(pin_led_1, true, FADE_DURATION);
			led_1_on = true;
		}
	}else{
		// spengo solo se è acceso
		if(led_1_on){
			fade(pin_led_1, false, FADE_DURATION);
			led_1_on = false;
		}
	}

	// accendo i led in base ai sensori di prossimità
	if(status_prox_2){
		// accendo solo se è spento
		if(!led_2_on){
			fade(pin_led_2, true, FADE_DURATION);
			led_2_on = true;
		}
	}else{
		// spengo solo se è acceso
		if(led_2_on){
			fade(pin_led_2, false, FADE_DURATION);
			led_2_on = false;
		}
	}

	if(status_prox_1 && status_prox_2){
		// accendo il led che rimane sempre acceso
		if(!led_4ever_on){
			fade(pin_led_4ever, true, FADE_DURATION);
			led_4ever_on = true;
		}

		// leggo dal mic
		int status_mic = analogRead(pin_mic);

		// tolgo il brusio limitando a 0
		status_mic -= noise;
		if(status_mic < 0) status_mic = 0;

		add_mic(status_mic);
		status_mic = get_mic_val();

		// il microfono è a 10 bit (da 0 a 1023);
		// il led è a 8 bit (da 0 a 255);
		// per non fondere il led, normalizzo il valore ottenuto tramite proporzione:
		//		x : 255 = uscita_microfono : 1023
		// quindi
		// 		x = (255 / 1023) * uscita_microfono

		int status_led = (int) ((255 / (float)1023) * status_mic);

		//TODO remove
		Serial.print("(NOISE, MIC, LED, RANGE) --->  ");
		Serial.print(noise);
		Serial.print(",\t");
		Serial.print(status_mic);
		Serial.print(",\t");
		Serial.println(status_led);
		Serial.print(",\t");
		Serial.println(rangify(status_led));

		// "rangifizzo" il valore del led per evitare il "flickeraggio".
		// infine scrivo il valore ottenuto sul led
		write_led(rangify(status_led));

	}else{
		// se uno dei sensori non è attivo
		// allora vado avanti a "registrare" il rumore di fondo:

		// spengo il led che rimane sempre acceso
		if(led_4ever_on){
			fade(pin_led_4ever, false, FADE_DURATION);
			led_4ever_on = false;
		}

		//spengo i led del microfono
		write_led(0);

		// prendo il rumore
		noise = analogRead(pin_mic);
	}

	delay(LOOP_DELAY);
}
