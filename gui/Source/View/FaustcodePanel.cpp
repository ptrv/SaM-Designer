/*
  ==============================================================================

    FaustcodePanel.cpp
    Created: 15 May 2012 7:50:53pm
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

#include "../Application/CommonHeaders.h"
#include "../Controller/ObjController.h"
#include "../Utilities/IdManager.h"

#include "FaustcodePanel.h"

using namespace synthamodeler;

class FaustCodeEditor : public TextEditor
{
public:
    FaustCodeEditor(const String& componentName_,
                    ObjController* objController_)
        : TextEditor(componentName_), objController(objController_)
    {
    }

    void mouseDoubleClick(const MouseEvent& e)
    {
        TextEditor::mouseDoubleClick(e);
        if(! getHighlightedRegionIfVar().isEmpty())
            objController->selectObjectsIfContainText(getHighlightedText());
        else
            objController->selectAll(false);
    }
    bool keyPressed(const KeyPress& kp)
    {
        TextEditor::keyPressed(kp);
        if(! getHighlightedRegionIfVar().isEmpty())
        {
            objController->selectObjectsIfContainText(getHighlightedText());
        }
        else
        {
            objController->selectAll(false);
        }
        return true;
    }
    void mouseUp (const MouseEvent & md)
    {
        TextEditor::mouseUp(md);
        if(! getHighlightedRegionIfVar().isEmpty())
        {
            objController->selectObjectsIfContainText(getHighlightedText());
        }
        else
        {
            objController->selectAll(false);
        }
    }
    Range<int>getHighlightedRegionIfVar()
    {
        Range<int> range = TextEditor::getHighlightedRegion();

        // Return only a range if selection has an equal sign on the right
        // It make only sense to return range if it is a variable name and not
        // the variable definition
        if(! range.isEmpty())
        {
            String lineTxt = getText().substring(range.getEnd())
                .upToFirstOccurrenceOf("\n", false, true);
            if(! lineTxt.contains("="))
                return Range<int>();
        }
        return range;
    }
private:
    ObjController* objController;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaustCodeEditor)
};
class FaustcodeComponent : public Component,
                           public Button::Listener,
                           public TextEditor::Listener
{
public:

    FaustcodeComponent(FaustcodePanel* fcp_,
                       ObjController* objController_,
                       ValueTree data_,
                       UndoManager* undoManager_)
    : parent(fcp_),
    data(data_),
    undoManager(undoManager_)
    {
        addAndMakeVisible(btOk = new TextButton("Ok"));
        btOk->addListener(this);

        addAndMakeVisible(btCancel = new TextButton(TRANS("Cancel")));
        btCancel->addListener(this);

        addAndMakeVisible(btHelp = new TextButton("?"));
        btHelp->addListener(this);

        addAndMakeVisible(teFaustCode = new FaustCodeEditor("faustcode texteditor",
                                                            objController_));
        teFaustCode->setMultiLine(true, false);
        teFaustCode->setReadOnly(false);
        teFaustCode->addListener(this);
        teFaustCode->setReturnKeyStartsNewLine(true);

        readFaustcodeValues();
    }

    ~FaustcodeComponent()
    {
        btOk = nullptr;
        btCancel = nullptr;
        btHelp = nullptr;
        teFaustCode = nullptr;
    }

    void resized()
    {
        teFaustCode->setBounds(5, 5, getWidth() - 10, getHeight() - 40);
        btOk->setBounds(getWidth()/2 - 55, getHeight() - 30, 50, 22);
        btCancel->setBounds(getWidth()/2 + 5, getHeight() - 30, 50, 22);
        btHelp->setBounds(getWidth() - 30, getHeight() - 30, 22, 22);
    }

    void buttonClicked(Button* button)
    {
        if (button == btOk)
        {
            writeFaustcodeValues();
            parent->closeButtonPressed();
        }
        else if (button == btCancel)
        {
            parent->closeButtonPressed();
        }
        else if (button == btHelp)
        {
            HintComponent* content = new HintComponent();
            CallOutBox::launchAsynchronously(content,
                                             button->getScreenBounds(),
                                             nullptr);
        }
    }

    void readFaustcodeValues()
    {
        StringArray faustCodeLines;
        ValueTree fcTree = data.getChildWithName(Objects::faustcodeblock);
        for (int i = 0; i < fcTree.getNumChildren(); ++i)
        {
            String text = fcTree.getChild(i)[Ids::value].toString();
            faustCodeLines.add(text);
        }
        faustCodeText = faustCodeLines.joinIntoString(newLine);
        teFaustCode->setText(faustCodeText, false);
    }

    void writeFaustcodeValues()
    {
        if(faustCodeText.compare(teFaustCode->getText()) != 0)
        {
            DBG("faustcode modified");
            StringArray faustCodeLines;
            faustCodeLines.addTokens(teFaustCode->getText(), "\n\r", "\"");

            undoManager->beginNewTransaction("Modify faustcode");
            ValueTree fcTree = data.getOrCreateChildWithName(Objects::faustcodeblock,
                                                             undoManager);

            fcTree.removeAllChildren(undoManager);
            for (int i = 0; i < faustCodeLines.size(); ++i)
            {
                if(faustCodeLines[i].isEmpty())
                    continue;
                ValueTree newData(Ids::faustcode);
                newData.setProperty(Ids::value, faustCodeLines[i], nullptr);

                fcTree.addChild(newData, -1, undoManager);
            }
        }
    }

    bool keyPressed(const KeyPress& key)
    {
        if(key == KeyPress('s', ModifierKeys::commandModifier, 0))
        {
            writeFaustcodeValues();
            return true;
        }
        return false;
    }

    void textEditorTextChanged(TextEditor&)
    {
    }

    void textEditorReturnKeyPressed(TextEditor&)
    {
    }

    void textEditorEscapeKeyPressed(TextEditor&)
    {
        parent->closeButtonPressed();
    }

    void textEditorFocusLost(TextEditor&)
    {
    }


private:
    FaustcodePanel* parent;
    ScopedPointer<TextButton> btOk;
    ScopedPointer<TextButton> btCancel;
    ScopedPointer<TextButton> btHelp;
    ScopedPointer<FaustCodeEditor> teFaustCode;
	ValueTree data;
    UndoManager* undoManager;
    String faustCodeText;


    class HintComponent : public Component
    {
    public:

        HintComponent()
        : exampleText("varname=hslider(\"DescriptiveLabelForVariable\","\
                      "defaultValue,minimumValue,maximumValue,stepSize);"),
            font(14.0f)
        {
            textW = font.getStringWidth(exampleText);
            setSize(textW,font.getHeight()*3);
        }

        ~HintComponent()
        {
        }

        void paint(Graphics& g)
        {
//            g.fillAll(Colours::white);
            g.setColour(Colours::white);
            g.setFont(font);
            g.drawText("Example:", 0, 0, 
                       getWidth(), font.getHeight(),
                       Justification::left, false);
            g.drawMultiLineText(exampleText, 0, font.getHeight()*2, textW);
        }
    private:
        String exampleText;
        Font font;
        int textW;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaustcodeComponent)
};


static String faustcodeWindowPos;


FaustcodePanel::FaustcodePanel(ObjController* objController,
                               ValueTree data,
                               UndoManager* undoManager)
: DialogWindow(TRANS("Define FAUST code for") + " " + data[Ids::mdlName].toString(),
                    Colour::greyLevel (0.92f), true)
{
    FaustcodeComponent * const fc = new FaustcodeComponent(this,
                                                           objController,
                                                           data,
                                                           undoManager);
    fc->setSize (600, 400);

    setContentOwned (fc, true);

    if (! restoreWindowStateFromString (faustcodeWindowPos))
        centreWithSize(getWidth(), getHeight());

    setResizable (true, true);
    setVisible(true);
}

FaustcodePanel::~FaustcodePanel()
{
}

void FaustcodePanel::closeButtonPressed()
{
    setVisible (false);
}

void FaustcodePanel::show()
{
    setVisible(true);
    toFront(true);
    if(FaustcodeComponent* const fc = dynamic_cast<FaustcodeComponent*>(getContentComponent()))
    {
        fc->readFaustcodeValues();
    }
}