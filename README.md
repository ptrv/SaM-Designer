# Synth-A-Modeler

The Synth-A-Modeler project was carried out at the TU Berlin's Audio
Communication Group. The Synth-A-Modeler compiler was written by Edgar
Berdahl, and the graphical user interface was created by Peter Vasil. We
would like to graciously thank Prof. Julius O. Smith III, Alexandros
Kontogeorgakopoulos, Prof. Stefan Weinzierl, Prof. Yann Orlarey, and the
Alexander von Humboldt foundation for their support.

## Screenshot

![Synth-A-ModelerGUI](https://github.com/ptrv/Synth-A-Modeler/raw/master/screenshot.png
 "Synth-A-ModelerGUI")


## Project structure

**cmd**: This folder consist Edgar Berdahl's *Synth-A-Modeler* compiler
  and example model files.

**cmd2**: A C++ version of the *Synth-A-Modeler* compiler.

**extras**: Consists of Synth-A-Modeler file Editor support for Emacs
  and Vim and an *Synth-A-ModelerGUI* Linux installer.

**gui**: This folder consist the *Synth-A-ModelerGUI* project.

## Setup

The Synth-A-ModelerGUI project is heavily using the [JUCE][1] library.
It also depends on Google's [re2][2] regular expression library for
parsing Synth-A-Modeler `*.mdl` files.

### Get the sources and dependencies

Clone the Synth-A-Modeler repository:

    git clone --recursive https://github.com/ptrv/Synth-A-Modeler.git

The `--recursive` switch tells git to pull also the JUCE submodule.
Without the switch you have to seperately get the submodule.

    git clone https://github.com/ptrv/Synth-A-Modeler.git
    cd Synth-A-Modeler
    git submodule update --init

Also there is a dependency on [re2][2], the regular expression library
used in this project.

For Mac OS X and Windows there are already binaries of the library
provided, on Linux consider the following steps to get and compile the
library:

    cd Synth-A-Modeler
    cd gui/Libs
    ./get_and_compile_re2.sh

### Compiling Introjucer

Introjucer is part of the JUCE library. It is the project manager for
JUCE projects. Every time you have to make changes to the
Synth-A-ModelerGUI projects (i.e. add new source files, change compile
options), you have to use the Introjucer application. But first it must
be compiled.

Depending on your development machine there are project files for several
platforms. For linux:

    cd Synth-A-Modeler
    cd juce/extras/Introjucer/Builds/Linux
    make
    build/Introjucer

With the Introjucer you can now open *.jucer files, like the one at the
`Synth-A-Modeler/gui` path.

### Compiling Synth-A-ModelerGUI

There are project files for different operating systems provided. For
Mac OS X a Xcode project, for Windows Visual Studio 2010 project and a
Makefile for Linux.

To compile on Linux:

    cd Synth-A-Modeler
    cd gui/Build/Linux
    make

Run the application:

    build/Synth-A-ModelerGUI


[1]: http://rawmaterialsoftware.com/juce.php
[2]: https://code.google.com/p/re2/
