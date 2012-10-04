/*
  ==============================================================================

    SnapGridPainter.h
    Created: 4 Oct 2012 2:48:21pm
    Author:  peter

  ==============================================================================
*/

#ifndef __SNAPGRIDPAINTER_H_EFB1F3E4__
#define __SNAPGRIDPAINTER_H_EFB1F3E4__


class ObjectsHolder;
class SnapGridPainter
{
public:
    //==============================================================================
    SnapGridPainter();
    ~SnapGridPainter();

    //==============================================================================
    void draw (Graphics& g);

    void updateColour();
    bool updateFromDesign (ObjectsHolder& design);

private:
    int snapGridSize;
    bool snapShown;
    Image backgroundFill;
};


#endif  // __SNAPGRIDPAINTER_H_EFB1F3E4__
