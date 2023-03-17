#!/bin/bash

case "$1" in
    "terminal")
        g++ main_terminal.cpp ./stuff/terminal_display.cpp ./stuff/game.cpp -ljsoncpp -o ./out/terminal && ./out/terminal terminal
        ;;
    "gui")
        g++ main_gui.cpp ./stuff/game.cpp ./stuff/gui_display.cpp -ljsoncpp -lGL -lGLU -lglut -o ./out/gui && ./out/gui
        ;;
    *)
        # Operazione da eseguire quando l'argomento non è riconosciuto
        echo "Invalid argument"
        exit 1
        ;;
esac
