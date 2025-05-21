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
  <BITS2 G18>

  Description:
  <BITS-i Fahrzeug Lernprojekt von 2. u. 3.Semester>
  
  Author:
  <Ibragim Tuzhaev>
  <Michal Kwiatkowski>  
  <Yunus Emre Giyim>
  <delo mahmodo>
  <david schneeberger>
  <Maximilian Kaehm >      

**************************************************/

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

    //Spätestens vor der letzten Abgabe auskommentieren, da sehr viel Zeit dabei verloren geht! 
    initial_led_show();

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
  drive_forward_controlled_velocity();
}
