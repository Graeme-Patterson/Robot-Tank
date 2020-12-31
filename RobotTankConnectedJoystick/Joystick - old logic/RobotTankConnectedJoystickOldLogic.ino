// Arduino Robot Tank
// uses L298N with 2 motors and PWM fo speed control
// by Graeme Patterson

#define ENA 9 
#define ENB 10
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7


int driveLeftMotor = 0;
int driveRightMotor = 0;
int motorLeft = 0;
int motorRight = 0;

void setup() {
  // setup output pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Read Joystick values
  int JoystickX = analogRead(A0);
  int JoystickY = analogRead(A1);

  //******************************************************************
  // determine if motion shuld be forwards or backwards
  if (JoystickY > 550)  // moving forwards
  {
    // set Left Motor for forward motion
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    // set Right Motor for forward motion
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);    

    // determine forward speed value
    motorLeft = JoystickY - 512;
    motorRight = JoystickY - 512;
  }
  else if (JoystickY < 480) // moving backwards
  {
    // set Left Motor for reverse motion
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    // set Right Motor for reverse motion
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);    

    // determine backwards speed value
    motorLeft = abs(JoystickY - 512);
    motorRight = abs(JoystickY - 512);
  }
  else // not moving forwards or backwards
  {
    motorLeft = 0;
    motorRight = 0;
  }

  //******************************************************************
  // Determine if motion is left or right
  int steering = abs(JoystickX - 512);
  if (JoystickX > 550) //turning right
  {
    motorLeft = motorLeft + (steering/2);
    motorRight = motorRight - (steering/2);
  }
  if (JoystickX < 480) //turning left
  {
    motorLeft = motorLeft - (steering/2);
    motorRight = motorRight + (steering/2);
  }

  //******************************************************************
  // Calculate actual PWM values for motor drive
  driveLeftMotor = map(motorLeft,0, 512, 0, 255);
  driveRightMotor = map(motorRight,0, 512, 0, 255);
  
  // Send PWM values to the motors
  analogWrite(ENA, driveLeftMotor);
  analogWrite(ENB, driveRightMotor);

}
