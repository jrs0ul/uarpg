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
        #include <SDL_opengl.h>
    #else
        #include <SDL/SDL_opengl.h>
    #endif
#else
  #include <SDL/SDL_opengl.h>
#endif

#include <cmath>
#include "DebugProps.h"
#include "Extensions.h"
#include "VertexObject.h"


//---------------------------
void DummyCircle(float r){
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1,1,1);

    glBegin(GL_LINES);


    float x = 0;
    float y = 0;
    for (int i = 0; i < 180;i++){
        x = r * cosf(i);
        y = r * sinf(i);
        glVertex3f(x , 0.5f, y );
        x = r * cosf(i + 0.1) ;
        y = r * sinf(i + 0.1) ;
        glVertex3f(x , 0.5f,  y );
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);

}
//------------------------------
void DummyLines(Vector3D axis, Vector3D pos){
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);

    float scale = 500;

    VertexObject vo;
    float verts[12];
    memcpy(&verts[0], pos.v, 3*sizeof(float));
    memcpy(&verts[3], pos.v, 3*sizeof(float));
    memcpy(&verts[6], pos.v, 3*sizeof(float));
    memcpy(&verts[9], pos.v, 3*sizeof(float));
    verts[3] += axis.v[0] * scale;
    verts[4] += axis.v[1] * scale;
    verts[5] += axis.v[2] * scale;

    verts[9]  += axis.v[0] * (-scale);
    verts[10] += axis.v[1] * (-scale);
    verts[11] += axis.v[2] * (-scale); 
    

    vo.init(verts, VPOSITION, 12*sizeof(float), false);
    vo.bind();
    glDrawArrays(GL_LINES, 0, 4);
    vo.unbind();
    
}
//-----------------------------
void DummyAARectangleXZ(Vector3D min, Vector3D max, float y){
    glPopMatrix();
    glPushMatrix();
    glColor3f(1,1,1);

    VertexObject vo;
    float verts[24];
    verts[0] = min.v[0];
    verts[1] = y;
    verts[2] = min.v[2];
    verts[3] = max.v[0];
    verts[4] = y;
    verts[5] = min.v[2];
    //---
    verts[6] = max.v[0];
    verts[7] = y;
    verts[8] = min.v[2];
    verts[9] = max.v[0];
    verts[10] = y;
    verts[11] = max.v[2];
    //---
    verts[12] = max.v[0];
    verts[13] = y;
    verts[14] = max.v[2];
    verts[15] = min.v[0];
    verts[16] = y;
    verts[17] = max.v[2];
    //---
    verts[18] = min.v[0];
    verts[19] = y;
    verts[20] = max.v[2];
    verts[21] = min.v[0];
    verts[22] = y;
    verts[23] = min.v[2];
    //---

    vo.init(verts, VPOSITION, 12*sizeof(float), false);
    vo.bind();
    glDrawArrays(GL_LINES, 0, 8);
    vo.unbind();

}
//-----------------------------
void DrawCross(Vector3D pos){
    glPushMatrix();
    glTranslatef(pos.v[0], pos.v[1], pos.v[2]);

    bool tex1disabled = false;
    bool tex2disabled = false;


    glActiveTexture(GL_TEXTURE0);
    if (glIsEnabled(GL_TEXTURE_2D)){
        tex1disabled = true;
        glDisable(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE1);
    if (glIsEnabled(GL_TEXTURE_2D)){
        tex2disabled = true;
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_LINES);
    glVertex3f(-0.5f,0,0);
    glVertex3f(0.5f,0,0);
    glVertex3f(0,0,-0.5f);
    glVertex3f(0,0,0.5f);
    glVertex3f(0,-0.5f, 0.0f);
    glVertex3f(0,0.5f, 0.0f);
    glEnd();

    if (tex1disabled){
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
    }
    if (tex2disabled){
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

