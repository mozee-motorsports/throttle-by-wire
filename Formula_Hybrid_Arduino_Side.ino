  /**************************************************************************************
  * Author: Daniel Nimsgern (nimsgernd@msoe.edu)
  * Latest Changes: 4/26/2023
  * Function: Arduino Throttle by wire using PCA9685 16 servo driver
  *
  * TO CHANGE SRERVO POSITIONS:
  *   FULL OPEN POSITION:
  *    change the value of SERVO_MIN
  *       higher value = 
  *       lower value  =
  *
  *   CLOSED POSITION:
  *     change the subtracted number of SERVO_90
  *       higher value = more open
  *       lower value  = less open
  *
  *   NOTE: the closed position will change when the full open position is changed so
  *         change the full open position before changing the closed position
  *
  * TO CHANGE POTENTIAMETER RANGE:
  *   set POT_MIN and POT_MAX to the potentiameter min and max values  respectivly
  *
  * !!! Please Feel free to message me if there are any questions about the code !!!
  **************************************************************************************/ 


  /**************************************************************************************
  *                                      Includes                                       *
  **************************************************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>

  /**************************************************************************************
  *                                    Definitions                                      *
  **************************************************************************************/

// Driver I2C address
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Potentiameters
byte pot1 = A0;
byte pot2 = A1;

// Potentiameter limits
#define POT_MIN 0
#define POT_MAX 1023

// Servo limits - TODO need calibration
#define SERVO_MIN 120
#define SERVO_MAX 700
#define SERVO_90 ((SERVO_MIN+SERVO_MAX)/2)-80

// Servo update frequancy
#define SERVO_FREQ 50

// Throttle servo position
#define THROTTLE 0

  /**************************************************************************************
  *                                    Initialization                                   *
  **************************************************************************************/

void setup() 
{
  /**
  * Initialize computer serial
  * Initialize potentiameter input
  * Initialize I2C to control servo driver
  * Initialize SPI to talk to STM32H723ZG
  **/

  // Initialize computer serial
  Serial.begin(9600);

  // Initialize potentiameter input
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);

  // Initialize I2C to control servo driver
  pwm.begin();
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
  */
  pwm.setOscillatorFrequency(25000000);
  pwm.setPWMFreq(SERVO_FREQ);

  // Initialize SPI to talk to STM32H723ZG
  // TODO

}

  /**************************************************************************************
  *                                       Looped                                        *
  **************************************************************************************/

void loop() {

  /**
  * Defualt Logic
  *   Read analog inputs
  *   Send Positon to Servo Driver
  *
  * Preferred Logic (Temporary)
  *   Read analog inputs
  *   Send position to STM32H723ZG
  *   Send position to Servo driver
  *
  * Preferred Logic (Ideal setup)
  *   Read analog inputs
  *   Talk to STM32H723ZG
  *   Recieve throttle position
  *   Send position to Servo driver
  **/

  int position1 = POT_MAX - analogRead(pot1);
  int position2 = POT_MAX - analogRead(pot2);

  Serial.println(position1);
  Serial.println(position2);

  if (abs(position1-position2) > 100)
  {

    pwm.setPWM(THROTTLE, 0, SERVO_MIN);

  } else 
  {
    if (position1 > position2)
    {

      int val = analogRead(position1);

      val = map(val, POT_MIN, POT_MAX, SERVO_MIN, SERVO_90);

      Serial.println(val);

      pwm.setPWM(THROTTLE, 0, val);

    } else
    {

      int val = analogRead(position2);

      val = map(val, POT_MIN, POT_MAX, SERVO_MIN, SERVO_90);

      Serial.println(val);

      pwm.setPWM(THROTTLE, 0, val);

    }
  }
    
  delay(10);

}