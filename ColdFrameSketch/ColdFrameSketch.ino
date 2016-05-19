//TMP36 Pin Variables
const double OPEN_TEMP = 80;
const double CLOSE_TEMP = 40;
const int OPENING_MILLISECONDS = 5000;
const int sensorPin = 0;    // Temp sensor TMP36GTZ the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
const int switchPin = 2;    // switch input
const int motor1Pin = 3;    // H-bridge leg 1 (pin 2, 1A)
const int motor2Pin = 4;    // H-bridge leg 2 (pin 7, 2A)
const int enablePin = 9;    // H-bridge enable pin

enum FrameState {
 unknown,
 open,
 closed 
};


FrameState currentFrameState;

//In the setup(), set all the pins for the H-bridge as outputs, and the pin for the switch as an input.:
//The set the enable pin high so the H-bridge can turn the motor on.:
void setup()
{
    Serial.begin(9600);  //Start the serial connection with the computer
                         //to view the result open the serial monitor 
                         
    // set the switch as an input:
    pinMode(switchPin, INPUT); 
 
    // set all the other pins you're using as outputs:
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    //pinMode(ledPin, OUTPUT);
 
    // set enablePin high so that motor can turn on:
   
  

    // Initialization
    currentFrameState = unknown;
    
    //if(!isLidClosed()) {  // Check if the lid is closed
       closeFrame(); // if not, close the frame
    //}
    
    
}
//In the main loop() read the switch. :
//If it’s high, turn the motor one way by taking one H-bridge pin high and the other low.:
//If the switch is low, reverse the direction by reversing the states of the two H-bridge pins.:
void loop()
{

  float currentTemp = getCurrentTemp();
  if(currentTemp > OPEN_TEMP) {
    if(currentFrameState != open){
      openFrame();  
    }
  } 
  else if(currentTemp < CLOSE_TEMP) {
    if(currentFrameState != closed) {
      closeFrame();
    }
  } 

  delay(5000);  //waiting 5 seconds
}
  
void openFrame() {
  digitalWrite(motor1Pin, LOW);   // set leg 1 of the H-bridge low
  digitalWrite(motor2Pin, HIGH);  // set leg 2 of the H-bridge high
  startMotor();
  unsigned long startTime = millis();
  while((millis()-startTime) < OPENING_MILLISECONDS) {
    // wait around
  }
  stopMotor();    
  currentFrameState = open;
}

void closeFrame() {
  // Start closing the frame  
  digitalWrite(motor1Pin, HIGH);  // set leg 1 of the H-bridge high
  digitalWrite(motor2Pin, LOW);   // set leg 2 of the H-bridge low
  startMotor();
  
  while(!isLidClosed()){  // wait for the lid to be closed
    // we don't really do anything here
  }
  stopMotor();
  currentFrameState = closed;
}

void stopMotor() {  
  analogWrite(enablePin,0);
  delay(2000);  // Give the motor a little breather 
}

void startMotor() {
   analogWrite(enablePin, 195);  // We think 255 is the top 
}

bool isLidClosed() {
  int lidState = digitalRead(switchPin);
  if(lidState == HIGH)
    return true;
  return false;
}

// Returns the current temperature in F
float getCurrentTemp() {
  //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // print out the voltage
 Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
 
  // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;//to degrees ((voltage - 500mV) times 100)
 
 Serial.print(temperatureF); 
 Serial.println(" degrees F");
  
 return temperatureF;
}
  

