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
#include "Particles.h"

#include "../Extensions.h"


void Particles::generate(unsigned maxparticles,
                         int systemlifetime, int particlelifetime,
                         float speed, unsigned particlesAtUpdate,
                         SpaceShift& org, SpaceShift& dir,
                         float size, COLOR scolor, COLOR ecolor,
                         bool stableDir){


    destroy();

    newParticlesAtUpdate = particlesAtUpdate;

    baseSpeed = speed;

    float * _verts = (float*)malloc(maxparticles * 4 * 3 * sizeof(float));
    float * _colors = (float*)malloc(maxparticles * 4 * 4 * sizeof(float));
    float * _uvs = (float *)malloc(maxparticles * 4 * 2 * sizeof(float));


    capacity = maxparticles;
    _count = 0;

    particleSize = size;
    particleLifetime = particlelifetime;
    systemLifetime = systemlifetime;
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

    origin.divDir = org.divDir;
    origin.constDir = org.constDir;

    origin.random[0] = org.random[0];
    origin.random[1] = org.random[1];
    origin.random[2] = org.random[2];


    origin.minusrandom[0] = org.minusrandom[0];
    origin.minusrandom[1] = org.minusrandom[1];
    origin.minusrandom[2] = org.minusrandom[2];

    unsigned long colorIndex = 0;

    for (unsigned i = 0; i < maxparticles; i++){

        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;
        memcpy(&_colors[colorIndex], scolor.c, sizeof(float)*4);
        colorIndex +=4;

        _uvs[i*4*2] = 0.0f;
        _uvs[i*4*2 + 1] = 0.0f;

        _uvs[i*4*2 + 2] = 1.0f;
        _uvs[i*4*2 + 3] = 0.0f;

        _uvs[i*4*2 + 4] = 1.0f;
        _uvs[i*4*2 + 5] = 1.0f;

        _uvs[i*4*2 + 6] = 0.0f;
        _uvs[i*4*2 + 7] = 1.0f;

    }
    verts.init(_verts, VPOSITION, maxparticles * 4, true);
    colors.init(_colors, VCOLOR, maxparticles * 4, true);
    uvs.init(_uvs, VTEXCOORD0, maxparticles * 4, false);
    free(_verts);
    free(_colors);
    free(_uvs);
}
//-------------------------------------------------------

void Particles::faceCamera(DCamera & cam, float * ptr){

    for (unsigned i = 0; i < _count; i ++){

        float width = p[i].size*0.5f;
        float height = width;


        Vector3D r = Vector3D(cam.right().x() * width, cam.right().y() * width, cam.right().z()*width);
        Vector3D u = Vector3D(cam.up().x()* height , cam.up().y()*height, cam.up().z()*height);
        Vector3D p1 = Vector3D(p[i].pos-r-u);
        Vector3D p2 = Vector3D(p[i].pos+r-u);
        Vector3D p3 = Vector3D(p[i].pos+r+u);
        Vector3D p4 = Vector3D(p[i].pos-r+u);


        ptr[i*4*3] = p1.x();
        ptr[i*4*3 + 1] =p1.y();
        ptr[i*4*3 + 2] =p1.z();

        ptr[i*4*3 + 3] =p2.x();
        ptr[i*4*3 + 4] =p2.y();
        ptr[i*4*3 + 5] =p2.z();

        ptr[i*4*3 + 6] =p3.x();
        ptr[i*4*3 + 7] =p3.y();
        ptr[i*4*3 + 8] =p3.z();

        ptr[i*4*3 + 9] =p4.x();
        ptr[i*4*3 + 10] =p4.y();
        ptr[i*4*3 + 11] =p4.z();
    }
}

//---------------------------------------------------------

void Particles::draw(DCamera & cam){

    float * ptr = 0;
    ptr = verts.lock();
    if (ptr){
        faceCamera(cam, ptr);
    }
    verts.unlock();

    verts.bind();
    colors.bind();
    uvs.bind();

    glDrawArrays(GL_QUADS, 0, _count * 4);

    uvs.unbind();
    colors.unbind();
    verts.unbind();
}

//------------------------------------------------------
void Particles::destroy(){

    verts.destroy();
    colors.destroy();
    uvs.destroy();

    _dead = false;
    age = 0;
}

//---------------------------------------------------

void Particles::updateFrame(float * ptr){

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

                dir = Vector3D((rand() % direction.random[0] - direction.minusrandom[0]) 
                                / direction.divDir.x() + direction.constDir.x(),
                               (rand() % direction.random[1] - direction.minusrandom[1]) 
                                / direction.divDir.y() + direction.constDir.y(),
                               (rand() % direction.random[2] - direction.minusrandom[2]) 
                                / direction.divDir.z() + direction.constDir.z());
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
void Particles::update(){

    for (unsigned a = 0 ; a < newParticlesAtUpdate; a++){
        if (_count < capacity){
            Particle newParticle;

            newParticle.pos = Vector3D((rand() % origin.random[0] - origin.minusrandom[0])
                                       / origin.divDir.x() + origin.constDir.x(),
                                       (rand() % origin.random[1] - origin.minusrandom[1]) 
                                       / origin.divDir.y() + origin.constDir.y(),
                                      (rand() % origin.random[2] - origin.minusrandom[2])
                                       / origin.divDir.z() + origin.constDir.z());

            newParticle.color = startColor;
            newParticle.size = particleSize;

            if (stableDirection)
                newParticle.dir = Vector3D((rand() % direction.random[0]
                                  - direction.minusrandom[0]) / 
                                  direction.divDir.x()
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
            _count++;
        }
    }

    float * ptr = 0;
    ptr = colors.lock();
    if (ptr){
        updateFrame(ptr);
    }
    colors.unlock();

    age++;
    if (age > systemLifetime){
        _dead = true;
    }


    unsigned long deadparticles = 0;
    for (unsigned i = 0; i < _count; i++){
        if (p[i].dead){
            p.remove(i);
            deadparticles ++;
        }
    }
    _count -= deadparticles;

}

