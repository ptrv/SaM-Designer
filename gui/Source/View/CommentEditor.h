/*
  ==============================================================================

    CommentEditor.h
    Created: 22 Oct 2012 4:40:15pm
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

#ifndef __COMMENTEDITOR_H_D81136D5__
#define __COMMENTEDITOR_H_D81136D5__


namespace synthamodeler
{
class CommentComponent;
class CommentEditor : public TextEditor
{
public:
    CommentEditor(CommentComponent& p, float fontHeight) : TextEditor("CommentEditor"), parent(p), fHeight(fontHeight)
    {
//        MemoryInputStream mis(BinaryData::UbuntuR_ttf, BinaryData::UbuntuR_ttfSize, false);
//        CustomTypeface* ctf = new CustomTypeface(mis);
//        ctf->setCharacteristics("Ubuntu", 0.7f, false, false, 'a');
//        font = Font(ctf);
        font.setHeight(fHeight);
        applyFontToAllText(font);
//        setFont(font);
        setTextToShowWhenEmpty("(comment)", Colours::grey);
        setColour(TextEditor::backgroundColourId, Colour(0x00000000));
        setColour(TextEditor::shadowColourId, Colour(0x00000000));

        BorderSize<int> b(0, 0, 0, 0);
        setBorder(b);

        setScrollToShowCursor(false);
        
        setMultiLine(true, false);
        setReturnKeyStartsNewLine(true);
    }
    virtual ~CommentEditor(){}

    void mouseDown(const MouseEvent& e)
    {
        parent.mouseDown(e);
        TextEditor::mouseDown(e);
    }
    void mouseDrag(const MouseEvent& e)
    {
        parent.mouseDrag(e);
        TextEditor::mouseDrag(e);
    }
    void mouseUp(const MouseEvent& e)
    {
        parent.mouseUp(e);
        TextEditor::mouseUp(e);
    }

    void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel)
    {
        if(e.mods.isAltDown() && hasKeyboardFocus(true))
        {
            float fontH = font.getHeight();
            if(wheel.deltaY < 0.0f)
            {
                fHeight = fontH - 0.5f;
            }
            else if(wheel.deltaY > 0.0f)
            {
                fHeight = fontH + 0.5f;
            }
            font.setHeight(fHeight);
            applyFontToAllText(font);
            parent.resized();
        }
        else
        {
            TextEditor::mouseWheelMove(e, wheel);
        }
    }

    void setFontHeight(float newHeight)
    {
        fHeight = newHeight;
        font.setHeight(fHeight);
    }
    void applyFont()
    {
        applyFontToAllText(font);
    }
private:
    CommentComponent& parent;
    Font font;
    float fHeight;
};
}


#endif  // __COMMENTEDITOR_H_D81136D5__
