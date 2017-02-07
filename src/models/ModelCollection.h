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
#ifndef _MODELCOLLECTION_H
#define _MODELCOLLECTION_H

#include "RatModel.h"
#include "../Matrix.h"



class AnimationSet{
public:
    DArray<RatModel> frames;

};

class ModelCollection{


        DArray<AnimationSet> subsets;
        DArray<FlatMatrix> matrices;

        unsigned _frameCount;


    public:
        void         destroy();
        bool         load(const char* path, bool useVBO = true);
        bool         drawsubset(unsigned int index = 0, unsigned int frame = 0, bool useVBO = true);
        bool         drawSubsetOutline(unsigned int index = 0, unsigned int frame = 0, float width = 2.5f);
        RatModel*    getSubset(unsigned index, unsigned frame = 0);
        void         getMatrix(unsigned int index, FlatMatrix& mat);
        unsigned int count(){return subsets.count();}
        void         transform(int index){glMultMatrixf(matrices[index].m);}
        int          frameCount(){return _frameCount;}

};


#endif //_MODELCOLLECTION_H
