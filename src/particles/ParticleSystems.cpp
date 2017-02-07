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

#include "ParticleSystems.h"

    void ParticleSystems::addBloodStream(Vector3D position, Vector3D constDir, GLuint texi){

        ParticleSystem system;


        SpaceShift org;

        org.divDir = Vector3D(1.0f, 1.0f, 1.0f);
        org.constDir = Vector3D(0.0f, 0.0f, 0.0f);
        org.random[0] = 1;
        org.random[1] = 1;
        org.random[2] = 1;
        org.minusrandom[0] = 1;
        org.minusrandom[1] = 1;
        org.minusrandom[2] = 1;

        SpaceShift dir;
        dir.divDir = Vector3D(30.0f, 30.0f, 30.0f);
        dir.constDir = constDir;
        dir.random[0] = 1;
        dir.random[1] = 60;
        dir.random[2] = 60;
        dir.minusrandom[0] = 1;
        dir.minusrandom[1] = 30;
        dir.minusrandom[2] = 30;


        system.sys.generate(100, 30, 15, 0.1f, 1, org, dir, 1.0f, COLOR(0.8f, 0.0f, 0.0f, 1.0f), COLOR(1.0f, 0.0f, 0.0f, 0.0f), true);
        system.pos = position;
        system.tex = texi;

        systems.add(system);


    }

    //-------------------------------------------------------------
    void ParticleSystems::addHealingAura(Vector3D position, GLuint texi){

        ParticleSystem system;


        SpaceShift org;

        org.divDir = Vector3D(100.0f, 100.0f, 100.0f);
        org.constDir = Vector3D(0.0f, 0.0f, 0.0f);
        org.random[0] = 100;
        org.random[1] = 1;
        org.random[2] = 100;
        org.minusrandom[0] = 50;
        org.minusrandom[1] = 1;
        org.minusrandom[2] = 50;

        SpaceShift dir;
        dir.divDir = Vector3D(100.0f, 100.0f, 100.0f);
        dir.constDir = Vector3D(0.0f, 0.5f, 0.0f);
        dir.random[0] = 1;
        dir.random[1] = 1;
        dir.random[2] = 1;
        dir.minusrandom[0] = 1;
        dir.minusrandom[1] = 1;
        dir.minusrandom[2] = 1;


        system.sys.generate(100, 60, 50, 0.05f, 2, org, dir, 1.0f, COLOR(1.0f, 1.0f, 0.0f, 1.0f), COLOR(1.0f, 1.0f, 0.0f, 0.0f), false);
        system.pos = position;
        system.tex = texi;

        systems.add(system);

    }
    //------------------------------------------------------
    void ParticleSystems::addPoisonCloud(Vector3D position, GLuint texi){

        ParticleSystem system;


        SpaceShift org;

        org.divDir = Vector3D(50.0f, 50.0f, 50.0f);
        org.constDir = Vector3D(0.0f, 0.0f, 0.0f);
        org.random[0] = 50;
        org.random[1] = 1;
        org.random[2] = 50;
        org.minusrandom[0] = 25;
        org.minusrandom[1] = 1;
        org.minusrandom[2] = 25;

        SpaceShift dir;
        dir.divDir = Vector3D(100.0f, 100.0f, 100.0f);
        dir.constDir = Vector3D(0.0f, 0.5f, 0.0f);
        dir.random[0] = 100;
        dir.random[1] = 1;
        dir.random[2] = 100;
        dir.minusrandom[0] = 50;
        dir.minusrandom[1] = 1;
        dir.minusrandom[2] = 50;


        system.sys.generate(100, 6000, 100, 0.08f, 1, org, dir, 0.8f, COLOR(0.0f, 1.0f, 0.0f, 1.0f), COLOR(0.0f, 1.0f, 0.0f, 0.0f), true);
        system.pos = position;
        system.tex = texi;

        systems.add(system);

    }
    //------------------------------------------------------
    void ParticleSystems::addFloatingNumbers(Vector3D position, const char * text,
                                             GLuint texi,
                                             COLOR col){
        ParticleText system;

        SpaceShift dir;
        dir.divDir = Vector3D(100.0f, 100.0f, 100.0f);
        dir.constDir = Vector3D(0.0f, 0.5f, 0.0f);
        dir.random[0] = 1;
        dir.random[1] = 1;
        dir.random[2] = 1;
        dir.minusrandom[0] = 1;
        dir.minusrandom[1] = 1;
        dir.minusrandom[2] = 1;

        system.sys.generate(text, 100, 0.07f, Vector3D(0,0,0),
                            dir, 0.6f, col, COLOR(col.c[0], col.c[1], col.c[2], 0.0f), false);
        system.pos = position;
        system.tex = texi;

        textsys.add(system);
    }
    //------------------------------------------------------
    void ParticleSystems::draw(DCamera & cam){

        //glPopMatrix();
        //glPushMatrix();
        for (unsigned i = 0; i < systems.count(); i++){

            if (!systems[i].sys.dead()){
                glPopMatrix();
                glPushMatrix();

                glDepthMask(GL_FALSE);
                glTranslatef(systems[i].pos.x(), systems[i].pos.y(), systems[i].pos.z());
                glBindTexture(GL_TEXTURE_2D, systems[i].tex);
                systems[i].sys.draw(cam);
                glDepthMask(GL_TRUE);
            }
        }
        for (unsigned i = 0; i < textsys.count(); i++){

            if (!textsys[i].sys.dead()){
                glPopMatrix();
                glPushMatrix();

                
                glTranslatef(textsys[i].pos.x(), textsys[i].pos.y(), textsys[i].pos.z());
                glDisable(GL_DEPTH_TEST);
                /*glBegin(GL_LINES);

                glBindTexture(GL_TEXTURE_2D, 0);
                glColor3f(1.0f, 1.0f, 1.0f);

                glVertex3f(textsys[i].pos.x() - 1.0f, textsys[i].pos.y(), textsys[i].pos.z());
                glVertex3f(textsys[i].pos.x() + 1.0f, textsys[i].pos.y(), textsys[i].pos.z());
                
                glVertex3f(textsys[i].pos.x(), textsys[i].pos.y() - 1.0f, textsys[i].pos.z());
                glVertex3f(textsys[i].pos.x(), textsys[i].pos.y() + 1.0f, textsys[i].pos.z());

                glVertex3f(textsys[i].pos.x(), textsys[i].pos.y(), textsys[i].pos.z() - 1.0f);
                glVertex3f(textsys[i].pos.x(), textsys[i].pos.y(), textsys[i].pos.z() + 1.0f);

                glEnd();*/
                

                glPopMatrix();
                glPushMatrix();

                glTranslatef(textsys[i].pos.x(), textsys[i].pos.y(), textsys[i].pos.z());
                //glDepthMask(GL_FALSE);
                glBindTexture(GL_TEXTURE_2D, textsys[i].tex);
                textsys[i].sys.draw(cam);
                //glDepthMask(GL_TRUE);
                glEnable(GL_DEPTH_TEST);
            }
        }

    }
    //------------------------------------------------------
    void ParticleSystems::update(){
        for (unsigned i = 0; i < systems.count(); i++){
            systems[i].sys.update();
        }

        for (unsigned i = 0; i < systems.count(); i++){
            if (systems[i].sys.dead()){
                systems[i].sys.destroy();
                systems.remove(i);
                puts("removed");
            }
        }
        for (unsigned i = 0; i < textsys.count(); i++){
            textsys[i].sys.update();
        }

        for (unsigned i = 0; i < textsys.count(); i++){
            if (textsys[i].sys.dead()){
                textsys[i].sys.destroy();
                textsys.remove(i);
                puts("removed");
            }
        }

    }
  //--------------------------------------------------
    void ParticleSystems::destroy(){
        for (unsigned long i = 0; i < systems.count(); i++)
            systems[i].sys.destroy();
        systems.destroy();

        for (unsigned long i = 0; i < textsys.count(); i++)
            textsys[i].sys.destroy();
            textsys.destroy();
  }
