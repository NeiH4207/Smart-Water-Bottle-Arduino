#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
#define echoPin 9 // attach pin D2 Arduino to pin Echo of HC-05
#define trigPin 8 //attach pin D3 Arduino to pin Trig of HC-05
#define LED 13

// defines variables
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement

float temp;    //Define the temperature float variable
int temp_sensor = 0;     // sensor middle pin on analog pin 0
int distance_arr[10];
int ptr = 0;
float temperature ;    //Define the temperature float variable
int sensor = 0;     // sensor middle pin on analog pin 0

float water_level = 0;
int waiting_mode = 0;

char c = ' ';
String s;
 
void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(13, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600);  //start the serial monitor 
  pinMode(13, OUTPUT);
  digitalWrite(LED, LOW);
  // HC-05 default serial speed for commincation mode is 9600
  BTserial.begin(9600);  
}

void fast_blink(){
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

void slow_blink(){
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

void set_temprature(){
  
  temperature = analogRead(temp_sensor);        // assigning the analog output to temperature
  
  temperature = temperature * 0.488758553;   // converting volts to degrees celsius ----- 0.48828125 = [(5V*1000)/1024]10
  if (temperature > 80) {
   fast_blink();
  }
}

void set_distance(){
  
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
    distance_arr[ptr] = duration * 0.03445 / 2; // Speed of sound wave divided by 2 (go and back)
    ptr += 1;
    ptr %= 10;
    // check the stability of water level
    int i;
    int mx = -1;
    int mn = 1000;
    for (i = 0; i < 10; i++){
      if (distance_arr[i] > mx) {
        mx = distance_arr[i];
      } 
      if (distance_arr[i] < mn) {
        mn = distance_arr[i];
      }
    }

    int diff = mx - mn;
    if (diff < 3) {
      distance = (mx + mn) / 2;
    }
      
}

void set_water_level(){
  water_level = 3.14159265 * 4.5 * 4.5 * (8 - distance);
  water_level = max(water_level, 0);
  if (distance > 20 || distance < 0){
    if (waiting_mode == 0) {
      waiting_mode = 1;
      Serial.println("Turn Waiting Mode On");
    } 
  } else{
    if (waiting_mode == 1) {
      waiting_mode = 0;
      Serial.println("Turn Waiting Mode Off");
      delay(5000);  //wait 5 second
    } 
  }
  if (water_level < 50) {
    slow_blink();
  }
}

void loop(){
    set_temprature();
    set_distance();
    set_water_level();

    
    if (waiting_mode == 1) {
      return;
    }

//    Displays the temperature on the Serial Monitor
    Serial.print("Temperature: ");     // print read value 
    Serial.print(temperature);     // print read value 
    Serial.println("°C");   //print on the serial monitor "__value °C temperature"
    
    
//    Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    Serial.print("Water level: ");
    Serial.print(water_level);
    Serial.println(" ml");

    // Send data to android app using BT serial
    BTserial.print(temperature); 
    BTserial.print(','); 
    BTserial.print(water_level); 
    BTserial.print('\n'); 

    // Respond
    boolean isValidInput; 
    do { 
        if (BTserial.available()){
          byte c; // get the next character from the bluetooth serial port
          c = 0;
          c = BTserial.read(); // Execute the option based on the character recieved
          if (c == 'S') {
            Serial.print("Successful!");
          }
        }
      
     } while ( isValidInput == true ); // Repeat the loop
  
  delay(1000);  //wait 5 second
}
