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
int joystick_x = 0;
int joystick_y = 0;
int JoystickX = 0;
int JoystickY = 0;

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
  joystick_x = analogRead(A0);
  joystick_y = analogRead(A1);
  JoystickX = map(joystick_x, 0,1024,-512, 512);
  JoystickY = map(joystick_y, 0,1024,-512, 512);

    //get initial motor speed --> this will be the PWM value based on Joystick absolute Y value
    motorspeed = abs(JoystickY);
    // set absolute left and right motor speeds
    motorLeft = motorspeed + (JoystickX/2);
    motorRight = motorspeed - (JoystickX/2);

    // Now set the overall direction
    if (JoystickY >= 0)
      TankDir = 1;
    else
      TankDir = -1;

    // Set the motor directions now based on over all tank direction and required track direction for joystick input
    if ((TankDir * motorLeft)>=0)
    {
        // set motor Left Direction to forwards
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);        
    }
    else
    {
      // set the motor Left Direction to backwards
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    if ((TankDir * motorRight) >= 0)
    {
      // set motor Right Direction to forwards
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    else
    {
      // set the motor Right Direction to backwards
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);  
    }

  // at this point we have motorLeft and motorRight values
  driveLeftMotor = abs(2 * motorLeft);
  driveRightMotor = abs(2 * motorRight);
  if(driveLeftMotor > 1024)
    driveLeftMotor = 1024;
  if(driveRightMotor > 1024)
    driveRightMotor = 1024;

  
  Serial.print("DLM = ");
  Serial.println(driveLeftMotor);
  Serial.print("DRM = ");
  Serial.println(driveRightMotor);  
  // Send PWM values to the motors
  analogWrite(ENA, driveLeftMotor);
  analogWrite(ENB, driveRightMotor);
}
