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
#include "TextParticles.h"

#include "../Extensions.h"


void TextParticles::generate(const char * text, int systemlifetime,
                         float speed,
                         Vector3D pos, SpaceShift& dir,
                         float size,
                         COLOR scolor,
                         COLOR ecolor,
                         bool stableDir){

    destroy();

    //newTextParticlesAtUpdate = TextParticlesAtUpdate;
    
    unsigned maxParticles = (unsigned)strlen(text);
    newParticlesAtUpdate = maxParticles;

    baseSpeed = speed;

    float * _verts = new float[maxParticles*4*3];
    float * _colors = new float[maxParticles*4*4];
    float * _uvs = new float[maxParticles*4*2];

    capacity = maxParticles;
    _count = 0;

    particleSize = size;

    systemLifetime =   systemlifetime;
    particleLifetime = systemLifetime;
    stableDirection = stableDir;

    startColor = scolor;
    endColor = ecolor;

    direction.divDir = dir.divDir;
    direction.constDir = dir.constDir;

    direction.random[0] = dir.random[0];
    direction.random[1] = dir.random[1];
    direction.random[2] = dir.random[2];


    direction.minusrandom[0] = dir.minusrandom[0];
    direction.minusrandom[1] = dir.minusrandom[1];
    direction.minusrandom[2] = dir.minusrandom[2];

    _pos = pos;


    unsigned long colorIndex = 0;

    for (unsigned i = 0; i < maxParticles; i++){

        _verts[i*4 * 3 ]     = i * particleSize + pos.x() -  particleSize/2.0f;
        _verts[i*4 * 3 + 1]  = pos.y() + particleSize/2.0f;
        _verts[i*4 * 3 + 2]  = pos.z();

        _verts[i*4 * 3 + 3]  = i * particleSize + pos.x() + particleSize/2.0f;
        _verts[i*4 * 3 + 4]  = pos.y() + particleSize/2.0f;
        _verts[i*4 * 3 + 5]  = pos.z();

        _verts[i*4 * 3 + 6]  = i * particleSize + pos.x() + particleSize/2.0f;
        _verts[i*4 * 3 + 7]  = pos.y() - particleSize/2.0f;
        _verts[i*4 * 3 + 8]  = pos.z();
        
        _verts[i*4 * 3 + 9]  =  i * particleSize + pos.x() - particleSize/2.0f;
        _verts[i*4 * 3 + 10] = pos.y() - particleSize/2.0f;
        _verts[i*4 * 3 + 11] = pos.z();

        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        
        unsigned frame = text[i] - 32;
    
        int hf = frame / 16;
        int startx = (frame - 16 * hf) * 16;
        int starty = hf * 16;

        float u1 = startx / 256.0f;
        float u2 = (startx + 16.0f) / 256.0f; 
        float v1 = (256.0f - starty) / 256.0f;
        float v2 = (256.0f - starty - 16) / 256.0f;

        _uvs[i*4*2] = u1;
        _uvs[i*4*2 + 1] = v1;

        _uvs[i*4*2 + 2] = u2;
        _uvs[i*4*2 + 3] = v1;

        _uvs[i*4*2 + 4] = u2;
        _uvs[i*4*2 + 5] = v2;

        _uvs[i*4*2 + 6] = u1;
        _uvs[i*4*2 + 7] = v2;



    }
    verts.init(_verts, VPOSITION, maxParticles * 4, true);
    colors.init(_colors, VCOLOR, maxParticles * 4, true);
    uvs.init(_uvs, VTEXCOORD0, maxParticles * 4, false);
    free(_verts);
    free(_colors);
    free(_uvs);

}

//---------------------------------------------------------

void TextParticles::draw(DCamera & cam){

    float matrix[16];

    Vector3D pos = Vector3D(0,0,0);

    if (p.count()){
        unsigned long index = p.count() / 2;
        pos = p[index].pos;
    }

   
    //glTranslatef(pos.x(), pos.y(), pos.z());

    cam.makeBillBoard(pos, matrix);
    glMultMatrixf(matrix);
    verts.bind();
    colors.bind();
    uvs.bind();

    glDrawArrays(GL_QUADS, 0, _count*4);

    uvs.unbind();
    colors.unbind();
    verts.unbind();

}
//------------------------------------------------------
void TextParticles::destroy(){

    verts.destroy();
    colors.destroy();
    uvs.destroy();

    _dead = false;
    age = 0;
}

//---------------------------------------------------

void TextParticles::updateFrame(float * ptr){

    unsigned long colorIndex = 0;
    for (unsigned i = 0; i < _count; i ++){
        
        memcpy(&ptr[colorIndex], p[i].color.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&ptr[colorIndex], p[i].color.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&ptr[colorIndex], p[i].color.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&ptr[colorIndex], p[i].color.c, sizeof(float)*4);
        colorIndex +=4;

      
        if (!p[i].dead){


            float speed = baseSpeed;
            Vector3D dir;
            if (!stableDirection){

                dir = Vector3D((rand() % direction.random[0] - direction.minusrandom[0]) / direction.divDir.x() + direction.constDir.x(),
                               (rand() % direction.random[1] - direction.minusrandom[1]) / direction.divDir.y() + direction.constDir.y(),
                               (rand() % direction.random[2] - direction.minusrandom[2]) / direction.divDir.z() + direction.constDir.z());
                dir.normalize();
            }
            else
                dir = p[i].dir; 

            p[i].pos = p[i].pos + Vector3D(dir.x()*speed, dir.y()*speed, dir.z()*speed);
            

            p[i].age++;

            if (p[i].color.c[0] < endColor.c[0])
                p[i].color.c[0] += 1.0f/particleLifetime;
            else
                p[i].color.c[0] -= 1.0f/particleLifetime;

            if (p[i].color.c[1] < endColor.c[1])
                p[i].color.c[1] += 1.0f/particleLifetime;
            else
                p[i].color.c[1] -= 1.0f/particleLifetime;

            if (p[i].color.c[2] < endColor.c[2])
                p[i].color.c[2] += 1.0f/particleLifetime;
            else
                p[i].color.c[2] -= 1.0f/particleLifetime;

            if (p[i].color.c[3] < endColor.c[3])
                p[i].color.c[3] += 1.0f/particleLifetime;
            else
                p[i].color.c[3] -= 1.0f/particleLifetime;


            if (p[i].age > particleLifetime)
                p[i].dead = true;
        }


    }



}


//-----------------------------------------------------
void TextParticles::update(){



    for (unsigned a = 0 ; a < newParticlesAtUpdate; a++){
        if (_count < capacity){
            Particle newParticle;

            newParticle.pos = _pos;


            newParticle.color = startColor;
            newParticle.size = particleSize;

            if (stableDirection)
                newParticle.dir = Vector3D((rand() % direction.random[0]
                                            - direction.minusrandom[0])
                                            / direction.divDir.x()
                                            + direction.constDir.x(),
                                           (rand() % direction.random[1]
                                            - direction.minusrandom[1])
                                           / direction.divDir.y()
                                           + direction.constDir.y(),
                                           (rand() % direction.random[2]
                                            - direction.minusrandom[2])
                                           / direction.divDir.z()
                                           + direction.constDir.z());

            newParticle.dir.normalize();

            p.add(newParticle);

            //printf("p.count : %lu _count %lu\n", p.count(), _count );
            _count++;
        }
    }
    float * ptr = 0;
    ptr = colors.lock();
    if (ptr)
        updateFrame(ptr);
    colors.unlock();

    age++;
    if (age > systemLifetime){
        _dead = true;
    }


    unsigned i = 0;
    while (i < _count){
        if (p[i].dead){
            p.remove(i);
            _count --;
        }
        i++;

    }


}

