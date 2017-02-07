/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul   *
 *   jrs0ul@gmail.com   *
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
#include <cstdio>
#include <cstring>
#include <cmath>
#include "TextureLoader.h"
#include "Vectors.h"




GLuint PicsContainer::getname(unsigned long index){
    if (index < TexNames.count())
        return TexNames[index];
    return 0;
}
//-----------------------------

PicData* PicsContainer::getInfo(unsigned long index){
    if (index < TexNames.count())
        return &PicInfo[index];
    return 0;
}
//-----------------------------
bool PicsContainer::load(const char *list){


    if (!initContainer(list))
        return false;

    for (unsigned long i = 0; i < PicInfo.count(); i++){

        Image naujas;

        unsigned short imageBits = 0;
        if (!naujas.loadTga(PicInfo[i].name,imageBits))
            printf("%s not found or corrupted by M$\n",PicInfo[i].name);
        PicInfo[i].width = naujas.width;
        PicInfo[i].height = naujas.height;


        PicInfo[i].htilew = PicInfo[i].twidth/2.0f;
        PicInfo[i].htileh = PicInfo[i].theight/2.0f;
        PicInfo[i].vframes = PicInfo[i].height/PicInfo[i].theight;
        PicInfo[i].hframes = PicInfo[i].width/PicInfo[i].twidth;

        int filtras = GL_NEAREST;
        if (PicInfo[i].filter)
            filtras = GL_LINEAR;


        glBindTexture(GL_TEXTURE_2D, TexNames[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

        if (imageBits > 24)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
                         0, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
                         0, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

        naujas.destroy();

    }
    return true;
}

//---------------------------------------------------
void PicsContainer::draw(
                long textureIndex,
                float x, float y,
                unsigned int frame,
                bool useCenter,
                float scaleX, float scaleY,
                float rotationAngle,
                COLOR upColor,
                COLOR dwColor
               ){

        SpriteBatchItem nb;

        nb.x = x;
        nb.y = y;
        nb.textureIndex = textureIndex;
        nb.frame = frame;
        nb.useCenter = useCenter;
        nb.scaleX = scaleX;
        nb.scaleY = scaleY;
        nb.rotationAngle = rotationAngle;
        nb.upColor = upColor;
        nb.dwColor = dwColor;

        batch.add(nb);
}
//---------------------------------------------------
    void PicsContainer::drawVA(void * vertices, void * uvs, void *colors,
                               unsigned uvsCount, unsigned vertexCount){
        glEnableClientState(GL_VERTEX_ARRAY);
        if (uvsCount > 0){
           // if (!glIsEnabled(GL_TEXTURE_2D))
             //   puts("texture disabled");
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        if (uvsCount > 0)
            glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glColorPointer(4, GL_FLOAT, 0, colors);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount / 2 );
        glDisableClientState(GL_COLOR_ARRAY);
        if (uvsCount > 0)
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

    }
//----------------------------------------------------------
Vector3D CalcUvs(PicData * p, unsigned frame){

    float hf = frame / p->hframes;

    float startx =  (frame - p->hframes * hf) * p->twidth;
    float starty = hf * p->theight;

    Vector3D result = Vector3D(
                               (startx*1.0f)/(p->width*1.0f),
                               ((startx+p->twidth)*1.0f)/(p->width*1.0f),
                               (( p->height - starty) * 1.0f ) / ( p->height * 1.0f )- 0.0001f,
                               (( p->height - starty - p->theight ) * 1.0f) / (p->height * 1.0f)
                              );
    return result;
}

//----------------------------------------------------------
void PicsContainer::drawBatch(ShaderProgram * justcolor,
                              ShaderProgram * texcolor,
                              int method){

        switch(method){
              //TODO: complete VA
        default:{
                DArray<float> vertices;
                DArray<float> uvs;
                float * colors = 0;
                unsigned colorIndex = 0;
                if (batch.count()){
                    colors = (float*)malloc(sizeof(float) * 24 * batch.count());
                }

                Vector3D uv(0.0f, 0.0f, 0.0f, 0.0f);
                float htilew, htileh;
                float twidth, theight;


                long texIndex = -1;

                for (unsigned long i = 0; i < batch.count(); i++){

                    PicData * p = 0;

                    htilew = 0.5f; htileh = 0.5f;
                    twidth = 1; theight = 1;

                    if ((batch[i].textureIndex >= 0) && ( batch[i].textureIndex < (long)count()))
                        p = &PicInfo[batch[i].textureIndex];

                    if(p){

                        uv = CalcUvs(p, batch[i].frame);
                        htilew = p->twidth / 2.0f; htileh = p->theight / 2.0f;
                        twidth = p->twidth; theight = p->theight;
                    }


                    if ((texIndex != batch[i].textureIndex)
                        && (vertices.count() > 0)){
                        //let's draw old stuff
                        if ((texIndex >= 0) && (texIndex < (long)count())){
                            glBindTexture(GL_TEXTURE_2D, TexNames[texIndex]);
                            if (texcolor){

                                texcolor->use();
                            }
                        }

                        else{
                            glBindTexture(GL_TEXTURE_2D, 0);
                            if (justcolor){
                                justcolor->use();
                            }
                        }

                        //SDL 1.2
                        glLoadIdentity();

                        drawVA(vertices.getData(), uvs.getData(), colors,
                               uvs.count(), vertices.count());

                        //printf("quad count %d \n",vertices.count());

                        vertices.destroy();
                        uvs.destroy();
                        colorIndex = 0;

                }
                texIndex = batch[i].textureIndex;

                //append to arrays
                uvs.add(uv.v[0]); uvs.add(uv.v[2]);
                uvs.add(uv.v[1]); uvs.add(uv.v[2]);
                uvs.add(uv.v[1]); uvs.add(uv.v[3]);
                uvs.add(uv.v[0]); uvs.add(uv.v[2]);
                uvs.add(uv.v[1]); uvs.add(uv.v[3]);
                uvs.add(uv.v[0]); uvs.add(uv.v[3]); 
                
                //----
                memcpy(&colors[colorIndex], batch[i].upColor.c,
                       sizeof(float) * 4);
                colorIndex += 4;

                memcpy(&colors[colorIndex], batch[i].upColor.c, 
                       sizeof(float) * 4);
                colorIndex += 4;

                memcpy(&colors[colorIndex], batch[i].dwColor.c, 
                       sizeof(float) * 4);
                colorIndex += 4;

                 memcpy(&colors[colorIndex], batch[i].upColor.c, 
                       sizeof(float) * 4);
                colorIndex += 4;

                memcpy(&colors[colorIndex], batch[i].dwColor.c, 
                       sizeof(float) * 4);
                colorIndex += 4;

                memcpy(&colors[colorIndex], batch[i].dwColor.c, 
                       sizeof(float) * 4);
                colorIndex += 4;
                //---
                if (batch[i].rotationAngle == 0.0f){
                    if (batch[i].useCenter){
                        float hwidth = htilew * batch[i].scaleX;
                        float hheight = htileh * batch[i].scaleY;


                        vertices.add(batch[i].x - hwidth); 
                        vertices.add(batch[i].y - hheight);

                        vertices.add(batch[i].x + hwidth); 
                        vertices.add(batch[i].y - hheight);

                        vertices.add(batch[i].x + hwidth); 
                        vertices.add(batch[i].y + hheight);

                        vertices.add(batch[i].x - hwidth); 
                        vertices.add(batch[i].y - hheight);

                        vertices.add(batch[i].x + hwidth); 
                        vertices.add(batch[i].y + hheight);


                        vertices.add(batch[i].x - hwidth); 
                        vertices.add(batch[i].y + hheight);
                    }
                    else{

                        vertices.add(batch[i].x);
                        vertices.add(batch[i].y);

                        vertices.add(batch[i].x + twidth * batch[i].scaleX);
                        vertices.add(batch[i].y);

                        vertices.add(batch[i].x + twidth * batch[i].scaleX); 
                        vertices.add(batch[i].y + theight * batch[i].scaleY);

                        vertices.add(batch[i].x);
                        vertices.add(batch[i].y);

                        vertices.add(batch[i].x + twidth * batch[i].scaleX); 
                        vertices.add(batch[i].y + theight * batch[i].scaleY);


                        vertices.add(batch[i].x); 
                        vertices.add(batch[i].y + theight * batch[i].scaleY);
                    }
                
                }
                else{

                //TODO: non-centered rotation

                    float angle = batch[i].rotationAngle * 0.0174532925 + 3.14f;
                  
                    float hwidth = htilew * batch[i].scaleX;
                    float hheight = htileh * batch[i].scaleY;

                    float cos_rot_w = cosf(angle) * hwidth;
                    float cos_rot_h = cosf(angle) * hheight;
                    float sin_rot_w = sinf(angle) * hwidth;
                    float sin_rot_h = sinf(angle) * hheight;


                    vertices.add(batch[i].x - cos_rot_w - sin_rot_h); 
                    vertices.add(batch[i].y - sin_rot_w + cos_rot_h);

                    vertices.add(batch[i].x + cos_rot_w - sin_rot_h); 
                    vertices.add(batch[i].y + sin_rot_w + cos_rot_h);

                    vertices.add(batch[i].x + cos_rot_w + sin_rot_h); 
                    vertices.add(batch[i].y + sin_rot_w - cos_rot_h);

                    vertices.add(batch[i].x - cos_rot_w - sin_rot_h); 
                    vertices.add(batch[i].y - sin_rot_w + cos_rot_h);

                    vertices.add(batch[i].x + cos_rot_w + sin_rot_h); 
                    vertices.add(batch[i].y + sin_rot_w - cos_rot_h);

                    vertices.add(batch[i].x - cos_rot_w + sin_rot_h); 
                    vertices.add(batch[i].y - sin_rot_w - cos_rot_h);

                }
            } //for

            if (vertices.count() > 0){

                if ((texIndex >= 0) && (texIndex < (long)count())){
                    glBindTexture(GL_TEXTURE_2D, TexNames[texIndex/*batch[i].textureIndex*/]);
                    if (texcolor){
                        texcolor->use();
                    }
                }
                else{
                    glBindTexture(GL_TEXTURE_2D, 0);
                    if (justcolor){
                        justcolor->use();
                    }
                }

                //for SDL 1.2
                glLoadIdentity();

                drawVA(vertices.getData(), uvs.getData(), colors,
                       uvs.count(), vertices.count());

                //printf("quad count %d \n",vertices.count());


                vertices.destroy();
                uvs.destroy();
                colorIndex = 0;
            }

            if (colors)
                free(colors);
        }
    }

    batch.destroy();
}

//-----------------------------------------------------
bool PicsContainer::loadFile(const char* file, unsigned long index,
                             int twidth, int theight, int filter){


        Image naujas;

        unsigned short imageBits=0;


        if (!naujas.loadTga(file, imageBits)){
            printf("%s not found or corrupted by M$\n", file);
            return false;
        }


        if (PicInfo.count() < index + 1){

            GLuint glui = 0;
            PicData p;
            p.twidth = twidth;
            p.theight = theight;
            p.filter = filter;
            for (unsigned i = PicInfo.count(); i < index + 1; i++){
                PicInfo.add(p);
                TexNames.add(glui);
            }

            glGenTextures(1, ((GLuint *)TexNames.getData()) + index);

            char * copy = (char*)malloc(strlen(file)+1);
            strcpy(copy, file);
            char * res = 0;
            res = strtok(copy, "/");
            while (res){
                strcpy(PicInfo[index].name, res);
                res = strtok(0, "/");
            }
            free(copy);

        }
        else{
            PicData * pp = &PicInfo[index];
            pp->twidth = twidth;
            pp->theight = theight;
            pp->filter = filter;
            char * copy = (char*)malloc(strlen(file)+1);
            strcpy(copy, file);
            char * res = 0;
            res = strtok(copy, "/");
            while (res){
                strcpy(pp->name, res);
                res = strtok(0, "/");
            }
            free(copy);
            
            if (glIsTexture(TexNames[index]))
                glDeleteTextures(1, ((GLuint *)TexNames.getData()) + index);
            glGenTextures(1, ((GLuint *)TexNames.getData()) + index);

        }

       
        PicInfo[index].width = naujas.width;
        PicInfo[index].height = naujas.height;


        PicInfo[index].htilew =PicInfo[index].twidth/2.0f;
        PicInfo[index].htileh =PicInfo[index].theight/2.0f;
        PicInfo[index].vframes=PicInfo[index].height/PicInfo[index].theight;
        PicInfo[index].hframes=PicInfo[index].width/PicInfo[index].twidth;

        int filtras = GL_NEAREST;
        if (PicInfo[index].filter){
            filtras = GL_LINEAR;
        }


        glBindTexture(GL_TEXTURE_2D, TexNames[index]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

        GLint border = 0;

        if (imageBits > 24)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
                 border, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
                 border, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

        naujas.destroy();

        return true;

    }

//-----------------------------------------------------
bool PicsContainer::loadFile(const char* file, unsigned long index,
                             int twidth, int theight,
                             const char* basePath, int filter){

    

    char buf[255];
    sprintf(buf, "%s%s", basePath, file);

    if (!loadFile(buf, index, twidth, theight, filter)){
        sprintf(buf, "base/pics/%s", file);
        puts("Let's try base/");

        if (!loadFile(buf, index, twidth,  theight, filter))
            return false;

    }

    return true;

}
//--------------------------------------------------
bool PicsContainer::loadFile(unsigned long index,
                             const char * BasePath){

    Image naujas;

    unsigned short imageBits = 0;


    char dir[50];
    char buf[214];

    sprintf(dir, "%spics/", BasePath);
    sprintf(buf, "%s%s", dir, PicInfo[index].name);

    if (!naujas.loadTga(buf, imageBits)){
        sprintf(buf, "base/pics/%s", PicInfo[index].name);
        puts("Let's try base/");
        if (!naujas.loadTga(buf, imageBits)){
            printf("%s not found or corrupted by M$\n", buf);
            return false;
        }

    }
        
    PicInfo[index].width = naujas.width;
    PicInfo[index].height = naujas.height;


    PicInfo[index].htilew = PicInfo[index].twidth / 2.0f;
    PicInfo[index].htileh = PicInfo[index].theight / 2.0f;
    PicInfo[index].vframes = PicInfo[index].height / PicInfo[index].theight;
    PicInfo[index].hframes = PicInfo[index].width / PicInfo[index].twidth;

    int filtras = GL_NEAREST;
    if (PicInfo[index].filter)
        filtras = GL_LINEAR;


    glBindTexture(GL_TEXTURE_2D, TexNames[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtras );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtras );

    if (imageBits > 24)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, naujas.width, naujas.height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE,naujas.data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, naujas.width, naujas.height,
                     0, GL_RGB, GL_UNSIGNED_BYTE,naujas.data);

    naujas.destroy();

    return true;

}


//--------------------------------
int PicsContainer::findByName(const char* picname, bool debug){
    unsigned long start = 0;

    if (!PicInfo.count())
        return -1;

    
    while ((strcmp(PicInfo[start].name, picname) != 0) && (start < PicInfo.count())){
        if (debug)
            puts(PicInfo[start].name);
        start++;
    }

    if (start == PicInfo.count())
        return -1;

    return start;
}
//---------------------------------------
bool PicsContainer::initContainer(const char *list){
    FILE* failas=fopen(list,"rt");

    int result = 0;

    if (!failas)
        return false;

    while (!feof(failas)){
        PicData data;
        data.name[0] = 0;
        result = fscanf(failas,"%s\n",data.name);
        result = fscanf(failas,"%d %d %d\n",
                        &data.theight, &data.twidth,
                        &data.filter);
        PicInfo.add(data);
    }

    fclose(failas);

    for (unsigned long i = 0; i < PicInfo.count(); i++ ){
        GLuint glui = 0;
        TexNames.add(glui);
    }

    glGenTextures(PicInfo.count(), (GLuint *)TexNames.getData());

    return true;

}

//----------------------------------
void PicsContainer::destroy(){
    for (unsigned long i = 0; i < TexNames.count(); i++){
        if (glIsTexture(TexNames[i]))
            glDeleteTextures(1, ((GLuint *)TexNames.getData()) + i);
    }
    TexNames.destroy();
    batch.destroy();
    PicInfo.destroy();
}

//-------------------------------
void PicsContainer::remove(unsigned long index){
    if (index < TexNames.count()){
        if (glIsTexture(TexNames[index]))
            glDeleteTextures(1, ((GLuint *)TexNames.getData()) + index);

        TexNames.remove(index);
        PicInfo.remove(index);
    }

}



