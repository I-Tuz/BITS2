#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

#include "config.h"

// Eigene Variablen deklarieren
extern bits_TimeOfFlightController tof_controller;
int tof_front, tof_Vorne_links, tof_Vorne_rechts;
int tof_Hinten_links, tof_Hinten_rechts;

inline void readAllToFSensors() {
  tof_front       = tof_controller.get_distance_cm(0); // ToF #0
  tof_Vorne_links   = tof_controller.get_distance_cm(1); // ToF #1
  tof_Vorne_rechts  = tof_controller.get_distance_cm(2); // ToF #2
  tof_Hinten_rechts = tof_controller.get_distance_cm(3); // ToF #3
  tof_Hinten_links  = tof_controller.get_distance_cm(4); // ToF #4

  _println_("--- TOF-Sensorwerte ---");
  _println_(String("Front (Haube)     : ") + tof_front + " cm");
  _println_(String("Hinten links       : ") + tof_Hinten_links + " cm");
  _println_(String("Hinten rechts      : ") + tof_Hinten_rechts + " cm");
  _println_(String("Vorne links      : ") + tof_Vorne_links + " cm");
  _println_(String("Vorne rechts     : ") + tof_Vorne_rechts + " cm");
}
void labyrinthFahrt() {
  readAllToFSensors();

  int min_dist = 40; // Mindestabstand in cm

  if (tof_Vorne_rechts > min_dist) {
    // Rechts frei → rechts abbiegen
    ServoSteer.turn(STEER_ANGLE_MAX);
    Motor.drive_forward(velocity);
    delay(1500);
    Motor.stop();
    ServoSteer.turn(STEER_START_POS); // wieder gerade
  } else if (tof_front > min_dist) {
    // Geradeaus frei → weiterfahren
    ServoSteer.turn(STEER_START_POS);
    Motor.drive_forward(velocity);
  } else if (tof_Vorne_links > min_dist) {
    // Links frei → links abbiegen
    ServoSteer.turn(STEER_ANGLE_MIN);
    Motor.drive_forward(velocity);
    delay(1500);
    Motor.stop();
    ServoSteer.turn(STEER_START_POS);
  } else {
    // Sackgasse → anhalten
    Motor.stop();
  }

  delay(200); // kleine Pause zur Stabilisierung
}



#endif // CUSTOM_FUNCTIONS_H
