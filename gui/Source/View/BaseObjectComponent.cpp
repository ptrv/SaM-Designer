/*
  ==============================================================================

    BaseObjectComponent.cpp
    Created: 23 Aug 2012 2:52:50pm
    Author:  Peter vasil

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

#include "BaseObjectComponent.h"

#include "Application/CommonHeaders.h"

#include "ObjectsHolder.h"
#include "IdLabel.h"


using namespace synthamodeler;

BaseObjectComponent::BaseObjectComponent(ObjController& owner_, ValueTree data_)
    : Node(data_), owner(owner_), data(data_), isIdLabelVisible(false)
{
    setComponentID("boc_" + data[Ids::identifier].toString());
}

BaseObjectComponent::~BaseObjectComponent()
{
    masterReference.clear();
}

Point<int> BaseObjectComponent::getCenter() const
{
    return getBounds().getCentre();
}

void BaseObjectComponent::setIdLabelVisible(const bool visible)
{
    isIdLabelVisible = visible;

    if (isIdLabelVisible)
    {
        idLabel = createIdLabel();
    }
    else
    {
        idLabel = nullptr;
    }
}

IdLabelComp& BaseObjectComponent::getIdLabelComp()
{
    if (idLabelComp == nullptr)
    {
        idLabelComp = new IdLabelComp(getData()[Ids::identifier].toString());
    }
    return *idLabelComp;
}

ObjectsHolder* BaseObjectComponent::getObjectsHolder() const noexcept
{
    return findParentComponentOfClass<ObjectsHolder>();
}

void BaseObjectComponent::showContextMenu()
{
    PopupMenu m;
    m.addItem(1, TRANS("Edit"));
    m.addItem(2, TRANS("Delete"));
    m.addSeparator();
    m.addItem(3, TRANS("Help"));

    const int r = m.show();

    if (r == 1)
    {
        propertiesWindow->makeVisible(true);
    }
    else if (r == 2)
    {
        getObjectsHolder()->deleteSelectedObjects();
    }
    else if (r == 3)
    {
        Utils::openHelpPatch(data.getType().toString());
    }
}
