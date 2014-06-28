void fade_in(int pin){
	for(int fadeValue = 0; fadeValue <= 255; fadeValue +=5) { 
		analogWrite(pin, fadeValue);
		// se non aspetti un po, l'accensione è istantanea (al tuo occhio)    
		delay(30);                     
	} 
}

void fade_out(int pin){
	for(int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) { 
		// sets the value (range from 0 to 255):
		analogWrite(pin, fadeValue);
		// wait for 30 milliseconds to see the dimming effect    
		delay(30);                     
	} 
}

void green_led_on(int pin){
	fade_in(pin);
	fade_out(pin);
}