/*
  ==============================================================================

    RedrawOptionsPanel.cpp
    Created: 18 Dec 2012 3:55:46pm
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"

#include "RedrawOptionsPanel.h"

using namespace synthamodeler;
//==============================================================================
class RedrawOptionsComponent : public Component,
                               public Button::Listener
{
public:
    RedrawOptionsComponent(RedrawOptionsPanel* rop_)
    : rop(rop_), btOk("Ok"), btCancel("Cancel"),
        lAlpha("labelAlpha", "alpha"), slAlpha("slAlpha"),
        lBeta("labelBeta", "beta"), slBeta("slBeta"),
        lK("labelK", "k"), slK("slK"),
        lDamp("labelDamp", "damp"), slDamp("slDamp"),
        lEnergy("labelEnergy", "energy"), slEnergy("slEnergy"),
        lTimeStep("labelTeimStep", "timestep"), slTimeStep("slTimeStep"),
        pf(StoredSettings::getInstance()->getProps())
    {
        
		btOk.addListener(this);
		addAndMakeVisible(&btOk);
		btCancel.addListener(this);
        addAndMakeVisible(&btCancel);

        addAndMakeVisible(&lAlpha);
        addAndMakeVisible(&slAlpha);
        addAndMakeVisible(&lBeta);
        addAndMakeVisible(&slBeta);
        addAndMakeVisible(&lK);
        addAndMakeVisible(&slK);
        addAndMakeVisible(&lDamp);
        addAndMakeVisible(&slDamp);
        addAndMakeVisible(&lEnergy);
        addAndMakeVisible(&slEnergy);
        addAndMakeVisible(&lTimeStep);
        addAndMakeVisible(&slTimeStep);

        slAlpha.setRange(0.1, 10, 0.1);
        slAlpha.setPopupMenuEnabled(true);
        slAlpha.setValue(pf.getDoubleValue("redrawparam_alpha", 1.0));
        slAlpha.setSliderStyle (Slider::LinearHorizontal);
        slAlpha.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);

        slBeta.setRange(0.001, 2, 0.001);
        slBeta.setPopupMenuEnabled(true);
        slBeta.setValue(pf.getDoubleValue("redrawparam_beta", 1.0));
        slBeta.setSliderStyle (Slider::LinearHorizontal);
        slBeta.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);

        slK.setRange(0.01, 5, 0.01);
        slK.setPopupMenuEnabled(true);
        slK.setValue(pf.getDoubleValue("redrawparam_k", 0.1));
        slK.setSliderStyle (Slider::LinearHorizontal);
        slK.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);

        slDamp.setRange(0.1, 0.99, 0.01);
        slDamp.setPopupMenuEnabled(true);
        slDamp.setValue(pf.getDoubleValue("redrawparam_damp", 0.75));
        slDamp.setSliderStyle (Slider::LinearHorizontal);
        slDamp.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);

        slEnergy.setRange(0.0, 10, 0.1);
        slEnergy.setPopupMenuEnabled(true);
        slEnergy.setValue(pf.getDoubleValue("redrawparam_energy", 0.5));
        slEnergy.setSliderStyle (Slider::LinearHorizontal);
        slEnergy.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);

        slTimeStep.setRange(0.1, 1, 0.1);
        slTimeStep.setPopupMenuEnabled(true);
        slTimeStep.setValue(pf.getDoubleValue("redrawparam_timestep", 0.5));
        slTimeStep.setSliderStyle (Slider::LinearHorizontal);
        slTimeStep.setTextBoxStyle(Slider::TextBoxLeft, false, 80, 20);


    }
    virtual ~RedrawOptionsComponent()
    {
    }

    void resized()
    {
        btOk.setBounds(getWidth() / 2 - 65, getHeight() - 30, 60, 22);
        btCancel.setBounds(getWidth() / 2 + 5, getHeight() - 30, 60, 22);

        lAlpha.setBounds(10, 10, 50, 20);
        lBeta.setBounds(10, 50, 50, 20);
        lK.setBounds(10, 90, 50, 20);
        lDamp.setBounds(10, 130, 50, 20);
        lEnergy.setBounds(10, 180, 50, 20);
        lTimeStep.setBounds(10, 230, 50, 20);

        slAlpha.setBounds(70, 10, 250, 20);
        slBeta.setBounds(70, 50, 250, 20);
        slK.setBounds(70, 90, 250, 20);
        slDamp.setBounds(70, 130, 250, 20);
        slEnergy.setBounds(70, 180, 250, 20);
        slTimeStep.setBounds(70, 230, 250, 20);

    }

    void buttonClicked (Button* button)
    {
		if(button == &btOk)
		{
            saveSettings();
            rop->closeButtonPressed();
		}
		else if( button == &btCancel)
		{
            rop->closeButtonPressed();
		}
    }
private:
    void saveSettings()
    {
        pf.setValue("redrawparam_alpha", slAlpha.getValue());
        pf.setValue("redrawparam_beta", slBeta.getValue());
        pf.setValue("redrawparam_k", slK.getValue());
        pf.setValue("redrawparam_damp", slDamp.getValue());
        pf.setValue("redrawparam_energy", slEnergy.getValue());
        pf.setValue("redrawparam_timestep", slTimeStep.getValue());
    }
    
    RedrawOptionsPanel* rop;
    TextButton btOk;
	TextButton btCancel;
    Label lAlpha;
    Slider slAlpha;
    Label lBeta;
    Slider slBeta;
    Label lK;
    Slider slK;
    Label lDamp;
    Slider slDamp;
    Label lEnergy;
    Slider slEnergy;
    Label lTimeStep;
    Slider slTimeStep;
    PropertiesFile& pf;
};

static String variablesWindowPos;
//==============================================================================
RedrawOptionsPanel::RedrawOptionsPanel()
: DialogWindow("Redraw options", Colours::grey, true)
{
    RedrawOptionsComponent * const roc = new RedrawOptionsComponent(this);
    roc->setSize (350, 300);

    setContentOwned (roc, true);

    if (! restoreWindowStateFromString (variablesWindowPos))
        centreWithSize(getWidth(), getHeight());

    setResizable (false, false);
    setVisible(true);

}

RedrawOptionsPanel::~RedrawOptionsPanel()
{
}

void RedrawOptionsPanel::closeButtonPressed()
{
    setVisible (false);
}

void RedrawOptionsPanel::show()
{
    RedrawOptionsPanel rop;
    rop.runModalLoop();
}