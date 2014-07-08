const int LOOP_DELAY = 100;

//PIN
int pin_prox_out_1 = 9;
int pin_prox_out_2 = 10;
int pin_prox_enable_1 = 2;
int pin_prox_enable_2 = 4;
int pin_mic_out = A0;

//MIC vars
int mic_min = 1024; //set it to HIGHest
int mic_max = 0; //set it to LOWest

void reset_mic_vars(void){
	mic_min = 1024;
	mic_max = 0;
}

void print_test(char* name, int val){
	Serial.print(name);
	Serial.print("\t\t---->\t");
	Serial.println(val);
}

void setup(){
	Serial.begin(9600);
	pinMode(pin_prox_out_1, INPUT);
	pinMode(pin_prox_out_2, INPUT);
	pinMode(pin_mic_out, INPUT);

	pinMode(pin_prox_enable_1, OUTPUT);
	pinMode(pin_prox_enable_2, OUTPUT);

}

void loop(){
	//Serial.println("ENABLE set to LOW");
	//digitalWrite(pin_prox_enable_1, LOW);
	//digitalWrite(pin_prox_enable_2, LOW);

	//status = analogRead(pin_prox_out_1);
	//print_test("PROXIMITY_1", status);

	//status = analogRead(pin_prox_out_2);
	//print_test("PROXIMITY_2", status);

	int mic_status = analogRead(pin_mic_out);

	if(mic_status > mic_max){
		mic_max = mic_status;
	}

	if(mic_status < mic_min){
		mic_min = mic_status;
	}

	int volume = mic_max - mic_min;
	if(volume > 0){
		print_test("MIC VOLUME", volume);
		reset_mic_vars();
	}
	

	delay(LOOP_DELAY);
}