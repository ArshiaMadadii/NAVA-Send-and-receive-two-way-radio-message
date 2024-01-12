//Temperature Monitoring System using Arduino, LM35 and Hc-05 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // RX, TX

#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);
int tempPin = A0;   // the output pin of LM35

int read_ADC= 0;
double temp = 0;
double tempC = 0;
double tempF = 0;

String line1 = "Subscribe To My Channel Muhammad Ansar"; // Scrolling line

void setup() {
Serial.begin(9600); 
mySerial.begin(9600); 
Serial.println("Ready");
mySerial.println("Ready");

pinMode(tempPin, INPUT);
lcd.begin(16,2);  

for(int i=0; i>-60; i-= 1){
lcd.clear();
lcd.setCursor(i, 0);
lcd.print(line1);
delay(400); 
 }  
lcd.clear(); 
}
 
void loop(){
read_ADC =0;

for (int i=0; i < 100; i++){   
read_ADC = read_ADC+ analogRead(tempPin);
delay(1); 
}
read_ADC = read_ADC/100; 
temp = (read_ADC / 1023.0) * 5000; // 5000 to get millivots.
tempC = temp * 0.1; // 500 is the offset
tempF = (tempC * 1.8) + 32; // conver to F  

lcd.setCursor(0,0);    
lcd.print("   Temperature  ");
lcd.setCursor(0,1); 
lcd.print(tempC,1);
lcd.print((char)223); //degree symbol 
lcd.print("C  ");

lcd.setCursor(9,1); 
lcd.print(tempF,1);
lcd.print((char)223); //degree symbol 
lcd.print("F  ");

mySerial.print("S");
mySerial.print(";");
mySerial.print(tempC,0); //send distance to MIT App
mySerial.print(";");
mySerial.print(tempF,0); //send distance to MIT App
mySerial.println(";");   

delay(1000);
}
 

