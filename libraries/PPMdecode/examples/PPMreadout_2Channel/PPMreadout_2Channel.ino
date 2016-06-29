#include <PPMdecode.h>

int interA = 0; //Let op! int.0 staat verbonden op !pin 2! zie: http://arduino.cc/en/Reference/attachInterrupt .
int interB = 1; //Let op! int.1 staat verbonden op !pin 3! zie: http://arduino.cc/en/Reference/attachInterrupt .
int channelsA = 6; //In te stellen op het aantal in te lezen kanalen.
int channelsB = 6; //In te stellen op het aantal in te lezen kanalen.
PPMdecode myPPMdecodeA = PPMdecode(interA, channelsA); //Aanmaken van een nieuw object. Voorlopig kun je er nog geen meerdere aanmaken.
PPMdecode myPPMdecodeB = PPMdecode(interB, channelsB);
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
	Serial.print("one: ");
	for (int i; i < channelsA; i++){

		int tmpA = myPPMdecodeA.channel[i];
		String myStringA =
			"CH"
			+ String(i)
			+ ": "
			+ String(tmpA)
			+ " "
			+ ((tmpA < 1000) ? " " : "")//text align
			+ ((tmpA < 100) ? " " : "")//text align
			+ ((tmpA < 10) ? " " : "");//text align
		Serial.print(myStringA);
	}
        Serial.print(" ");
        
	Serial.print("TWO: ");
	for (int i; i < channelsB; i++){

		int tmpB = myPPMdecodeB.channel[i];
		String myStringB =
			"CH"
			+ String(i)
			+ ": "
			+ String(tmpB)
			+ " "
			+ ((tmpB < 1000) ? " " : "")//text align
			+ ((tmpB < 100) ? " " : "")//text align
			+ ((tmpB < 10) ? " " : "");//text align
		Serial.print(myStringB);
	}
	Serial.println("");

}