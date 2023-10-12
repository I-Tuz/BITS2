#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'
bold=$(tput bold)
normal=$(tput sgr0)

echo -e "$GREEN"
echo -e "Automatischer download der Bibliotheken wird vorbereitet..."
sleep 2

echo -e "$NOCOLOR"
mkdir BITS-i_Bibliotheken
echo -e "$GREEN"
echo -e "Ordner BITS-i_Bibliotheken wurde erfolgreich erstellt."
echo -e " "
sleep 1
cd BITS-i_Bibliotheken

echo -e "Starte download..."
sleep 1
echo -e "$NOCOLOR"

# BITS-i Bibliotheken
git clone https://git-ce.rwth-aachen.de/bits/bits_libs/bits_time_of_flight_controller.git
git clone https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk_imu.git
git clone https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk_ir_receiver.git
git clone https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk-led-strip-controller.git
git clone https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk-motor-driver.git
git clone https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk-pid-controller.git
git clone https://git-ce.rwth-aachen.de/thk_libs/microcontrollers/thk_servo.git
git clone https://git-ce.rwth-aachen.de/bits/bits_libs/bits_motor_driver.git

# Externe Bibliotheken, können auch über die Arduino IDE installiert werden
git clone https://github.com/ElectronicCats/mpu6050.git
git clone https://github.com/Arduino-IRremote/Arduino-IRremote.git
git clone https://github.com/adafruit/Adafruit_NeoPixel.git
git clone https://github.com/adafruit/Adafruit_VL53L0X.git

echo -e "$GREEN"
echo -e "Alle Bibliotheken wurden erfolgreich "
echo -e "in den Ordner BITS-i_Bibliotheken gedownloadet."
echo -e "$RED"
echo -e "__________________________________________________________________"
echo -e "__________________________________________________________________"
echo -e " "
echo -e "\033[31;7m${bold}WICHTIG!!!!!\033[0m"
echo -e " "
echo -e "\033[31;7m${bold}Kopiert alle Ordner aus BITS-i_Bibliotheken in den Arduino Libraries Ordner\033[0m"
echo -e "$RED"
echo -e "__________________________________________________________________"
echo -e "__________________________________________________________________"
echo -e " "

echo -e "$GREEN"
echo -e "Wenn die Bibliotheken erfolgreich in den Arduino Libraries Ordner" 
echo -n "kopiert wurden,"
read -p " mit beliebiger Taste fortfahren" x
echo -e " "

echo -e -n "\033[32;7mSoll das Basis Projekt für BITS-i gedownloadet werden? [J/N]\033[0m"
read -p " " input
if [ $input = 'j' ] || [ $input = 'J' ]; then
cd ..
git clone https://git-ce.rwth-aachen.de/bits/03_projects/bits-bare-minimum.git
echo -e "$GREEN"
echo -e "Das Basis Projekt für BITS-i wurde erfolgreich gedownloadet"
echo -e "$NOCOLOR"
fi

echo -e " "
read -p "Mit beliebiger Taste beenden" x