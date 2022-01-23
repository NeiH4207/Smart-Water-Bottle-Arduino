#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
#define echoPin 9 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 8 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement

float temp;    //Define the temperature float variable
int temp_sensor = 0;     // sensor middle pin on analog pin 0

float temperature ;    //Define the temperature float variable
int sensor = 0;     // sensor middle pin on analog pin 0

float water_remaining = 0;

char c = ' ';
String s;
 
void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(13, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600);  //start the serial monitor 
  pinMode(13, OUTPUT);

  // HC-05 default serial speed for commincation mode is 9600
  BTserial.begin(9600);  
}


void get_temprature(){
  
  temperature = analogRead(temp_sensor);        // assigning the analog output to temperature
  
  temperature = temperature * 0.488758553;   // converting volts to degrees celsius ----- 0.48828125 = [(5V*1000)/1024]10
}

void get_distance(){
  
  // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.03445 / 2; // Speed of sound wave divided by 2 (go and back)
}

void get_water_remaining(){
  water_remaining = 3.14159265 * 4.5 * 4.5 * (8 - distance);
  
}

void loop(){
    get_temprature();
    get_distance();
    get_water_remaining();
    Serial.print("Temprature: ");     // print read value 
    Serial.print(temperature);     // print read value 
    Serial.println("°C");   //print on the serial monitor "__value °C temperature"
    
    
//    Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    Serial.print("Water remaining: ");
    Serial.print(water_remaining);
    Serial.println(" ml");

//    BTserial.print("temperature: "); 
    BTserial.print(temperature); 
//    BTserial.println("°C");   
//    BTserial.print("Water remaining: "); 
    BTserial.print(water_remaining); 
//    BTserial.println(" ml");
    boolean isValidInput; 
    do { 
        if (BTserial.available()){
          byte c; // get the next character from the bluetooth serial port
          c = 0;
          c = BTserial.read(); // Execute the option based on the character recieved
          Serial.print("Bluetooth respond: ");
          Serial.println(c);
        }
      
     } while ( isValidInput == true ); // Repeat the loop
  
  delay(5000);  //wait 5 second
}
