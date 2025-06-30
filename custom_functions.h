#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

#include "config.h"

// ─── Zustände ─────────────────────────────────────────────────
enum FahrZustand { IDLE, KURVE_LAUFEN, RAUMSCHAFFEN };
static FahrZustand zustand        = IDLE;
static unsigned long zustandStart = 0;
static int kurvenrichtung         = 0;  // -1 links, 1 rechts

// ─── Sensor-Variablen ───────────────────────────────────────────
extern bits_TimeOfFlightController tof_controller;
static int tof_front, tof_Vorne_links, tof_Vorne_rechts;
static int us_front, front_dist;

// ─── Schwellen / Zeiten ─────────────────────────────────────────
static constexpr int FRONT_MIN     = 60;   // ab hier schon einlenken!
static constexpr int SIDE_MIN      = 25;   // Sicherheitsabstand Seite
static constexpr int EMERGENCY_MIN = 20;   // ab hier sofort rückwärts
static constexpr int REVERSE_TIME  = 500;  // ms

// ─── Sensoren auslesen ───────────────────────────────────────────
inline void readAllSensors() {
  tof_front       = tof_controller.get_distance_cm(0);
  tof_Vorne_links = tof_controller.get_distance_cm(1);
  tof_Vorne_rechts= tof_controller.get_distance_cm(2);
  us_front        = us_get_distance(TRIGF, ECHOF);
  front_dist      = (tof_front + us_front) / 2;

  // Debug:
  _println_("Front_dist: " + String(front_dist)
           + "  ToF_L:" + String(tof_Vorne_links)
           + "  ToF_R:" + String(tof_Vorne_rechts));
}

// ─── Labyrinth-Routine ───────────────────────────────────────────
inline void labyrinthFahrt() {
  unsigned long now = millis();

  // 1) Sensoren updaten
  readAllSensors();

  // 2) Instant-Notstopp: Wenn Front zu nah → in RAUMSCHAFFEN wechseln
  if (front_dist < EMERGENCY_MIN && zustand != RAUMSCHAFFEN) {
    Motor.stop();
    zustandStart = now;
    zustand      = RAUMSCHAFFEN;
  }

  // 3) State Machine
  switch (zustand) {

    // ── IDLE: Geradeaus oder Kurve wählen ──────────────────────────
    case IDLE:
      // a) wenn vorne wirklich Platz → geradeaus
      if (front_dist > FRONT_MIN) {
        ServoSteer.turn(STEER_START_POS);
        Motor.drive_forward(velocity);

      // b) wenn eine Seite mehr Platz hat → starten wir die Kurve
      } else if (tof_Vorne_links > FRONT_MIN || tof_Vorne_rechts > FRONT_MIN) {
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

      // c) Sackgasse → Rückwärts
      } else {
        Motor.stop();
        zustandStart = now;
        zustand      = RAUMSCHAFFEN;
      }
      break;

    // ── KURVE_LAUFEN: Dynamisch beenden ───────────────────────────
    case KURVE_LAUFEN:
      // a) sobald vorne wieder frei → Kurve beenden
      if (front_dist > FRONT_MIN) {
        Motor.stop();
        unsigned long turnTime = now - zustandStart;
        _println_("Kurvenzeit (ms): " + String(turnTime));
        ServoSteer.turn(STEER_START_POS);
        Motor.drive_forward(velocity);
        zustand = IDLE;

      // b) wenn innere Wand zu nah → abbrechen
      } else if ((kurvenrichtung ==  1 && tof_Vorne_rechts < SIDE_MIN) ||
                 (kurvenrichtung == -1 && tof_Vorne_links  < SIDE_MIN)) {
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
