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

#include "Arena.h"


    //Reads four vectors from file
    bool CArena::readCamState(XmlNode * node, CameraState& cs){

        float x, y, z;
        XmlNode* component = 0;
        component = node->getNode(L"Pos");
        if (!component)
            return false;

        char buf[255];
        wcstombs(buf, component->getAttribute(0)->getValue(), 255);
        x = atof(buf);
        wcstombs(buf, component->getAttribute(1)->getValue(), 255);
        y = atof(buf);
        wcstombs(buf, component->getAttribute(2)->getValue(), 255);
        z = atof(buf);


        cs.pos = Vector3D(x, y, z);
        //printf("pos %f %f %f\n", x, y, z);

        component = 0;
        component = node->getNode(L"Dir");
        if (!component)
            return false;
        wcstombs(buf, component->getAttribute(0)->getValue(), 255);
        x = atof(buf);
        wcstombs(buf, component->getAttribute(1)->getValue(), 255);
        y = atof(buf);
        wcstombs(buf, component->getAttribute(2)->getValue(), 255);
        z = atof(buf);

        cs.dir = Vector3D(x, y, z);
        //printf("dir %f %f %f\n", x, y, z);

        component = 0;
        component = node->getNode(L"Right");
        if (!component)
            return false;
        wcstombs(buf, component->getAttribute(0)->getValue(), 255);
        x = atof(buf);
        wcstombs(buf, component->getAttribute(1)->getValue(), 255);
        y = atof(buf);
        wcstombs(buf, component->getAttribute(2)->getValue(), 255);
        z = atof(buf);

        cs.right = Vector3D(x, y, z);
        //printf("right %f %f %f\n", x, y, z);

        component = 0;
        component = node->getNode(L"Up");
        if (!component)
            return false;
        wcstombs(buf, component->getAttribute(0)->getValue(), 255);
        x = atof(buf);
        wcstombs(buf, component->getAttribute(1)->getValue(), 255);
        y = atof(buf);
        wcstombs(buf, component->getAttribute(2)->getValue(), 255);
        z = atof(buf);


        cs.up = Vector3D(x, y, z);
        //printf("up %f %f %f\n", x, y, z);

        return true;

    }
    //-----------------------------------------
    bool CArena::readColor(XmlNode* node, COLOR& c){
        char buf[255];
        float r,g,b,a;
        if (!node)
            return false;

        wcstombs(buf, node->getAttribute(0)->getValue(), 255);
        r = atof(buf);
        wcstombs(buf, node->getAttribute(1)->getValue(), 255);
        g = atof(buf);
        wcstombs(buf, node->getAttribute(2)->getValue(), 255);
        b = atof(buf);
        wcstombs(buf, node->getAttribute(3)->getValue(), 255);
        a = atof(buf);
        c = COLOR(r, g, b, a);

        return true;

    }
    //-----------------------------------------

    /*loads xml file, gets names of mesh and textures
    loads them*/
    bool CArena::load(const char * path, const char * basepath){

        Xml data;
        
        puts(path);
        if (!data.load(path))
            return false;

        char nmeshName[255] = {0};
        char ntextureName[255] = {0};

        XmlNode * arena = 0;

        arena = data.root.getNode(L"Arena");
        if (arena){
            wcstombs(nmeshName, arena->getNode(L"Model")->getValue(), 255);

            XmlNode * textures = 0;
            textures = arena->getNode(L"Textures");
            //loads all textures
            if (textures){

                for (unsigned long i = 0; i < textures->childrenCount(); i++){
                    wcstombs(ntextureName, 
                             textures->getNode(i)->getValue(),
                             255);
                
                    PicData * info = 0;
                    info = _textures.getInfo(i);
                    bool loadIt = true;
                    if (info){
                        if (strcmp(info->name, ntextureName) == 0)
                            loadIt = false; //no nead to reload same texture
                    }

                    if (loadIt){
                        _textures.loadFile(ntextureName, i, 512, 512, basepath, 1);
                    }
                }
            }
            //load texture indexes
            XmlNode * textureIndexes = 0;
            textureIndexes = arena->getNode(L"SubsetTextures");
            texIndexes.destroy();
            if (textureIndexes){
                for (unsigned long i = 0; 
                     i < textureIndexes->childrenCount(); i++){
                    XmlNode * indexNode = 0;
                    indexNode = textureIndexes->getNode(i);
                    if (indexNode){
                        unsigned long index;
                        char buf[255];
                        wcstombs(buf, indexNode->getValue(), 255);
                        index = atoi(buf);
                        texIndexes.add(index);
                    }

                }
            }


            XmlNode * light = 0;
            light = arena->getNode(L"Light");
            if (light){

                XmlNode * _diffuse = 0;
                XmlNode * _ambient = 0;
                XmlNode * _pos = 0;
                XmlNode * _dir = 0;
                _ambient = light->getNode(L"Ambient");
                readColor(_ambient, ambient);
                _diffuse = light->getNode(L"Diffuse");
                readColor(_diffuse, diffuse);
                _pos = light->getNode(L"Pos");
                COLOR tmp;
                readColor(_pos, tmp);
                lightPos = Vector3D(tmp.c[0], tmp.c[1], tmp.c[2]);
                lightPos.v[3] = tmp.c[3];

                _dir = light->getNode(L"LightPos");
                if (_dir){
                    readColor(_dir, tmp);
                    lPos = Vector3D(tmp.c[0], tmp.c[1], tmp.c[2]);
                }
                _dir = 0;
                _dir = light->getNode(L"LightTo");
                if (_dir){
                    readColor(_dir, tmp);
                    lTo = Vector3D(tmp.c[0], tmp.c[1], tmp.c[2]);
                }


            }


            XmlNode * attribs = 0;
            attribs = arena->getNode(L"Attribs");
            if (attribs){
                subAttribs.destroy();
                for (unsigned long i = 0; i < attribs->childrenCount(); i++){
                    XmlNode * attrib = 0;
                    attrib = attribs->getNode(i);
                    if (attrib){
                        ArenaSubsetAttrib na;
                        char buf[255];
                        wcstombs(buf, attrib->getAttribute(0)->getValue(), 255);
                        na.drawBack = (atoi(buf)) ? true : false;
                        wcstombs(buf, attrib->getAttribute(1)->getValue(), 255);
                        na.alphaTest = (atoi(buf)) ? true : false;

                        XmlNode * diffuse = 0;
                        XmlNode * ambient = 0;
                        diffuse = attrib->getNode(L"Diffuse");
                        readColor(diffuse, na.diffuse);
                        ambient = attrib->getNode(L"Ambient");
                        readColor(ambient, na.ambient);
                        subAttribs.add(na);
                    }

                }

            }


            randomPositions.destroy();
            introPath.destroy();
            outroPath.destroy();


            //Loads intro camera path
            XmlNode * _introPath = 0;
            _introPath = arena->getNode(L"IntroPath");
            if (_introPath){
                for (unsigned long i = 0; i < _introPath->childrenCount(); i++){
                    XmlNode * node = 0;
                    node = _introPath->getNode(i);
                    CameraState cs;
                    if (node){
                        puts("geting node");
                        readCamState(node, cs);
                        introPath.add(cs);
                    }
                }
            }

            //loads random camera points
            XmlNode * cameraPoints = 0;
            cameraPoints = arena->getNode(L"CameraPoints");
            if (cameraPoints){
                for (unsigned long i = 0; i < cameraPoints->childrenCount(); i++){
                    XmlNode * point = 0;
                    point = cameraPoints->getNode(i);
                    CameraState cs;
                    if (point){
                        puts("geting point");
                        readCamState(point, cs);
                        randomPositions.add(cs);
                    }
                }

            }

            XmlNode * _outroPath = 0;
            _outroPath = arena->getNode(L"OutroPath");
            if (_outroPath){
                for (unsigned long i = 0; i < _outroPath->childrenCount(); i++){
                    XmlNode * node = 0;
                    node = _outroPath->getNode(i);
                    CameraState cs;
                    if (node){
                        readCamState(node, cs);
                        outroPath.add(cs);
                    }
                }
            }
        }

        data.destroy();

        

        char buf[255];

        if (strcmp(meshName, nmeshName) != 0){
            mesh.destroy();
            sprintf(buf, "%s%s", basepath, nmeshName);
            if (!mesh.load(buf)){
                puts("Loading from base");
                sprintf(buf, "base/%s", nmeshName);
                mesh.load(buf);
            }

            strncpy(meshName, nmeshName, 255);
        }

        

        return true;

    }

    //--------------------------------------
    void CArena::setLightning(){

        float nom[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.c);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.c);
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos.v);
        glLightfv(GL_LIGHT0, GL_SPECULAR, nom);
        glEnable(GL_LIGHT0);
    }



//---------------------------------------

void CArena::draw(ShaderProgram * meshShader, 
                  bool drawOutline,
                  bool drawblack){


    glPopMatrix();
    glPushMatrix();

    for (unsigned i = 0; i < mesh.count(); i++){

        if (!drawOutline){
            unsigned long tindex = 0;
            if (i < texIndexes.count())
                tindex = texIndexes[i];
            if (meshShader)
                meshShader->use();

            if (!drawblack){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.c);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.c);
            }
            else{
                COLOR black(0,0,0,1);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, black.c);
                glMaterialfv(GL_FRONT, GL_AMBIENT, black.c);
            }

            glBindTexture(GL_TEXTURE_2D,
                          _textures.getname(tindex));
            glTranslatef(0, 0, 0);

            if (subAttribs[i].alphaTest){
                glAlphaFunc(GL_GREATER,0.6);
                glEnable(GL_ALPHA_TEST);
            }

            if (subAttribs[i].drawBack){
                glDisable (GL_CULL_FACE);
                glPolygonMode (GL_FRONT|GL_BACK, GL_FILL);
            }
            mesh.draw(i);
            if (subAttribs[i].drawBack){
                glEnable (GL_CULL_FACE);
                glPolygonMode (GL_FRONT, GL_FILL);
            }

            if (subAttribs[i].alphaTest)
                glDisable(GL_ALPHA_TEST);

        }
        else{ //just outline
            if (!subAttribs[i].drawBack){
                if (meshShader)
                    glUseProgram(0);
                mesh.drawOutline(i);
            }
        }

    }//for
}


//---------------------------------------
    CameraState * CArena::getPosition(unsigned long i){
        if (i < randomPositions.count()){

            return &randomPositions[i];
        }

        return 0;
    }
    //--------------------------------------
    CameraState * CArena::getIPathNode(unsigned long i){
        if (i < introPath.count()){

            return &introPath[i];
        }

        return 0;

    }
   //--------------------------------------
    CameraState * CArena::getOPathNode(unsigned long i){
        if (i < introPath.count()){

            return &outroPath[i];
        }

        return 0;

    }


    //------------------------------------

    void CArena::destroy(){

        _textures.destroy();
        mesh.destroy();
        randomPositions.destroy();
        introPath.destroy();
        outroPath.destroy();
        texIndexes.destroy();
        subAttribs.destroy();

    }
