/*
  ==============================================================================

    ObjectsHolder.h
    Created: 11 Apr 2012 5:10:20pm
    Author:  Peter Vasil

  ==============================================================================

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

 */

#ifndef __OBJCOMP_H_F3604232__
#define __OBJCOMP_H_F3604232__

namespace synthamodeler
{
class BaseObjectComponent;
class ObjectComponent;
class MDLFile;
class ObjController;
class SelectableObject;
class SnapGridPainter;
class VariablesPanel;
class RedrawOptionsPanel;
class DirectedGraph;
class ContentComp;
/**
 * Component which contains all objects.
 */
class ObjectsHolder : public Component,
                      public LassoSource <SelectableObject*>,
                      public ChangeListener,
                      public Timer
{
public:
    ObjectsHolder(ObjController& objController_);
    ~ObjectsHolder();

    //==========================================================================
    void paint(Graphics& g);
    void resized();

    void changeListenerCallback(ChangeBroadcaster*);
    void updateComponents();

    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);

    bool keyPressed(const KeyPress& key);

    bool dispatchMenuItemClick(const ApplicationCommandTarget::InvocationInfo& info);

    void setMDLFile(MDLFile* newMDLFile);
    void reloadMDLFile();

    void editObjectProperties(BaseObjectComponent* oc);

    void findLassoItemsInArea(Array <SelectableObject*>& results,
                              const Rectangle<int>& area);

    SelectedItemSet <SelectableObject*>& getLassoSelection();
    //==========================================================================
    void deselectAllSelectedObjects();
    ObjectComponent* getSelectedObject(int index);
    void updateSelectedObjects();

    ObjController& getObjController() const { return objController; }

//    const Rectangle<int> getComponentArea() const;
    void showLinkPopupMenu(String so, String eo);
    void showAudioConnectionPopupMenu();

    void deleteSelectedObjects();

    const Rectangle<int> getObjectsExtent() const;
    const Rectangle<int> getObjectsBounds() const;
    const bool getShowObjectNames() const { return showObjectNames; }
    //==========================================================================
    void setSnappingGrid (const int numPixels, const bool active, const bool shown);

    int getSnappingGridSize() const throw() { return snapGridPixels; }
    bool isSnapActive (const bool disableIfCtrlKeyDown) const throw();
    bool isSnapShown() const throw() { return snapShown; }

    int snapPosition (int pos) const throw();


    void timerCallback();

    void redrawObjects(const int cmdId);

    //==========================================================================
private:

    ContentComp* getContentComp();

    void showContextMenu(const Point<int> mPos);

    void showRedrawOptions();

    void checkExtent(const Rectangle<int>& r);
    ObjController& objController;
    MDLFile* mdlFile;

    bool dragging;
    Point<int> draggingStart;
    Point<int> draggingActual;

    LassoComponent<SelectableObject*> lassoComp;
    ScopedPointer<SnapGridPainter> grid;

    bool isDrawingObjectNames;
    bool showObjectNames;

    int maxX, maxY;

    int snapGridPixels;
    bool snapActive, snapShown;

    ScopedPointer<VariablesPanel> vpanel;
    ScopedPointer<DirectedGraph> graph;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectsHolder);
};
}

#endif  // __OBJCOMP_H_F3604232__
