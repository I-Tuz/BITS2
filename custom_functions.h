#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

#include "basic_functions.h"
#include "config.h"  

void 


readAndPlotDistances(){
    //erstellt mit Google NotebookLM
    // Abstand vom vorderen Ultraschallsensor messen
    int us_vorne_distanz = us_get_distance(TRIGF, ECHOF); // Nutzt Funktion aus basic_functions.h [7] und Pins aus config.h [8]

    // Abstand vom vorderen Time-of-Flight Sensor messen
    int tof_vorne_distanz = get_front_tof_distance();    // Nutzt Funktion aus basic_functions.h [7]

    // Optional: Abstand vom hinteren US-Sensor (falls relevant)
    // int us_hinten_distanz = us_get_distance(TRIGB, ECHOB); // Nutzt Funktion aus basic_functions.h [7] und Pins aus config.h [8]

    // Ausgabe der Messdaten für den seriellen Plotter [1-3]
    // Werte durch Leerzeichen trennen und Zeile mit Zeilenumbruch beenden [implied by plotter format]
    _print_(us_vorne_distanz);   // Erster Wert [4, 5]
    _print_(" ");                 // Trennzeichen
    _println_(tof_vorne_distanz);  // Zweiter Wert, gefolgt von Zeilenumbruch [4, 5]

    // Optional: Weitere Werte ausgeben
    // _print_(us_hinten_distanz); _println_(""); // Beispiel für nur einen weiteren Wert
    // _print_(us_vorne_distanz); _print_(" "); _print_(tof_vorne_distanz); _print_(" "); _println_(us_hinten_distanz); // Beispiel für 3 Werte

    // Eine kurze Pause kann helfen, den Plotter nicht zu überlasten und stabilere Graphen zu erhalten (dies ist externe Information, die nicht direkt in den Quellen steht).
    delay(50);
}

#endif // CUSTOMFUNCTIONS_H