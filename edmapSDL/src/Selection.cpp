/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
 *   jrs0ul@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "Selection.h"

void Selection::selectAll(Map & m){
    for (unsigned i = 0; i < m.assetCount(); i++){
        items.add(i);
    }
    allSelected = true;
}
//--------------------------------------
void Selection::deleteSelected(Map& m){


    for (unsigned i = 0; i < items.count(); i++){
        
        m.removeAsset(items[i]);
        for (unsigned a = i; a < items.count(); a++){
            if ((items[a] > 0) && (items[a] > items[i]))
                items[a] -= 1;
        }
    }
    items.destroy();

}
//-----------------------------------------
void Selection::duplicateSelected(Map& map){
    for (unsigned i = 0; i < items.count(); i++){
        Asset a = *(map.getAsset(items[i]));
        map.addAsset(a);
    }
}
//-----------------------------------------
void Selection::moveSelected(Vector3D shift, Map& m){

    for (unsigned i = 0; i < items.count(); i++){
        m.moveAsset(items[i], shift);
    }

}
//----------------------------------------
int Selection::isSelected(unsigned long assetIndex){
    for (unsigned i = 0; i < items.count(); i++)
        if (items[i] == assetIndex)
            return i;

    return -1;
}
//---------------------------------------
void Selection::scaleSelected(Vector3D scale, Map& m){

   // printf("%f %f %f\n", scale.v[0], scale.v[1], scale.v[2]);
    for (unsigned i = 0; i < items.count(); i++){
        m.scaleAsset(items[i], scale);
    }

}
//---------------------------------------
void Selection::rotateSelected(Vector3D rotation, Map& m){
    for (unsigned i = 0; i < items.count(); i++){
        m.rotateAsset(items[i], rotation);
    }
}
//--------------------------------------
void Selection::setSelectedScale(Vector3D scale, Map& m){

    for (unsigned i = 0; i < items.count(); i++){
        m.setAssetScale(items[i], scale);
    }
}
//--------------------------------------
void Selection::setSelectedRotation(Vector3D rotation, Map& m){

    for (unsigned i = 0; i < items.count(); i++){
        m.setAssetRotation(items[i], rotation);
    }
}
//--------------------------------------
void Selection::alignSelectedToFirstX(Map& m){

    if (!items.count())
        return;
    Asset* a = m.getAsset(items[0]);
    for (unsigned i = 1; i < items.count(); i++){
        Asset* b = m.getAsset(items[i]);

        m.setAssetPosition(items[i], Vector3D(a->position.v[0],
                                              b->position.v[1],
                                              b->position.v[2]));
                                              
    }
}
//--------------------------------------
void Selection::alignSelectedToFirstY(Map& m){
    if (!items.count())
        return;
    Asset* a = m.getAsset(items[0]);
    for (unsigned i = 0; i < items.count(); i++){
        Asset* b = m.getAsset(items[i]);

        m.setAssetPosition(items[i], Vector3D(b->position.v[0],
                                              a->position.v[1],
                                              b->position.v[2]));
    }
}
//--------------------------------------
void Selection::alignSelectedToFirstZ(Map& m){
    if (!items.count())
        return;
    Asset* a = m.getAsset(items[0]);
    for (unsigned i = 0; i < items.count(); i++){
        Asset* b = m.getAsset(items[i]);

        m.setAssetPosition(items[i], Vector3D(b->position.v[0],
                                              b->position.v[1],
                                              a->position.v[2]));
    }
}
