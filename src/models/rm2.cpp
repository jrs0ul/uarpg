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



#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL.h>
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL.h>

        #include <SDL/SDL_opengl.h>
    #endif
#else
    #include <SDL/SDL.h>

    #include <SDL/SDL_opengl.h>
#endif


#include <cstdio>
#include "../VertexObject.h"
#include "rm2.h"
#include "../Extensions.h"

//-----------------------------------------------
bool Model::load(FILE * f, bool useVBO){

    if (!fread(&_vertexcount, sizeof(int), 1, f))
        return false;

    if (_vertexcount){

        //printf("%lu vertices\n", _vertexcount);
        verts = (float*)malloc((_vertexcount * 3 + 1) * sizeof(float));
        if (!fread(verts, 4, _vertexcount*3, f))
            return false;
        normals = (float*)malloc((_vertexcount * 3 + 1) * sizeof(float));
        if (!fread(normals, 4, _vertexcount * 3, f))
            return false;
    }

    if (!fread(&_facecount, 4, 1, f))
        return false;
       //printf("%lu faces\n", _facecount);
    if (_facecount){
        indices = (unsigned *)malloc((_facecount * 3 + 1) * sizeof(int));
        if (!fread(indices, sizeof(int), _facecount*3, f))
            return false;
        
        int hasUVS = 0;
        if (!fread(&hasUVS, sizeof(int), 1, f))
            hasUVS = 0;
        isUVS = (hasUVS == 1)? true: false;
        //printf("hasUVS %d\n", hasUVS);

        if (isUVS){

            
            // puts("reading uvs");
            float * _uvs = (float*)malloc(_facecount*6*sizeof(float));
            if (!fread(_uvs, sizeof(float), _facecount*6, f))
                return false;
            uvs.init(_uvs, VTEXCOORD0, _facecount*3, false, useVBO);
            free(_uvs);

        }
    }
    return true;
}
//-------------------------------------------------------------
bool Model::save(FILE * f){

    if (!f)
        return false;
    if (!fwrite(&_vertexcount, sizeof(int), 1, f))
        return false;
    if (_vertexcount){
        if (!fwrite(verts, 4, _vertexcount * 3, f))
            return false;
        if (!fwrite(normals, 4, _vertexcount * 3, f))
            return false;
    }
    if (!fwrite(&_facecount, 4, 1, f))
        return false;
    if (_facecount){
        if (!fwrite(indices, sizeof(int), _facecount*3, f))
            return false;

        int hasUVS = 0;
        hasUVS = isUVS ? 1 : 0;
        if (!fwrite(&hasUVS, sizeof(int), 1, f))
            return false;
        if (isUVS){

            char * data = uvs.getRawData();
            if (!fwrite(data, sizeof(float), _facecount * 6, f))
                return false;

        }
    }

    return true;
}
//--------------------------------------------------------------
unsigned Model::indice(unsigned long index){
   if (index < _facecount * 3)
        return indices[index];

    return 0;

}
//--------------------------------------------------------------
float Model::vertexcoord(unsigned long index){
    if (index < _vertexcount * 3)
        return verts[index];

    return 0.0f;
}

//--------------------------------------------------------------
void Model::createUnindexedMesh(bool createVBO, bool useDynamic){

    long size = _facecount * 9 + _facecount * 9;

    float * _data = 0;

    if (_facecount * 9){
        _data = (float*)malloc(size * sizeof(float));
    }
    for (unsigned int i = 0; i < _facecount * 3; i++){

        _data[i * 6]     = verts[indices[i]*3];
        _data[i * 6 + 1] = verts[indices[i]*3 + 1];
        _data[i * 6 + 2] = verts[indices[i]*3 + 2];

            
        _data[i * 6 + 3] = normals[indices[i]*3];
        _data[i * 6 + 4] = normals[indices[i]*3 + 1];
        _data[i * 6 + 5] = normals[indices[i]*3 + 2];

    }
    
    if (_data){
        //puts("blip");
        geometry.init(_data, VPOSITION|VNORMAL,
                      _facecount * 3, true, createVBO );
        free(_data);
        //puts("blop");
    }

}
//----------------------------------------------
void Model::destroy(){

    geometry.destroy();
    uvs.destroy();

    if (normals){
        free(normals); normals = 0;
    }

    if (indices){
        free(indices); indices = 0;
    }

    if (verts){
        free(verts);
        verts = 0;
    }
}
//----------------------------------------
bool AssignedBones::load(FILE* f){

    int inflcount = 0;
    if (!fread(&inflcount, sizeof(int), 1, f))
        return false;

    //printf("assigned bones: %d\n", inflcount);
    float * weights = 0;
    int * indexes = 0;
    if (inflcount){
        weights = (float*)malloc((inflcount + 2) * sizeof(float));
        indexes = (int*)malloc((inflcount + 2) * sizeof(int));

        if (!fread(indexes, sizeof(int), inflcount, f))
            return false;
        if (!fread(weights, sizeof(float), inflcount, f))
            return false;

        for (int i = 0; i < inflcount; i++){
            AssignedBoneToVertice abv;

            abv.boneID = indexes[i];
            abv.weight = weights[i];

            bones.add(abv);
        }
        free(indexes); indexes = 0;
        free(weights); weights = 0;
    }

    return true;

}
//----------------------------------------
bool AssignedBones::save(FILE * f){

    int inflcount = bones.count();
    if (!fwrite(&inflcount, sizeof(int), 1, f))
        return false;

    if (inflcount){
        float * weights = 0;
        int * indexes = 0;

        weights = (float*)malloc((inflcount + 2) * sizeof(float));
        indexes = (int*)malloc((inflcount + 2) * sizeof(int));

        for (int i = 0; i < inflcount; i++){

            indexes[i] = bones[i].boneID;
            weights[i] = bones[i].weight;
        }

        if (!fwrite(indexes, sizeof(int), inflcount, f))
            return false;
        if (!fwrite(weights, sizeof(float), inflcount, f))
            return false;

        free(indexes); indexes = 0;
        free(weights); weights = 0;
    }


    return true;
}
//----------------------------------------
void Subset::calcVertnNormal(unsigned frame, unsigned indice, Vector3D & v, Vector3D & n ){

    Vector3D mainnorm = Vector3D(0,0,0);
    Vector3D mainvert = Vector3D(0,0,0);


    for (unsigned long a = 0; a < vertsbones[indice].bones.count(); a++){ //for each bone

        Vector3D vert = Vector3D(mesh.verts[indice * 3],
                                 mesh.verts[indice * 3 + 1],
                                 mesh.verts[indice * 3 + 2]);

        Vector3D norm = Vector3D(mesh.normals[indice * 3],
                                 mesh.normals[indice * 3 + 1],
                                 mesh.normals[indice * 3 + 2]);

        float w = vertsbones[indice].bones[a].weight;
        unsigned index = vertsbones[indice].bones[a].boneID;

        vert.transform(anim[index].frames[frame].mat);

        //apply weight
        vert = Vector3D(vert.x()*w, vert.y()*w, vert.z()*w);
        mainvert = mainvert + vert;
        //--------------

        Matrix nm;
        MatrixCopy(anim[index].frames[frame].mat, nm);
        nm[0][3] = 0; nm[1][3] = 0; nm[2][3] = 0; 
        nm[3][0] = 0; nm[3][1] = 0; nm[3][2] = 0;
        nm[3][3] = 1;
        norm.transform(nm);
        norm = Vector3D(norm.x()*w, norm.y()*w, norm.z()*w);
        mainnorm = mainnorm + norm;
    }
    mainnorm.normalize();

    v = mainvert;
    n = mainnorm;

}

//-------------------------------------------------------------
void Subset::draw(unsigned long frame){

    if (frame >= _framecount)
        frame = _framecount - 1;

    if (_bonecount){

        float * ptr = mesh.geometry.lock();

        for (unsigned int i = 0; i < mesh.faceCount() * 3; i++){
            Vector3D norm = Vector3D(0,0,0);
            Vector3D vert = Vector3D(0,0,0);

            TransformedVertex * tf = &tvertices[mesh.indices[i]];
            if (tf->frame != (int)frame){
                calcVertnNormal(frame, mesh.indices[i], vert, norm);
                tf->frame = (int)frame;
                memcpy(tf->vert, vert.v, 3 * sizeof(float));
                memcpy(tf->norm, norm.v, 3 * sizeof(float));
            } 
            else{
                memcpy(vert.v, tf->vert, 3 * sizeof(float));
                memcpy(norm.v, tf->norm, 3 * sizeof(float));
            }
            memcpy(&ptr[i * 6], vert.v, 3 * sizeof(float));
            memcpy(&ptr[i * 6 + 3], norm.v, 3 * sizeof(float));
        }
        mesh.geometry.unlock();
    }

    mesh.geometry.bind();
    if (mesh.isUVS)
        mesh.uvs.bind();

    glDrawArrays(GL_TRIANGLES, 0, mesh.faceCount() * 3 );

    if (mesh.isUVS)
        mesh.uvs.unbind();
    mesh.geometry.unbind();
}

//----------------------------------------------------
void Subset::destroy(){

    mesh.destroy();
    //assigned bones
    for (unsigned i = 0; i < vertsbones.count(); i++){
        vertsbones[i].destroy();
    }
    vertsbones.destroy();
    //animation
    for (unsigned i = 0; i < anim.count(); i++){
        anim[i].destroy();
    }
    anim.destroy();
    //transformed vertices
    tvertices.destroy();
}
//------------------------------------------------
bool Subset::load(FILE * f, bool createVBO){

    if (!f)
        return false;

    //puts("loading model");
    mesh.load(f, createVBO);
    mesh.createUnindexedMesh(createVBO);
    //puts("loading bones data");
    if (!fread(&_bonecount, sizeof(int), 1, f))
        return false;

   // printf("bonecount: %lu\n", _bonecount);

    if (_bonecount){
        for (unsigned a = 0; a < mesh._vertexcount; a++){

            AssignedBones b;
            if (!b.load(f))
                return false;

            vertsbones.add(b);
            TransformedVertex tv;
            tv.frame = -1;
            tvertices.add(tv);
        }
    }
    //----------------------------------------------

    //puts("loading framecount");
    int framec = 0;
    if (!fread(&framec, sizeof(int), 1, f))
        return false;
    _framecount = (unsigned long)framec;
    //printf("frames: %d\n", framec);

    for (unsigned b = 0; b < _bonecount; b++){
        PoseBoneFrames bf;

        //read each frame
        for (int a = 0; a < framec; a++){
            PoseBone nbone;
            float stuff[30];

            if (!fread(stuff, sizeof(float), 16, f))
                return false;

            int index = 0;
            for (int y = 0; y < 4; y++){
                for (int x = 0; x < 4; x++){
                    nbone.mat[y][x] = stuff[index];
                    index ++;
                }
            }

            bf.frames.add(nbone);
        }
        anim.add(bf);
    }

    //printf("loaded animated bones: %lu\n", anim.count());

    //puts("Loaded");
    return true;
}
//---------------------------------------------
bool Subset::save(FILE * f){
    if (!f)
        return false;
    mesh.save(f);
    if (!fwrite(&_bonecount, sizeof(int), 1, f))
        return false;

    if (_bonecount){
        for (unsigned a = 0; a < mesh._vertexcount; a++){

            if (!vertsbones[a].save(f))
                return false;

        }
    }
    //----------
    int framec = (int)_framecount;
    if (!fwrite(&framec, sizeof(int), 1, f))
        return false;
    for (unsigned b = 0; b < _bonecount; b++){
        for (int a = 0; a < framec; a++){

            FlatMatrix fm;
            fm = Matrix2Flat(anim[b].frames[a].mat);

            if (!fwrite(fm.m, sizeof(float), 16, f))
                return false;
        }
    }

    return true;
}

//---------------------------------------------
bool SubsetCollection::load(const char* filename, bool createVBO){

    FILE * modelfile = 0;

    modelfile = fopen(filename, "rb");
    if (!modelfile)
        return false;

    char magic[4];

    if (!fread(magic, 1, 3, modelfile)){
        fclose(modelfile);
        return false;
    }
    magic[3] = '\0';
    puts(magic);
    if (strcmp(magic,"RM2") != 0){
        puts("not RM2");
        return false;
    }
        
    unsigned subsetCount = 0;
    if (!fread(&subsetCount, sizeof(int), 1, modelfile)){
        fclose(modelfile);
        return false;
    }

    for (unsigned i = 0; i < subsetCount; i++){
        Subset newSubset;
        newSubset.load(modelfile, createVBO); 
        subsets.add(newSubset);
    }

    fclose(modelfile);

    return true;
}
//--------------------------------------------------------------
bool SubsetCollection::save(const char * filename){

    FILE * modelfile = 0;
    modelfile = fopen(filename, "wb+");
    if (!modelfile)
        return false;

    char magic[4];
    strcpy(magic, "RM2");
    magic[3] = '\0';
    if (!fwrite(magic, 1, 3, modelfile)){
        fclose(modelfile);
        return false;
    }
    unsigned long subsetCount = subsets.count();
    if (!fwrite(&subsetCount, 1, sizeof(int), modelfile)){
        fclose(modelfile);
        return false;
    }

    for (unsigned i = 0; i < subsetCount; i++){
        if (!subsets[i].save(modelfile)){
            fclose(modelfile);
            return false;
        }

    }



    fclose(modelfile);
    return true;
}

//--------------------------------------------
bool SubsetCollection::draw(unsigned subset, unsigned frame){
    if (!subsets.count())
        return false;

    if (subset >= subsets.count())
        subset = 0;

        
    subsets[subset].draw(frame);

    return true;
}
//--------------------------------------------
bool SubsetCollection::drawOutline(unsigned subset,
                                   unsigned frame,
                                   float width,
                                   COLOR color){
        if (!subsets.count())
            return false;

        bool enableLightAfter = false;

        if (glIsEnabled(GL_LIGHTING)){
            glDisable (GL_LIGHTING);
            enableLightAfter = true;
        }
        glDisable(GL_TEXTURE_2D);


        glPolygonMode (GL_BACK, GL_LINE);
        glLineWidth (width);

        int func = GL_LESS;
        glGetIntegerv(GL_DEPTH_FUNC, &func);

        glCullFace (GL_FRONT);
        glDepthFunc (GL_LEQUAL);

        glColor4fv (color.c);

        if (subset >= subsets.count())
            subset = 0;

        subsets[subset].draw(frame);

        glDepthFunc (func);
        glCullFace (GL_BACK);

        glPolygonMode (GL_BACK, GL_FILL);

        glColor3f (1.0f, 1.0f, 1.0f);

        glEnable (GL_TEXTURE_2D);
        if (enableLightAfter)
            glEnable (GL_LIGHTING);
        glLineWidth (1.0f);

        return true;
}
//--------------------------------------------
Subset* SubsetCollection::getSubset(unsigned long index){
    if (index < subsets.count())
        return &subsets[index];

    return 0;

}
//---------------------------------------------
void SubsetCollection::addSubset(Subset& s, bool useVBO){

    Subset newSub;

    puts("copying stuff...");

    newSub.mesh.verts = (float*)malloc((s.mesh._vertexcount * 3 + 1)
                                        * sizeof(float));
    memcpy(newSub.mesh.verts, s.mesh.verts, (s.mesh._vertexcount * 3 + 1)
                                            * sizeof(float));
    newSub.mesh._vertexcount = s.mesh._vertexcount;

    printf("vcount %lu\n", newSub.mesh._vertexcount);

    newSub.mesh.normals = (float*)malloc((s.mesh._vertexcount * 3 + 1) 
                                         * sizeof(float));
    memcpy(newSub.mesh.normals, 
           s.mesh.normals, (s.mesh._vertexcount * 3 + 1) * sizeof(float));

    newSub.mesh.indices = (unsigned*)malloc((s.mesh.faceCount() * 3 + 1) 
                                             * sizeof(int));
    memcpy(newSub.mesh.indices, s.mesh.indices, (s.mesh.faceCount() * 3 + 1) 
                                                * sizeof(int));
    newSub.mesh.setFaceCount(s.mesh.faceCount());
   
    puts("generating unindexed mesh");

    newSub.mesh.createUnindexedMesh(useVBO);

    newSub.mesh.isUVS = s.mesh.isUVS;
    newSub.mesh.uvs.copyFrom(s.mesh.uvs);

    newSub.setFramecount(s.framecount());
    newSub.setBonecount(s.bonecount());
    //copy vertsbones
    if (s.bonecount()){
        for (unsigned a = 0; a < newSub.mesh._vertexcount; a++){
            
            AssignedBones b;

            for (unsigned long i = 0; i < s.vertsbones[a].bones.count(); i++){

                AssignedBoneToVertice as;

                as.boneID = s.vertsbones[a].bones[i].boneID;
                as.weight = s.vertsbones[a].bones[i].weight;
                b.bones.add(as);
            }

            newSub.vertsbones.add(b);
            TransformedVertex tv;
            tv.frame = -1;
            newSub.tvertices.add(tv);
        }
    }
    //copy anim
    for (unsigned b = 0; b < s.bonecount(); b++){
        PoseBoneFrames bf;
        for (unsigned a = 0; a < s.framecount(); a++){
            PoseBone nbone;
            for (int y = 0; y < 4; y++){
                for (int x = 0; x < 4; x++){
                    nbone.mat[y][x] = s.anim[b].frames[a].mat[y][x];
                }
            }

            bf.frames.add(nbone);
        }
        newSub.anim.add(bf);
    }


    puts("done");

    subsets.add(newSub);

}
//--------------------------------------------
void SubsetCollection::drawall(unsigned frame){

    for (unsigned i = 0; i < subsets.count(); i++){
        subsets[i].draw(frame);
    }
}
//---------------------------------------------
void SubsetCollection::destroy(){
    for (unsigned i = 0; i < subsets.count(); i++){
        subsets[i].destroy();
    }
    subsets.destroy();
}
//---------------------------------------------
void SubsetCollection::dump(const char * path){
    FILE * f;

    f = fopen(path, "wt+");
    if (f){
    /*    
        for (unsigned i = 0; i < armatures.count(); i++){
            fprintf(f, "Object %u:\n", i);

            for (unsigned a = 0; a < armatures[i].frames.count(); a++){
                fprintf(f, "  Frame %u:\n", a);

                for (unsigned x = 0; x < armatures[i].frames[a].pbones.count(); x++){

                    fprintf(f, "    %2.6f %2.6f %2.6f %2.6f\n",
                            armatures[i].frames[a].pbones[x].mat[0][0],
                            armatures[i].frames[a].pbones[x].mat[0][1],
                            armatures[i].frames[a].pbones[x].mat[0][2],
                            armatures[i].frames[a].pbones[x].mat[0][3]
                            );
                    fprintf(f, "    %2.6f %2.6f %2.6f %2.6f\n",
                            armatures[i].frames[a].pbones[x].mat[1][0],
                            armatures[i].frames[a].pbones[x].mat[1][1],
                            armatures[i].frames[a].pbones[x].mat[1][2],
                            armatures[i].frames[a].pbones[x].mat[1][3]
                            );
                    fprintf(f, "    %2.6f %2.6f %2.6f %2.6f\n",
                            armatures[i].frames[a].pbones[x].mat[2][0],
                            armatures[i].frames[a].pbones[x].mat[2][1],
                            armatures[i].frames[a].pbones[x].mat[2][2],
                            armatures[i].frames[a].pbones[x].mat[2][3]
                            );
                    fprintf(f, "    %2.6f %2.6f %2.6f %2.6f\n",
                            armatures[i].frames[a].pbones[x].mat[3][0],
                            armatures[i].frames[a].pbones[x].mat[3][1],
                            armatures[i].frames[a].pbones[x].mat[3][2],
                            armatures[i].frames[a].pbones[x].mat[3][3]
                            );
                    fprintf(f, "\n");
                }
            }
        }*/

        for (unsigned i = 0; i < subsets.count(); i++){
            fprintf(f, "subset %u [\n", i);

            fputs("----------------------------------------------------\n", f);
            for (unsigned a = 0; a < subsets[i].anim.count(); a++){
                fprintf(f, "bone: %u { \n", a);

                for (unsigned y = 0; y < subsets[i].anim[a].frames.count(); y++){

                    if (y == 29){

                    PoseBone * pb = 0;
                    pb = &subsets[i].anim[a].frames[y];



                    fprintf(f, "%u.\n", y);

                    fprintf(f, "%2.3f %2.3f %2.3f %2.3f \n", 
                            pb->mat[0][0], pb->mat[0][1], pb->mat[0][2], pb->mat[0][3]);
                    fprintf(f, "%2.3f %2.3f %2.3f %2.3f \n",
                            pb->mat[1][0], pb->mat[1][1], pb->mat[1][2], pb->mat[1][3]);
                    fprintf(f, "%2.3f %2.3f %2.3f %2.3f \n",
                            pb->mat[2][0], pb->mat[2][1], pb->mat[2][2], pb->mat[2][3]);
                    fprintf(f, "%2.3f %2.3f %2.3f %2.3f \n",
                            pb->mat[3][0], pb->mat[3][1], pb->mat[3][2], pb->mat[3][3]);
                    fputs("\n",f);
                    }
                }


                fputs("}\n",f);

            }

            fputs("----------------------------------------------------\n", f);
            for (unsigned a = 0; a < subsets[i].vertsbones.count(); a++){
                fprintf(f, "vertex(%u){ ", a);

                float totalweight = 0.0f;

                for ( unsigned z = 0; z < subsets[i].vertsbones[a].bones.count(); z++){

                    fprintf(f, "bone:%u w:%2.3f   ", 
                            subsets[i].vertsbones[a].bones[z].boneID,
                            subsets[i].vertsbones[a].bones[z].weight
                            );
                    totalweight += subsets[i].vertsbones[a].bones[z].weight;

                }
                fprintf(f, " =(%2.3f) }\n", totalweight);
            }
            fputs("]\n", f);
        }

        fclose(f);
    }
}
//-----------------------------------
unsigned long SubsetCollection::framecount(unsigned long subset){
    if (subset >= subsets.count())
        return 0;
    else
        return subsets[subset].framecount();
}
//-----------------------------------
unsigned long SubsetCollection::facecount(unsigned long subset){

    if (subset >= subsets.count())
        return 0;
    else
        return subsets[subset].mesh._vertexcount;
}
