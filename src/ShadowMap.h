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


#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

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


#include "Extensions.h"



void PrepareShadowMap(int size, GLuint* shadowMap,
                      bool useFBO, GLuint* _fboID);

void DestroyShadowMap(GLuint * shadowMap,
                      bool useFBO, GLuint* _fboID);


#endif //SHADOW_MAP_H
