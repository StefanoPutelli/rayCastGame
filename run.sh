#!/bin/bash

case "$1" in
    "terminal")
        g++ main.cpp ./stuff/terminal_display.cpp ./stuff/game.cpp -o ./out/terminal && ./out/terminal
        ;;
    "gui")
        g++ main.cpp ./stuff/gui_display.cpp ./stuff/game.cpp -o ./out/gui `pkg-config gtkmm-3.0 --cflags --libs` && ./out/gui
        ./gui
        ;;
    *)
        # Operazione da eseguire quando l'argomento non è riconosciuto
        echo "Invalid argument"
        exit 1
        ;;
esac
