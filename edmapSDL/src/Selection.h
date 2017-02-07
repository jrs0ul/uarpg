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

#ifndef _SELECTION_H
#define _SELECTION_H

#include "../../src/DArray.h"
#include "../../src/Worldmap.h"

class Selection{

    DArray<unsigned> items;
public:
    bool            allSelected;

                    Selection(){
                        allSelected = false;
                    }

    void            alignSelectedToFirstX(Map& m);
    void            alignSelectedToFirstY(Map& m);
    void            alignSelectedToFirstZ(Map& m);
    void            setSelectedScale(Vector3D scale, Map& m);
    void            setSelectedRotation(Vector3D rotation, Map& m);
    void            moveSelected(Vector3D shift, Map& m);
    void            scaleSelected(Vector3D scale, Map& m);
    void            rotateSelected(Vector3D rotation, Map& m);
    void            duplicateSelected(Map& map);
    void            deleteSelected(Map& m);
    void            selectAll(Map & m);
    void            add(unsigned i){items.add(i);}
    const unsigned *getArray(){return (const unsigned *)items.getData();}
    unsigned long   count(){return items.count();}
    void            destroy(){items.destroy(); allSelected = false;}
    int             isSelected(unsigned long assetIndex);
    void            remove(unsigned long index){items.remove(index);}

};


#endif //_SELECTION_H
