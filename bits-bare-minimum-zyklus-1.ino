/***************************************************
 ________  ___  _________  ________    _______     
|\   __  \|\  \|\___   ___\\   ____\  /  ___  \    
\ \  \|\ /\ \  \|___ \  \_\ \  \___|_/__/|_/  /|   
 \ \   __  \ \  \   \ \  \ \ \_____  \__|//  / /   
  \ \  \|\  \ \  \   \ \  \ \|____|\  \  /  /_/__  
   \ \_______\ \__\   \ \__\  ____\_\  \|\________\
    \|_______|\|__|    \|__| |\_________\\|_______|
                             \|_________|          
                                                        
  Name:
  <Hier kommt der Name des Projekts>

  Description:
  <Hier kommt was dieses Skript macht>
  

  Author:
  <Hier kommt dein Name>    

**************************************************/
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

#include "config.h"
#include "basic_functions.h"
#include "custom_config.h"
#include "custom_functions.h"

void setup(){
    // Hier wird entschieden wie die Serielle Kommunikation vom Mega erfolgen soll. Weiteres siehe in config.h
    #ifdef DEBUG_SERIAL
    Serial.begin(115200);
    #endif
    #ifdef WIFI_COMMUNICATION
    Serial3.begin(115200);
    #endif

    // Initialisierung der optischen Time-of-Flight Sensoren
    tof_controller.init();

    // Starten der I2C Kommunikation
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

    // Initialisierung der LED-Streifen
    front_strip.init();
    back_strip.init();
    left_strip.init();
    right_strip.init();

    // Initialisierung der IMU inkl. Lichtspiel
    imu.init();
    start = millis();
    _println_("Waiting for IMU getting ready. Please do not move the car!");
    _print_("Waiting");
    set_led_color(255,0,0);
    for (int k=0; k<13; k++){
        for (int i=0; i<FRONT_LED_NUM; i++){
            front_strip.set_pixel_color(i,thk_LedStripController::Color(red, green, blue));
            front_strip.show();
            delay(500/FRONT_LED_NUM);
            _print_(".");
        }
        for (int i=0; i<FRONT_LED_NUM; i++){
            front_strip.set_pixel_color(i,thk_LedStripController::Color(0, 0, 0));
            front_strip.show();
            delay(500/FRONT_LED_NUM);
            _print_(".");
        }
        red -= 20;  
        green += 20;   
    }

    // Pin deklaration der Ultraschallsensoren
    pinMode(TRIGF, OUTPUT);
    pinMode(TRIGB, OUTPUT);
    pinMode(ECHOF, INPUT);
    pinMode(ECHOB, INPUT);

    // Initialisierung des Motors
    Motor.init();    

    // Initialisierung der Servomotoren
    ServoSteer.begin();
    ServoSearch.begin();

    // Initialisierung abgeschlossen
    _println_("Ready!");
    buzzer_signal(350, 2, 100);
    
    set_led_color(0,100,0);
    all_led_blink(2,200);
}

void loop(){

}

