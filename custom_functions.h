#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

#include "config.h"

// ─── State-Machine Zustände ────────────────────────────────────────
enum FahrZustand { IDLE, KURVE_LAUFEN, RAUMSCHAFFEN };
static FahrZustand   zustand        = IDLE;
static unsigned long zustandStart   = 0;
static int           kurvenrichtung = 0;  // -1 = links, 1 = rechts

// ─── Sensor-Variablen ───────────────────────────────────────────────
extern bits_TimeOfFlightController tof_controller;
static int tof_front, tof_Vorne_links, tof_Vorne_rechts;
static int us_front, front_dist;

// ─── Schwellen / Zeiten ─────────────────────────────────────────────
static constexpr int FRONT_MIN     = 40;   // cm, freier Weg voraus
static constexpr int SIDE_MIN      = 10;   // cm, Mindestabstand vorn-links/-rechts
static constexpr int EMERGENCY_MIN = 20;   // cm, sofort Rückwärts
static constexpr int REVERSE_TIME  = 500;  // ms, Rückwärtsdauer

// ─── Hilfsfunktion: Alle Sensoren auslesen ──────────────────────────
inline void readAllSensors() {
  tof_front        = tof_controller.get_distance_cm(0);
  tof_Vorne_links  = tof_controller.get_distance_cm(1);
  tof_Vorne_rechts = tof_controller.get_distance_cm(2);
  us_front         = us_get_distance(TRIGF, ECHOF);
  front_dist       = (tof_front + us_front) / 2;

  // Debug-Ausgabe
  _println_("Front-Dist: " + String(front_dist)
           + "  ToF L: " + String(tof_Vorne_links)
           + "  ToF R: " + String(tof_Vorne_rechts));
}

// ─── Labyrinth-Fahrt ohne delay() ──────────────────────────────────
inline void labyrinthFahrt() {
  unsigned long now = millis();

  // ① Sensoren updaten
  readAllSensors();

  // ② Globaler Notstopp: wenn FRONT oder vorn-links/-rechts zu nah
  if ((front_dist < EMERGENCY_MIN
       || tof_Vorne_links < SIDE_MIN
       || tof_Vorne_rechts < SIDE_MIN)
      && zustand != RAUMSCHAFFEN) {
    Motor.stop();
    zustandStart = now;
    zustand      = RAUMSCHAFFEN;
  }

  // ③ State Machine
  switch (zustand) {

    // ── IDLE: Geradeaus, Kurve wählen oder Rückwärts ───────────────
    case IDLE:
      if (front_dist > FRONT_MIN) {
        ServoSteer.turn(STEER_START_POS);
        Motor.drive_forward(velocity);

      } else if (tof_Vorne_links > FRONT_MIN
                 || tof_Vorne_rechts > FRONT_MIN) {
        // Seite mit mehr Platz wählen
        if (tof_Vorne_links > tof_Vorne_rechts) {
          kurvenrichtung = -1;
          ServoSteer.turn(STEER_ANGLE_MIN);
        } else {
          kurvenrichtung = 1;
          ServoSteer.turn(STEER_ANGLE_MAX);
        }
        Motor.drive_forward(velocity);
        zustandStart = now;
        zustand      = KURVE_LAUFEN;

      } else {
        Motor.stop();
        zustandStart = now;
        zustand      = RAUMSCHAFFEN;
      }
      break;

    // ── KURVE_LAUFEN: Dynamisch beobachten und ggf. abbrechen ────
    case KURVE_LAUFEN:
      // a) Kurve fertig, sobald Front wieder frei
      if (front_dist > FRONT_MIN) {
        Motor.stop();
        unsigned long turnTime = now - zustandStart;
        _println_("Kurvenzeit (ms): " + String(turnTime));
        ServoSteer.turn(STEER_START_POS);
        Motor.drive_forward(velocity);
        zustand = IDLE;

      // b) Innere Wand zu nah (nochmal extra ) → abbrechen
      } else if ((kurvenrichtung ==  1 && tof_Vorne_rechts < SIDE_MIN)
              || (kurvenrichtung == -1 && tof_Vorne_links  < SIDE_MIN)) {
        Motor.stop();
        zustandStart = now;
        zustand      = RAUMSCHAFFEN;
      }
      break;

    // ── RAUMSCHAFFEN: Rückwärts fahren, dann Neustart ─────────────
    case RAUMSCHAFFEN:
      if (now - zustandStart < REVERSE_TIME) {
        Motor.drive_backward(velocity_backward);
      } else {
        Motor.stop();
        ServoSteer.turn(STEER_START_POS);
        zustand = IDLE;
      }
      break;
  }
}

#endif // CUSTOM_FUNCTIONS_H
