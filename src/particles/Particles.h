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
#ifndef _PARTICLES_H
#define _PARTICLES_H

#include "../DArray.h"
#include "../Camera.h"
#include "../Vectors.h"
#include "../Colors.h"
#include "../VertexObject.h"

struct Particle{

    Vector3D pos;
    Vector3D dir;
    float size;
    COLOR color;
    int age;
    bool dead;

    Particle(){
        dead = false;
        age = 0;
        size = 1.0f;
    }

};

struct SpaceShift{
    Vector3D constDir;
    Vector3D divDir;
    int random[3];
    int minusrandom[3];

};

//------------------------


class Particles{
    DArray<Particle> p;

    VertexObject verts;
    VertexObject colors;
    VertexObject uvs;

    unsigned _count;
    unsigned long capacity;
    COLOR startColor;
    COLOR endColor;
    float particleSize;
    int particleLifetime;
    int systemLifetime;

    int age;
    bool _dead;

    bool stableDirection;

    SpaceShift origin;
    SpaceShift direction;


    unsigned newParticlesAtUpdate;
    float baseSpeed;

    void faceCamera(DCamera & cam, float * ptr);
    void updateFrame(float * ptr);
public:
    void generate(unsigned maxparticles, int systemlifetime, int particlelifetime,
                  float speed, unsigned particlesAtUpdate,
                  SpaceShift& org, SpaceShift& dir, float size = 1.0f,
                  COLOR scolor = COLOR(1.0f,1.0f,1.0f,1.0f), COLOR ecolor = COLOR(1.0f, 1.0f, 1.0f, 0.0f),
                  bool stableDir = false);
    void draw(DCamera& cam);
    void update();

    void destroy();
    bool dead(){return _dead;}
    Particles(){
                _count = 0;
                newParticlesAtUpdate = 0;
                age = 0;
                _dead = false;
               }

};



#endif //_PARTICLES_H
