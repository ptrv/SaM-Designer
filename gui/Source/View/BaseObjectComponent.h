/*
  ==============================================================================

    BaseObjectComponent.h
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

#ifndef __BASEOBJECTCOMPONENT_H_ADAD1FA0__
#define __BASEOBJECTCOMPONENT_H_ADAD1FA0__

namespace synthamodeler
{
class ObjController;
class ObjectsHolder;
class Node;

class BaseObjectComponent : public Component,
                            public Node
{
public:
    BaseObjectComponent(ObjController& _owner, ValueTree data_);
    virtual ~BaseObjectComponent();
    
    ValueTree getData();

protected:
    ObjectsHolder* getObjectsHolder() const noexcept;
    void showContextMenu();


    ObjController& owner;
    ValueTree data;

private:
    WeakReference<BaseObjectComponent>::Master masterReference;
    friend class WeakReference<BaseObjectComponent>;
};
};


#endif  // __BASEOBJECTCOMPONENT_H_ADAD1FA0__
