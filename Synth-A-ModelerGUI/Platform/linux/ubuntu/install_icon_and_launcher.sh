#!/bin/sh

BASEDIR=$(dirname $0)

ICON_PATH=$HOME/.local/share/icons
LAUNCHER_PATH=$HOME/.local/share/applications
BIN_PATH=$HOME/bin

echo "\nSynth-A-ModelerGUI installer :)\n"
echo "Install executable..."
mkdir -p $BIN_PATH
cp $BASEDIR/../../../Builds/Linux/build/Synth-A-ModelerGUI $BIN_PATH

echo "Install launcher..."
mkdir -p $LAUNCHER_PATH
cp $BASEDIR/synthamodeler.desktop $LAUNCHER_PATH

echo "Install icon..."
mkdir -p $ICON_PATH
cp $BASEDIR/../../../BinaryData/synthamodeler_icon.png $ICON_PATH

echo "Done."
