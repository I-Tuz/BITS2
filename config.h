/*
##########################################################
######### DIESES SKRIPT NICHT BEARBEITEN!!! ##############
##########################################################
*/

/*      BITS-i sensor shematic
 *    
 *     +----#2-----L4-----#4----+
 *   L1|                        |L3
 *     |        <------         |
 *  US1| #0      BITS-i         |US2
 *     |                        |
 *   L1|                        |L3
 *     +----#1-----L2-----#3----+
 *
 * US1 - 2: Ultraschall Sensoren
 * #0 - #4: Time of Flight Sensoren  
 * L1 - 4: LED Streifen            
 *
 *                   #2   L4  #3
 *                   ___      ___
 *                  [_ _]    [_ _]   _
 *             /|  ___S________S_   | \   L3
 *            / |-/        ____  [++| |+
 *    US1 #0 <<<<<---<|  |>____O)<ooo>|   US2
 *            \ |-\___ ________ _[++| |+  
 *             \|    _S_      _S_   |_/   L3    
 *                  [___]    [___]      
 *                   #1   L2  #4
 *
 */

#ifndef BITSIPARAMETER_H
#define BITSIPARAMETER_H

/* ************** *
 * Debug Settings *
 * ************** */
// #define DEBUG_TOF
// #define DEBUG_IMU
// #define DEBUG_COURSE

#define DEBUG_SERIAL
// #define WIFI_COMMUNICATION

#ifdef DEBUG_SERIAL
#define _println_(x) Serial.println(x)
#define _print_(x) Serial.print(x)
#else
#ifdef WIFI_COMMUNICATION
#define _println_(x) Serial3.println(x)
#define _print_(x) Serial3.print(x)
#else
#define _println_(x)
#define _print_(x)
#endif
#endif

/* ************ *
 * Bibliotheken *
 * ************ */
#include <bits_motor_driver_tb6612fng.h>
#include <thk_servo_controller.h>
#include <bits_time_of_flight_controller.h>
#include <thk_ir_controller.h>
#include <RF24.h>
#include <thk_imu.h>
#include <thk_ledstrip_controller.h>
#include <util/atomic.h> // For the ATOMIC_BLOCK macro

/* ************ *
 * LED Streifen *
 * ************ */
const int FRONT_LED_NUM = 13;
const int RIGHT_LED_NUM = 10;
const int BACK_LED_NUM = 10;
const int LEFT_LED_NUM = 10;

thk_LedStripController front_strip(FRONT_LED_NUM,33);     
thk_LedStripController left_strip(LEFT_LED_NUM, 35);   
thk_LedStripController right_strip(RIGHT_LED_NUM, 37);  
thk_LedStripController back_strip(BACK_LED_NUM,39);     
bool state = true;
uint8_t red, green, blue;

/* ************* *
 * Antriebsmotor *
 * ************* */
bits_MotorDriverTB6612FNG Motor;
uint8_t velocity = 100;
uint8_t velocity_backward = 80; // Geschwindigkeit für rückwährts fahren

/* ********** *
 * Servomotor *
 * ********** */
const int STEER_SERVO_PIN = 11;
const int STEER_ANGLE_MAX = 107;    // Muss eventuell angepasst werden!
const int STEER_ANGLE_MIN = 43;     // Muss eventuell angepasst werden!
const int STEER_START_POS = 90;     // Muss eventuell angepasst werden!
int STEER_HALF_RIGHT = STEER_START_POS + (STEER_ANGLE_MAX-STEER_START_POS)/2;
int STEER_HALF_LEFT = STEER_START_POS - (STEER_START_POS-STEER_ANGLE_MIN)/2;
thk_ServoController ServoSteer(STEER_SERVO_PIN, STEER_START_POS, STEER_ANGLE_MAX, STEER_ANGLE_MIN);

const int SEARCH_SERVO_PIN = 12;
const int SEARCH_ANGLE_MAX = 180;   // Muss eventuell angepasst werden!
const int SEARCH_ANGLE_MIN = 0;     // Muss eventuell angepasst werden!
const int SEARCH_START_POS = 90;    // Muss eventuell angepasst werden!
thk_ServoController ServoSearch(SEARCH_SERVO_PIN, SEARCH_START_POS, SEARCH_ANGLE_MAX, SEARCH_ANGLE_MIN);

float servo_steering_angle;

/* ****** *
 * Buzzer *
 * ****** */
const int BUZZER_PIN = A5;

/* *** *
 * IMU *
 * *** */
thk_IMU imu(0);
float yaw, pitch, roll;

/* ******************* *
 * Ultraschallsensoren *
 * ******************* */
#define TRIGF 4
#define ECHOF 2

#define TRIGB 7
#define ECHOB 3
int entfernung;
double dauer;

/* *********************** *
 * Time of flight Sensoren *
 * *********************** */
int tof_l_1, tof_r_1, tof_l_2, tof_r_2;
bits_TimeOfFlightController tof_controller(8000);

// Choose which ToF is mounted on the servo
#define TOF_VL53L0X
// #define TOF_TFLUNA

#ifdef TOF_TFLUNA      // TFLuna-I2C Library v.0.1.1
#include <TFLI2C.h>
TFLI2C tflI2C;

int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address or
                                // set variable to your own value
#endif

/* ************ *
 * IR Empfänger *
 * ************ */
const uint8_t IR_PIN = 46;
thk_IrController ir_sensor(IR_PIN);

/* ******** *
 * NRF24L01 *
 * ******** */
int ReceivedMessage[3];
RF24 radio(49, 53); // NRF24L01 used SPI pins + CE Pin 49 and CSN PIN 53 on the Mega Pro
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01

/* ********* *
 * Sonstiges *
 * ********* */
long start;
int sensor_distance = 19; // Abstand der Seitlichen ToFs von einander in cm
float weighted_distance;
const int MIN_DIST_F = 35;  // Mindest Abstand für Vorne
const int MIN_DIST_B = 10;  // Mindest Abstand für Hinten
const int PATH_DISTANCE = 60; // Abstand, der auf eine Abzweigung deutet.
int US_distance_f, US_distance_b;
int tof_distance_front;
float output_value;

#endif