#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

#include "config.h"

// ─── State-Machine Zustände ────────────────────────────────
enum FahrZustand { IDLE, KURVE_LAUFEN, RAUMSCHAFFEN };
static FahrZustand   zustand        = IDLE;
static unsigned long zustandStart   = 0;
static int           kurvenrichtung = 0;  // -1=links, 1=rechts

// ─── Sensor-Variablen ────────────────────────────────────────
extern bits_TimeOfFlightController tof_controller;
static int tof_front, tof_Vorne_links, tof_Vorne_rechts;
static int us_front, front_dist;

// ─── Schwellen / Zeiten ──────────────────────────────────────
static constexpr int FRONT_MIN     = 45;   // cm, freier Weg voraus
static constexpr int SIDE_MIN      = 13;   // cm, kritischer Seitenabstand
static constexpr int EMERGENCY_MIN = 20;   // cm, Notstopp vorne
static constexpr int REVERSE_TIME  = 500;  // ms, Rückwärtsdauer

// ─── Hilfsfunktion: Alle Sensoren auslesen ────────────────────
inline void readAllSensors() {
  tof_front        = tof_controller.get_distance_cm(0);
  tof_Vorne_links  = tof_controller.get_distance_cm(1);
  tof_Vorne_rechts = tof_controller.get_distance_cm(2);
  us_front         = us_get_distance(TRIGF, ECHOF);
  front_dist       = (tof_front + us_front) / 2;

  _println_("Sensoren: front=" + String(front_dist)
           + "   L=" + String(tof_Vorne_links)
           + "   R=" + String(tof_Vorne_rechts));
}

// ─── Labyrinth-Fahrt ohne delay() ────────────────────────────
inline void labyrinthFahrt() {
  unsigned long now = millis();

  // ① Sensoren updaten
  readAllSensors();

  // ② Sofort-Notstopp vorne oder seitlich
  bool sideCritical = (tof_Vorne_links < SIDE_MIN || tof_Vorne_rechts < SIDE_MIN);
  if ((front_dist   < EMERGENCY_MIN || sideCritical)
      && zustand != RAUMSCHAFFEN) {
    Motor.stop();
    // Lenke weg von zu naher Seite
    if      (tof_Vorne_links  < SIDE_MIN) ServoSteer.turn(STEER_ANGLE_MIN);
    else if (tof_Vorne_rechts < SIDE_MIN) ServoSteer.turn(STEER_ANGLE_MAX);
    // sonst frontal too close → gerade aus lenken
    else                                  ServoSteer.turn(STEER_START_POS);

    zustandStart = now;
    zustand      = RAUMSCHAFFEN;
  }

  // ③ State-Machine
  switch (zustand) {

    // ── IDLE: geradeaus, Kurve oder Rückwärts? ────────────────
    case IDLE:
      // a) Geradeaus, wenn frei UND keine Seitengefahr
      if (front_dist > FRONT_MIN && !sideCritical) {
        ServoSteer.turn(STEER_START_POS);
        Motor.drive_forward(velocity);

      // b) Kurve starten, wenn eine Seite genug Platz hat
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

      // c) sonst Sackgasse → zurücksetzen
      } else {
        Motor.stop();
        zustandStart = now;
        zustand      = RAUMSCHAFFEN;
      }
      break;

    // ── KURVE_LAUFEN: bis Front frei oder Seitengefahr ─────────
    case KURVE_LAUFEN:
      // a) Kurve beendet, sobald Front wieder frei
      if (front_dist > FRONT_MIN) {
        Motor.stop();
        unsigned long turnTime = now - zustandStart;
        _println_("Kurvenzeit (ms): " + String(turnTime));
        ServoSteer.turn(STEER_START_POS);
        Motor.drive_forward(velocity);
        zustand = IDLE;

      // b) innere Wand zu nah → Abbruch
      } else if ((kurvenrichtung ==  -1 && tof_Vorne_rechts < SIDE_MIN)
              || (kurvenrichtung == 1 && tof_Vorne_links  < SIDE_MIN)) {
        Motor.stop();
        zustandStart = now;
        zustand      = RAUMSCHAFFEN;
      }
      break;

    // ── RAUMSCHAFFEN: Rückwärts, bis genug Platz ist ────────────
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
