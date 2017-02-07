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

#ifndef _RATMODELV2_H
#define _RATMODELV2_H

#include "../DArray.h"
#include "../Vectors.h"
#include "../Matrix.h"
#include "../VertexObject.h"
#include "../Colors.h"



//--------------------
struct PoseBone{
    Matrix mat;
};
//------------------

struct PoseBoneFrames{

    DArray<PoseBone> frames;

    void destroy(){frames.destroy();}
};
//---------------------
struct AssignedBoneToVertice{
    unsigned boneID;
    float weight;
};

//-------------------------
struct AssignedBones{
    DArray<AssignedBoneToVertice> bones;

    void destroy(){bones.destroy();}
    bool load(FILE *f);
    bool save(FILE * f);
};

//----------------------------
struct TransformedVertex{
    float vert[3];
    float norm[3];
    int frame;

    TransformedVertex(){
        vert[0] = 0; vert[1] = 0; vert[2] = 0;
        norm[0] = 0; norm[1] = 0; norm[2] = 0;
    }
};

//=========================================
//Model data, vertices, normals etc.
class Model{
    unsigned long _facecount;
public:
    float * verts;
    float * normals;
    unsigned * indices;
    unsigned long _vertexcount;


    VertexObject geometry;
    VertexObject uvs;


    bool isUVS;

    Model(){
        verts = 0;
        normals = 0;
        indices = 0;
        _vertexcount = 0;
        _facecount = 0;
        isUVS = false;
    }

    unsigned long faceCount(){return _facecount;}
    void setFaceCount(unsigned value){_facecount = value;}
    bool load(FILE* f, bool useVBO = true);
    bool save(FILE* f);
    //create mesh without indices. Each uvs represents a separate vertex
    void createUnindexedMesh(bool createVBO = true, bool useDynamic = true);
    void destroy();
    unsigned indice(unsigned long index);
    float vertexcoord(unsigned long index);
};

//=========================================
//Single mesh in model file
class Subset{
    unsigned long _framecount;
    unsigned long _bonecount;
public:
    //anim[bone].frames[frame].mat <- pose matrix
    DArray<PoseBoneFrames> anim;
    Model mesh; //model data
    //bones assigned to each vertice, so
    //count of this DArray should be equal vertex count
    DArray<AssignedBones> vertsbones; 
    //already transformed vertices, for speedup
    DArray<TransformedVertex> tvertices;

         Subset(){ _framecount = 0; _bonecount = 0;}
    bool load(FILE * f, bool createVBO = true);
    bool save(FILE * f);
    void draw(unsigned long frame);
    //calculates vertex and normal for given animation frame
    void calcVertnNormal(
                         unsigned frame,
                         unsigned indice,
                         Vector3D & v, 
                         Vector3D & n );
    unsigned long framecount(){return _framecount;}
    void          setFramecount(unsigned long fc){_framecount = fc;}
    unsigned long bonecount(){return _bonecount;}
    void          setBonecount(unsigned long bc){_bonecount = bc;}
    void destroy(); 
};

//=========================================
//Collection of meshes
class SubsetCollection{
        DArray<Subset> subsets;
    public:
        bool load(const char* filename, bool createVBO = true);
        bool save(const char* filename);
        bool draw(unsigned subset = 0, unsigned frame = 0);
        bool drawOutline(unsigned subset = 0,
                         unsigned frame = 0,
                         float width = 2.0f,
                         COLOR color = COLOR(0,0,0));
        void drawall(unsigned frame = 0);
        void destroy();
        void dump(const char * path);
        unsigned long count(){return subsets.count();}
        unsigned long framecount (unsigned long subset);
        unsigned long facecount(unsigned long subset);
        
        Subset* getSubset(unsigned long index);
        void    addSubset(Subset& s, bool useVBO = true);

};

#endif //RATMODELV2_H
