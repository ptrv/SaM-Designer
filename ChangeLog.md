ChangeLog Synth-A-Modeler Designer
==================================

### 0.4.0 ###

#### New ####

#### Changes ####

#### Bug fixes ####

0.3.2
-----

#### Bug fixes ####

* Fix typo in PropertiesWindow
* Fix compile error with clang with recent JUCE
* Fix a bug that caused the Mac main menu not to update properly

0.3.1
-----

#### New ####

* Store window positions of Preferencess Window and Exporter Input
  Window in user settings
* Add `Print SAM Log` Tools menu entry for showing contents of
  `SAM-debug-compilation.txt` in Post Window
* New Menu Bar entry: Windows. Move some entries from View to Windows
  and add list with currently open MDL files and two new entries for
  switching to next or previous window
* Show unique MDL names in editing window path

#### Changes ####

* Use native title bar on `PropertiesWindow`
* Use different key bindings for previous window and nextwindow on Mac

#### Bug fixes ####

* Fix #47. Prevent editing properties when multiple objects of different
  types are selected
* Reset `PropertiesWindow` when an MDL file was closed
* Fix focus of `PropertiesWindow` when there is no selection
* Fix a bug that caused to crash the application when using 'save as...'


0.3.0
-----

#### New ####

* Add user option to toggle parameter autocorrection
* Menu entry for toggling `PropertiesWindow` always on top

#### Changes ####

* Remove user option 'open *.dsp file after compilation'
* Fix setting component focus on `ObjectsHolder`
* Refactor `SAMCmd` to use separate Thread for commands in order to not
  block the user interface
* Use current snap-grid size for the distance when moving components
  with arrow keys
* Use Introjucer GUI builder with more components
* Improve `IdManager::addId` and `IdManager::renameId` methods
* Improve focus handling of `PropertiesWindow`
* User interface: Make strings more consitent
* Improve hitTest in LinkComponent
* More info in MDLInformation
* Linux makefile: Use `-std=c++11` and set warning level higher

#### Bug fixes ####

* Fix handling of Alters when trying to set the identifier of an object
  to an already existing name
* Fix auto correction of parameter values


-------------------------------------------------------------------------------

Copyright 2013 Peter Vasil
Copying and distribution of this file, with or without modification, are
permitted provided the copyright notice and this notice are preserved.
