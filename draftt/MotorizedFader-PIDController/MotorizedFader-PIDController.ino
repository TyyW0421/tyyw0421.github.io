// Prepared by Prof. Ken Nakagaki for Class "Actuated User Interfaces and Technology"
// (With help of Anup[TA] and Chat-GPT[AI])
// Feb 15 2023

const int potPin = A0;    // analog input for reading the potentiometer value
const int enablePin = 3;  // PWM output for controlling motor speed
const int in1Pin = 5;     // control input 1 for motor driver
const int in2Pin = 6;     // control input 2 for motor driver

const int maxSpeed = 255;  // maximum motor speed (255)

//core PID values
const double Kp = 1.0;    // proportional gain
const double Ki = 0.8;   // integral gain
const double Kd = 0.25;     // derivative gain

// Define the setPosition for the motor
int setPosition = 512;

// Variables for storing PID controller state
double lastError = 0;
double integral = 0;

unsigned long lastMillis = 0;


void setup() {
  pinMode(enablePin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(potPin, INPUT);
  // Set the initial motor speed to zero
  analogWrite(enablePin, 0);

  Serial.begin(115200);
}

void loop() {
  //sweepPosition(); //Enable This to  use Aumatic Sweeping Function 


  unsigned long elapsedMillis = millis() - lastMillis;
  lastMillis = millis();


  // Read the potentiometer value
  int sensorPosition = analogRead(potPin);


  //PID Control

  // Calculate the error between the desired potentiometer position and the current position
  double error = setPosition - sensorPosition;
  
  // Calculate the PID controller output
  double output = Kp * error + Ki * integral + Kd * (error - lastError);

  // Constrain the output to the allowable motor speed range
  output = constrain(output, -maxSpeed, maxSpeed);




  // Printing to check parameters

  // Serial.print("ElapsedTime:");
  // Serial.println(elapsedMillis);
  // Serial.print(",");
  // Serial.print("Output:");
  // Serial.print(output);
  // Serial.print(",");
  // Serial.print("Kd:");
  // Serial.print(Kd * (error - lastError));
  // Serial.print(",");

  Serial.print("setPosition:");
  Serial.print(setPosition);
  Serial.print(",");
  Serial.print("sensorPosition:");
  Serial.println(sensorPosition);




  // Motor Control
  if (output > 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  }
  // Update the motor speed
  analogWrite(enablePin, abs(output));


  // Update the integral term
  integral += error * elapsedMillis / 1000.0;
  // Update the last error
  lastError = error;
  // Wait for the next control loop iteration
}


// parameters to tune sweep positions
int minSweepVal = 100;
int maxSweepVal = 900;
int sweepSpeed = 4;
int sweepDirection = 1;  //1 or -1
int currentSweepingParameter = sweepSpeed;
void sweepPosition() {  //custom function to sweep
  setPosition += sweepDirection * sweepSpeed;

  if (setPosition > maxSweepVal) {
    sweepDirection = -1;
  } else if (setPosition < minSweepVal) {
    sweepDirection = 1;
  }
}