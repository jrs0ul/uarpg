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
#ifndef _PARTICLE_SYSTEMS_H
#define _PARTICLE_SYSTEMS_H


#ifdef WIN32
    #ifdef _MSC_VER
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#else
	#include <SDL/SDL_opengl.h>
#endif

#include "Particles.h"
#include "TextParticles.h"


    struct ParticleSystem{
        Particles sys;
        GLuint tex;
        Vector3D pos;
    };

    struct ParticleText{
        TextParticles sys;
        GLuint tex;
        Vector3D pos;
    };


    class ParticleSystems{
        DArray<ParticleSystem> systems;
        DArray<ParticleText> textsys;

    public:
        void update();
        void addBloodStream(Vector3D position, Vector3D constDir, GLuint texi);
        void addPoisonCloud(Vector3D position, GLuint texi);
        void addHealingAura(Vector3D position, GLuint texi);
        void addFloatingNumbers(Vector3D position, const char * text,
                                GLuint texi,
                                COLOR col = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

        void draw(DCamera & cam);
        void destroy();
    };


#endif //_PARTICLE_SYSTEMS_H
