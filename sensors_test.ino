const int LOOP_DELAY = 2000;

//PIN
int pin_prox_out_1 = 9;
int pin_prox_out_2 = 10;
int pin_prox_enable_1 = 2;
int pin_prox_enable_2 = 4;
int pin_mic_out = A0;

void print_test(char* name, int val){
	Serial.print(name);
	Serial.print("\t\t---->\t");
	Serial.println(val);
}

void setup(){
	Serial.begin(9600);
	pinMode(pin_prox_out_1, OUTPUT);
	pinMode(pin_prox_out_2, OUTPUT);
	pinMode(pin_mic_out, OUTPUT);

	pinMode(pin_prox_enable_1, INPUT);
	pinMode(pin_prox_enable_2, INPUT);

}

void loop(){
	Serial.println("ENABLE set to LOW");
	//digitalWrite(pin_prox_enable_1, LOW);
	//digitalWrite(pin_prox_enable_2, LOW);

	int status;
	status = analogRead(pin_prox_out_1);
	print_test("PROXIMITY_1", status);

	status = analogRead(pin_prox_out_2);
	print_test("PROXIMITY_2", status);

	status = analogRead(pin_mic_out);
	print_test("MICROPHONE", status);
	

	delay(LOOP_DELAY);
}
