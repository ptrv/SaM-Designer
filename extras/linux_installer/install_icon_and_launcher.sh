#!/bin/sh

BASEDIR=$(dirname $0)

ICON_PATH=$HOME/.local/share/icons
LAUNCHER_PATH=$HOME/.local/share/applications
BIN_PATH=$HOME/bin

echo "\nSynth-A-Modeler-Designer installer :)\n"
echo "Install executable..."
mkdir -p $BIN_PATH
cp $BASEDIR/../../gui/Builds/Linux/build/Synth-A-Modeler-Designer $BIN_PATH

echo "Install launcher..."
mkdir -p $LAUNCHER_PATH
cp $BASEDIR/synthamodeler.desktop $LAUNCHER_PATH

echo "Install icon..."
mkdir -p $ICON_PATH
cp $BASEDIR/../../gui/BinaryData/synthamodeler_icon.png $ICON_PATH

echo "Done."
