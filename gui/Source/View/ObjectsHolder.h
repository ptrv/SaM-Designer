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
class FaustcodePanel;
class RedrawOptionsPanel;
class DirectedGraph;
class ContentComp;
/**
 * Component which contains all objects.
 */
class ObjectsHolder : public Component,
                      public LassoSource <SelectableObject*>,
                      public ChangeListener,
                      public Timer,
                      public ApplicationCommandTarget
{
public:
    ObjectsHolder(ObjController& objController_);
    ~ObjectsHolder();

    //==========================================================================
    void paint(Graphics& g);
    void resized();

    void changeListenerCallback(ChangeBroadcaster*);

    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);

    bool keyPressed(const KeyPress& key);

    //==========================================================================
    ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands(Array<CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	bool perform(const InvocationInfo& info);
    //==========================================================================

    void setMDLFile(MDLFile* newMDLFile);
    void reloadMDLFile();

    void findLassoItemsInArea(Array <SelectableObject*>& results,
                              const Rectangle<int>& area);

    SelectedItemSet <SelectableObject*>& getLassoSelection();
    //==========================================================================
    void deselectAllSelectedObjects();
    ObjectComponent* getSelectedObject(int index);
    void updateSelectedObjects();

    ObjController& getObjController() const { return objController; }

//    const Rectangle<int> getComponentArea() const;

    void deleteSelectedObjects();

    const Rectangle<int> getObjectsExtent() const;
    const Rectangle<int> getObjectsBounds() const;
    bool getShowObjectNames() const { return showObjectNames; }
    //==========================================================================
    void setSnappingGrid (const int numPixels, const bool active, const bool shown);

    int getSnappingGridSize() const throw() { return snapGridPixels; }
    bool isSnapActive (const bool disableIfCtrlKeyDown) const throw();
    bool isSnapShown() const throw() { return snapShown; }

    int snapPosition (int pos) const throw();


    void timerCallback();

    void redrawObjects(const int cmdId);

    static int objectsHolderNum;
    //==========================================================================
private:

    ContentComp* getContentComp();

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

    ScopedPointer<FaustcodePanel> fcPanel;
    ScopedPointer<DirectedGraph> graph;
//==============================================================================
    void openFaustcodePanel();
    void setSegmentedLinks();
    void insertNewObject(const Identifier& objType);
    void insertNewObject(const Identifier& objType, const Point<int>& point);
    void insertNewLink(const Identifier& linkType);
    void insertNewLink(const Identifier& linkType,
                       const String& startId,
                       const String& endId);
    void insertNewAudioConnection();

    Point<int> getMouseXYRelativeViewport();
    bool getStartEndObjects(String& startId, String& endId);
    bool getStartEndObjectsLeftRight(String& startId, String& endId);
    void showObjectIds();
    void showAudioConnections();

    WeakReference<ObjectsHolder>::Master masterReference;
    friend class WeakReference<ObjectsHolder>;

    friend class ContextMenus;
//==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectsHolder);
};
}

#endif  // __OBJCOMP_H_F3604232__
