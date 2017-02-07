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

#include "ShadowMap.h"



void PrepareShadowMap(int size, GLuint* shadowMap,
                      bool useFBO, GLuint* _fboID){
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    *shadowMap = 0;
    glGenTextures(1, shadowMap);
    glBindTexture(GL_TEXTURE_2D, *shadowMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glDisable(GL_TEXTURE_2D);

    if (useFBO){
        GLenum FBOstatus;

        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffersEXT(1, _fboID);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *_fboID);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                              GL_TEXTURE_2D, *shadowMap, 0);

        FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
            puts("FBO fail!");

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }

    glActiveTexture(GL_TEXTURE0);

}
//---------------------------------------
void DestroyShadowMap(GLuint * shadowMap,
                      bool useFBO, GLuint* _fboID){

    glActiveTexture(GL_TEXTURE1);

    glDeleteTextures(1, shadowMap);

    if (useFBO){
        glDeleteFramebuffersEXT(1, _fboID);
    }

    glActiveTexture(GL_TEXTURE0);
}


