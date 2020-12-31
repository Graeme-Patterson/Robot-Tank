// Arduino Robot Tank
// uses L298N with 2 motors and PWM for speed control
// NodeMCU (ESP8266 12E) for controller using RemoteXY for Remote control via Phone
// by Graeme Patterson

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "TheTank"
#define REMOTEXY_WIFI_PASSWORD ""  // Open WiFi connection
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = 
  { 255,3,0,1,0,52,0,8,192,0,
  5,32,60,27,30,30,2,26,31,2,
  0,72,4,22,11,133,26,31,31,79,
  78,0,79,70,70,0,65,18,6,3,
  9,9,129,0,17,4,18,6,17,67,
  111,110,110,101,99,116,101,100,0 }; 
  
// this structure defines all the variables of your control interface 
struct {

    // input variable
  int8_t joystick_1_x; // =-100..100 x-coordinate joystick position 
  int8_t joystick_1_y; // =-100..100 y-coordinate joystick position 
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 

    // output variable
  uint8_t led_1_g; // =0..255 LED Green brightness 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_1 13

// define pins to control L298N
#define ENA D1 
#define ENB D2
#define IN1 D5
#define IN2 D6
#define IN3 D7
#define IN4 D8


int driveLeftMotor = 0;
int driveRightMotor = 0;
int motorLeft = 0;
int motorRight = 0;
int JoystickX = 0;
int JoystickY = 0;
String xvalue;
String yvalue;

void setup() {
  RemoteXY_Init (); 
  Serial.begin(115200);
  //pinMode (PIN_SWITCH_1, OUTPUT);  
  // setup output pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  RemoteXY_Handler ();
  // Turn the LED on at the Controller
  RemoteXY.led_1_g = 255;

  // use the RemoteXY structure for data transfer
  xvalue = map(RemoteXY.joystick_1_x,-100, 100,0,1024);
  yvalue = map(RemoteXY.joystick_1_y,-100, 100,0,1024);
  //Serial.println("Y = " + yvalue);
  //Serial.println("X = " + xvalue);  
  // Read Joystick values
  JoystickX = xvalue.toInt();
  JoystickY = yvalue.toInt();

  //Serial.println(JoystickX);
  //Serial.println(JoystickY); 

  if(RemoteXY.switch_1==0)
  {
    //Serial.println("Remote is Off!");
    motorLeft = 0;
    motorRight = 0;
  }
  else
  {
    //******************************************************************
    // determine if motion should be forwards or backwards
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
  }
  //******************************************************************
  // Calculate actual PWM values for motor drive
  driveLeftMotor = map(motorLeft,-512, 512, 0, 1024);
  driveRightMotor = map(motorRight,-512, 512, 0, 1024);
  Serial.print("DLM = ");
  Serial.println(driveLeftMotor);
  Serial.print("DRM = ");
  Serial.println(driveRightMotor);  
  // Send PWM values to the motors
  analogWrite(ENA, driveLeftMotor);
  analogWrite(ENB, driveRightMotor);
  
}
