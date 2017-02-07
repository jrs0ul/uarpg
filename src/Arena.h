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
#ifndef ARENA_FILE_H
#define ARENA_FILE_H


    #include "Vectors.h"
    #include "DArray.h"
    #include "models/rm2.h"
    #include "TextureLoader.h"
    #include "Xml.h"



    struct CameraState{
        Vector3D pos;
        Vector3D dir;
        Vector3D up;
        Vector3D right;
    };

    struct ArenaSubsetAttrib{
        bool alphaTest;
        bool drawBack;
        COLOR diffuse;
        COLOR ambient;
    };

    //--------------------------

    class CArena{


        SubsetCollection mesh;
        PicsContainer _textures;

        COLOR ambient;
        COLOR diffuse;
        Vector3D lightPos;

        Vector3D lPos;
        Vector3D lTo;

        char meshName[255];

        DArray<CameraState> introPath;
        DArray<CameraState> randomPositions;
        DArray<CameraState> outroPath;
        DArray<unsigned long> texIndexes;
        DArray<ArenaSubsetAttrib> subAttribs;


        bool readCamState(XmlNode* node, CameraState& cs);
        bool readColor(XmlNode* node, COLOR& c);



    public:

        bool load(const char * path, const char * basepath);

        void setLightning();

        unsigned camPostionCount(){return randomPositions.count();}
        unsigned introPathCount() {return introPath.count();}
        unsigned outroPathCount() {return outroPath.count();}
        CameraState * getPosition(unsigned long i);
        CameraState * getIPathNode(unsigned long i);
        CameraState * getOPathNode(unsigned long i);
        Vector3D getLightPos(){return lightPos;}

        Vector3D getLightOrigin(){return lPos;}
        Vector3D getLightDest(){return lTo;}

        void draw(ShaderProgram * meshs,  
                  bool drawOutline = true, 
                  bool drawblack = false //for shadows
                 );

        void destroy();

        CArena(){
            ambient = COLOR(0, 0, 0, 0);
            diffuse = COLOR(0, 0, 0, 0);
            lightPos = Vector3D(0, 0, 0);
            lPos = Vector3D(0, 0, 0);
            lTo = Vector3D(0, 0, 0);

        }




    };



#endif //ARENA_FILE_H

