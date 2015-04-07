# Synth-A-Modeler Designer

The Synth-A-Modeler project was carried out at the TU Berlin's Audio
Communication Group. The
[**Synth-A-Modeler compiler**](https://github.com/eberdahl/SaM) was
written by Edgar Berdahl, and the graphical user interface, the
[**Synth-A-Modeler Designer**](https://github.com/ptrv/SaM-Designer) was
created by Peter Vasil. We would like to graciously thank Prof. Julius
O. Smith III, Alexandros Kontogeorgakopoulos, Prof. Stefan Weinzierl,
Prof. Yann Orlarey, and the Alexander von Humboldt foundation for their
support.

## Screenshot

![Synth-A-Modeler Designer](https://github.com/ptrv/SaM-Designer/raw/master/screenshot.png
 "Synth-A-Modeler Designer")


## Project structure

**SaM**: This folder consist Edgar Berdahl's *Synth-A-Modeler compiler*
  and example model files.

<!-- **cmd2**: A C++ version of the *Synth-A-Modeler* compiler (experimental). -->

**extras**: Consists of Synth-A-Modeler file editor support for Emacs
  and Vim and a *Synth-A-Modeler Designer* Linux installer.

**gui**: This folder consist of the *Synth-A-Modeler Designer* project
  files and sources.

## Setup

The Synth-A-ModelerGUI project is heavily using the [JUCE][1] library.
It also depends on Google's [re2][2] regular expression library for
parsing Synth-A-Modeler `*.mdl` files.

### Get the sources and dependencies

Clone the Synth-A-Modeler repository:

    git clone --recursive https://github.com/ptrv/SaM-Designer.git

The `--recursive` switch tells git to pull also the JUCE submodule.
Without the switch you have to seperately get the submodule.

    git clone https://github.com/ptrv/SaM-Designer.git
    cd SaM-Designer
    git submodule update --init

Also there is a dependency on [re2][2], the regular expression library
used in this project, which is also a submodule of this project.

For Mac OS X and Windows there are already binaries of the library
provided, on Linux consider the following steps to get and compile the
library:

    cd SaM-Designer
    git submodule update --init
    cd gui/Libs
    ./build_re2.sh

### Compiling Introjucer

Introjucer is part of the JUCE library. It is the project manager for
JUCE projects. Every time you have to make changes to the
Synth-A-ModelerGUI projects (i.e. add new source files, change compile
options), you have to use the Introjucer application. But first it must
be compiled.

Depending on your development machine there are project files for several
platforms. For linux:

    cd SaM-Designer
    cd juce/extras/Introjucer/Builds/Linux
    make
    build/Introjucer

With the Introjucer you can now open `*.jucer` files, like the one at the
`SaM-Designer/gui` path.

### Compiling Synth-A-Modeler Designer

There are project files for different operating systems provided. For
Mac OS X a Xcode project, for Windows Visual Studio 2010 project and a
Makefile for Linux.

To compile on Linux:

    cd SaM-Designer
    cd gui/Build/Linux
    make

Run the application:

    build/Synth-A-Modeler-Designer


[1]: http://rawmaterialsoftware.com/juce.php
[2]: https://code.google.com/p/re2/

## Usage

Please refer to the [Wiki][3] for information on the usage of
*Synth-A-Modeler Designer*.

[3]: https://github.com/ptrv/Synth-A-Modeler/wiki
