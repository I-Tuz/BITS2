#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'
bold=$(tput bold)
normal=$(tput sgr0)

echo -e "$GREEN"
echo -e "Automatischer Download der Bibliotheken wird vorbereitet..."
sleep 2

echo -e "Starte Download..."
sleep 1
echo -e "$NOCOLOR"

# BITS Bibliotheken
git clone https://git-ce.rwth-aachen.de/bits_libs/bits-i-libraries.git

echo -e "$GREEN"
echo -e "Alle Bibliotheken wurden erfolgreich "
echo -e "in den Ordner BITS-i_Bibliotheken gedownloadet."
echo -e "$RED"
echo -e "__________________________________________________________________"
echo -e "__________________________________________________________________"
echo -e " "
echo -e "\033[31;7m${bold}WICHTIG!!!!!\033[0m"
echo -e " "
echo -e "\033[31;7m${bold}Bitte entzippen Sie die Datei und kopieren Sie "
echo -e "alle Ordner aus BITS_Bibliotheken in den Arduino Libraries Ordner\033[0m"
echo -e "$RED"
echo -e "__________________________________________________________________"
echo -e "__________________________________________________________________"
echo -e " "
echo -e "Dieser Versionsstand ist mit BITS1, BITS2 und BITS3 kompatibel!"

echo -e "$GREEN"
echo -e "Wenn die Bibliotheken erfolgreich in den Arduino Libraries Ordner" 
echo -n "kopiert wurden, "
read -p "mit beliebiger Taste beenden" x