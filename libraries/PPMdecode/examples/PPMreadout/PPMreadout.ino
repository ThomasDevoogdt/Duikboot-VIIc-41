#include <PPMdecode.h>

int inter = 0; //Let op! int.0 staat verbonden op !pin 2! zie: http://arduino.cc/en/Reference/attachInterrupt .
int channels = 6; //In te stellen op het aantal in te lezen kanalen.
PPMdecode myPPMdecode = PPMdecode(inter, channels); //Aanmaken van een nieuw object. Voorlopig kun je er nog geen meerdere aanmaken.
/*
Mogelijkheden om op te roepen:
	class.channel[i]; met i het gewenste kanaal.
	class.synchronized; ter controle of alles normaal werkt.
*/

void setup() {
	Serial.begin(115200); //Vergeet de monitor niet in te stellen!
}

void loop(){
	//Alles wat in de hoofd-lus staat is louter bedoeld om visueel de kanalen zichtbaar te maken. 
	Serial.print(myPPMdecode.synchronized ? "Synchronized = True     " : "Synchronized = False    ");
	for (int i; i < channels; i++){
		int tmp = myPPMdecode.channel[i];
		String myString =
			"CH"
			+ String(i)
			+ ": "
			+ String(tmp)
			+ " "
			+ ((tmp < 1000) ? " " : "")//text align
			+ ((tmp < 100) ? " " : "")//text align
			+ ((tmp < 10) ? " " : "");//text align
		Serial.print(myString);
	}
	Serial.println("");
}