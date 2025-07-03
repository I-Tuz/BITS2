# **BITS-i Bare Minimum**

Dieses Projekt dient als Grundlage für alle Projekte mit BITS-i.<br />
<br />

## **Voraussetzung**

Folgende Bibliotheken inklusive der jeweiligen Voraussetzungen sind für dieses Projekt notwendig:
- [thk_ir_receiver](https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk_ir_receiver.git)
- [thk_servo](https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk_servo.git)
- [bits_time_of_flight_controller](https://git-ce.rwth-aachen.de/bits/bits_libs/bits_time_of_flight_controller.git)
- [thk_led_strip_controller](https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk-led-strip-controller.git)
- [thk_imu](https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk_imu.git)
- [bits_motor_driver](https://git-ce.rwth-aachen.de/bits/bits_libs/bits_motor_driver.git)
- [RF24 by TMRh20](https://github.com/nRF24/RF24)<br />
<br />

## **Installation:**

Um dieses Projekt verwenden können, muss dieses Repository geklont und die `bits-bare-minimum.ino` mit der Arduino IDE geöffnet werden.
Es muss darauf geachtet werden, dass der Ordner, in dem sich die `bits-bare-minimum.ino` befindet, `bits-bare-minimum` lautet.

## **Anwendung:**

**`config.h`:** Hier werden alle für *BITS-i* benötigten Bibliotheken inkludiert, Objekte instanziiert und Parameter definiert. **[Hier dürfen lediglich die Max-, Min- und Startposition der Servomotoren angepasst werden. Alle weiteren Änderungen dürfen nur nach erfolgter Rücksprache mit den Betreuuern gemacht werden!!!]**

**`basic_functions.h`:** Hier befinden sich alle zur Verfügung gestellten Funktionen für *BITS-i*. **[Diese Datei darf nicht bearbeitet werden!!!]**

**`custom_config.h`:** Hier können alle weiteren benötigten Objekte instanziiert und Parameter definiert werden. Diese Datei darf frei bearbeitet werden.

**`custom_functions.h`:** Hier gehören alle weiteren benötigten Funktionen hin, welche für das Projekt geschrieben wurden.

**`bits-bare-minimum.ino`:** Dies ist die Basis für ein Projekt mit *BITS-i*.


