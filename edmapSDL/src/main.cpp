/***************************************************************************
 *   Copyright (C) 2009 by jrs0ul                                          *
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




#include <cstdlib>
#include <cstring>

#include "../../src/TextureLoader.h"
#include "../../src/Worldmap.h"
#include "../../src/Camera.h"
#include "../../src/npc.h"
#include "../../src/Utils.h"
#include "../../src/gui/Button.h"
#include "../../src/gui/Text.h"
#include "../../src/gui/EditBox.h"
#include "../../src/gui/SelectMenu.h"
#include "../../src/DebugProps.h"
#include "ViewDialog.h"
#include "TilesetInfo.h"
#include "MapHeaderDlg.h"
#include "TileTool.h"
#include "AssetBrowser.h"
#include "SDLVideo.h"
#include "Selection.h"

#include "main.h"

const float TileWidth3D = 8.0f;
const char BasePath[] = "base";

PicsContainer Pics;
PicsContainer Pics2;
SubsetCollection Tiles3D;

CTilesetInfo TilesInfo;

int MapX = 0;
int MapY = 0;

bool ShowGrid = false;
bool ShowNpc = false;


Square sq;


int oldMouseX = 0;
int oldMouseY = 0;

DCamera         Cam3D;

unsigned ActiveLayer = 0;

Vector3D POS;
Vector3D ORG;
Map GMap;


unsigned globalKEY;
bool Works = true;

unsigned char keys[30]={0};
unsigned char Oldkeys[30]={0};
int MouseX, MouseY; //relative mouse coords
int _MouseX, _MouseY;
int MouseButton, OldMouseButton;

MapHeadDlg MHD;
TileTool TT;
ViewDialog VD;

char FileName[255] = {0};
bool TilesetLoaded = false;

NpcArray NpcNation;
SubsetCollection DummyFighter;

SDLVideo SDL;
Vector3D V0;
Vector3D V1;
Vector3D V2;

bool JustSavedFile = false;
//----drag stuff
bool Drag = false;
float PlaneDistance;
Vector3D OriginPos;
Vector3D OldPos;
Vector3D NewPos;
//---------------
unsigned TileCount = 0;

//-----------------------------
float PerspectiveMatrix[16];
float OrthoMatrix[16];
//-----------------------------
bool IsPerspective = true;
//--------------------------------
EditBox XYZEdit;
int WhichAxis = -1;
bool AllSelected = false;
unsigned WhichCamParam = 0;
SelectMenu FileMenu;


Selection SelectedItems;


Vector3D MinB;
Vector3D MaxB;


//---------------------------------
void makeMouseRay(int mouseX, int mouseY,
                  float _near, float _far,
                  Vector3D & rayPos,
                  Vector3D & rayDir){

    GLint viewport[4];
    float modelviewf[16];
    float projectionf[16];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewf);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionf);

    float winx, winy;
    winx = (float)mouseX;
    winy = ( float ) viewport[3] - ( float ) mouseY;
    GLdouble posX, posY, posZ;
    GLdouble posX2, posY2, posZ2;

    
    Vector3D v1 = Unproject(winx,
                  winy,
                   _near,
                   modelviewf,
                   projectionf,
                   viewport);
   Vector3D v2 = Unproject(winx,
                           winy,
                           _far, 
                           modelviewf,
                           projectionf, viewport);
    posX = v1.v[0]; posY = v1.v[1]; posZ = v1.v[2]; 
    posX2 = v2.v[0]; posY2 = v2.v[1]; posZ2 = v2.v[2]; 

 
    rayPos.set(posX, posY, posZ);
    rayDir.set(rayPos.v[0] - posX2, rayPos.v[1] - posY2, rayPos.v[2] - posZ2);

    rayDir.normalize();

}

//---------------------------------
Vector3D PointOnPlane3D(int mouseX, int mouseY, 
                        float _near, float _far,
                        Vector3D planeNormal,
                        float planeDistance = 0.0f){

    Vector3D result;
    Vector3D rayPos;
    Vector3D rayDir;
    makeMouseRay(mouseX, mouseY,
                 _near, _far,
                 rayPos,
                 rayDir);

    Vector3D PointOnPlane = Vector3D(0,0,0);
    PointOnPlane = PointOnPlane - rayPos;

    float cosAlpha = planeNormal * rayDir;
    float t = 0.0;
    if (cosAlpha != 0.0f){
        float delta = 0.0f;
        if (planeDistance == 0.0f)
            delta = planeNormal * PointOnPlane;
        else
            delta = planeDistance + (planeNormal * PointOnPlane);
        t = delta/cosAlpha;
    }

    result = Vector3D(rayPos.v[0] + rayDir.v[0]*t,
                      rayPos.v[1] + rayDir.v[1]*t,
                      rayPos.v[2] + rayDir.v[2]*t);

    //printf("%f %f %f\n", result.v[0], result.v[1], result.v[2]);

    return result;
}
//-------------------------------------------------
static void DisplayGrid(){
    if (ShowGrid)
            ShowGrid = false;
    else
            ShowGrid = true;


}
//-------------------------------------------------
static void ShowNpcs(){
    ShowNpc = !ShowNpc;
}

//-----------------------------------------

void PickTile(unsigned x, unsigned y, unsigned layer){
    sq = GMap._getTile(x, y, layer);

    printf("tilenum %u\n", sq.tilenum);
    if (sq.tilenum > sq.count)
        sq.tilenum = 0;

    for (unsigned i = 0; i < sq.count; i++){
        sq.frames[i] -= 30;
        printf ("%u ", sq.frames[i]);
    }
    puts("");

    char buf[50];
    sprintf(buf, "%u", sq.count);

}

//---------------------------------------------------
void RightButtonDraw(unsigned mx, unsigned my, int comboindex){
        if ((mx < GMap.width()) && (my < GMap.height())){
            if ((comboindex > -1) && (comboindex < 3)){
                GMap.setTile(mx, my, comboindex, 0);
                GMap.setWalkable(mx, my, false);
            }

            if (comboindex == 3){
                GMap.setMonsterGroup(mx, my, 0);
            }

            if (comboindex == 4){
                GMap.setWalkable(mx, my, 1);
            }
        }
}
//----------------------------------------------------
void LeftButtonDraw(unsigned mx, unsigned my, int layer){

    if ((mx < GMap.width()) && (my < GMap.height())){

        //0,1,2 layers
        if ((layer > -1) && (layer < 3)){

            if (!TT.picktiles){
                Square tmp = sq;
                for (unsigned i = 0; i < tmp.count; i++){
                    tmp.frames[i] += 30;
                }
                GMap.setTile(mx, my, layer, tmp);

                if (layer < 2){
                    if (TilesetLoaded)
                        GMap.setWalkable(mx, my,
                                         TilesInfo.walkable[sq.frames[0]]);
                }
            }
            else{
                PickTile(mx, my, layer);
            }
        }
        //monster groups
        if (layer == 5){
            if (!TT.picktiles){
                GMap.setMonsterGroup(mx, my, 
                                     1);//atoi(gtk_entry_get_text(GTK_ENTRY(TT.MGEdit))));
            }
            else{
                char buf[255];
                sprintf(buf, "%u", GMap.getMonsterGroup(mx, my));
                //gtk_entry_set_text(GTK_ENTRY(TT.MGEdit), buf);
            }
        }
        //collision
        if (layer == 4){
            if (!TT.picktiles){
                GMap.setWalkable(mx, my, 0);
            }
        }

       
    } //x and y are in map

}
//-----------------------------------------------------
void OnMouseMove2D(int x, int y, int button){

    if (TT.draw3D){
        unsigned mx = 0, my = 0;
        mx = (unsigned)(x + MapX)/32;
        my = (unsigned)(y + MapY)/32;

        if (button & SDL_BUTTON(1)){ //left mouse button
            LeftButtonDraw(mx, my, ActiveLayer);
        }
        //right mouse button
        if (button & SDL_BUTTON(3)){
            RightButtonDraw(mx, my, ActiveLayer);
        }
    }
    else{
        if (button & SDL_BUTTON(1)){
            MapX += (int)((x - oldMouseX) * 0.5f);
            MapY += (int)((y - oldMouseY) * 0.5f);
        }
        oldMouseX = x;
        oldMouseY = y;
    }
}

/*    //-----------------------------
    static gint scroll_event(GtkWidget *widget,
        GdkEventScroll *event){
            if (event->direction){
                Cam3D.fly(0.5f, false);
            }
            else
                Cam3D.fly(-0.5f, false);
        return FALSE;
    }*/

//-------------------------------------------
Vector3D PointOnFarPlane(int x, int y, float distance){

    float m[16];
    GLint viewport[4];
    float modelview[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    GLfloat aspect = (GLfloat) SDL.width() / SDL.height();
    MatrixPerspective(60.0, aspect, 1.0f, distance, m);

    return Unproject(x, -y,
                     1.0f,
                     modelview,
                     m,
                     viewport);
}

//--------------------------------------------
void    CalclObjectBoundaries(const unsigned * selected,
                              unsigned long count,
                              Vector3D& min,
                              Vector3D& max){

    float maxX = 0.0f;
    float maxZ = 0.0f;
    float minX = 0.0f;
    float minZ = 0.0f;
    bool first = true;
    for (unsigned long i = 0; i < count; i++){ //for each selected object
        FlatMatrix fm;
        GMap.buildAssetMatrix(selected[i], fm);
        Matrix m;
        MatrixFlat2Matrix(fm, m);

        Model * mesh;
        Asset * a = GMap.getAsset(selected[i]);

        
        mesh = &((Tiles3D.getSubset(a->assetId))->mesh);

        Vector3D v;

        for (unsigned long a = 0; a < mesh->_vertexcount; a++){ //for each vertex
    
            v.set(mesh->vertexcoord(a * 3),
                  mesh->vertexcoord(a * 3 + 1),
                  mesh->vertexcoord(a * 3 + 2));


            v.transform(m);
            if (first){
                minX = v.v[0];
                maxX = v.v[0];
                minZ = v.v[2];
                maxZ = v.v[2];
                first = false;
            }
            else{
                if (v.v[0] > maxX)
                    maxX = v.v[0];
                if (v.v[2] > maxZ)
                    maxZ = v.v[2];
                if (v.v[0] < minX)
                    minX = v.v[0];
                if (v.v[2] < minZ)
                    minZ = v.v[2];
            }

        }
        

    }

    min = Vector3D(minX, 0, minZ);
    max = Vector3D(maxX, 0, maxZ);


}

//--------------------------------------------
void OnMouseMove3D(int x, int y, int button){

    Vector3D pos;

    pos = PointOnPlane3D(x, y, 1.0f, 500.0f, Vector3D(0,1,0) );

    if (TT.moveCameraButton.pressed){

            if (button & SDL_BUTTON(1)){
                float deltax = (x - oldMouseX) * 0.005f;
                float deltay = (y - oldMouseY) * 0.005f;
                Cam3D.pitch(deltax);
                Cam3D.yaw(deltay, false);
            }
            if (button & SDL_BUTTON(3)){
                float deltax = (x - oldMouseX) * 0.05f;
                float deltay = (y - oldMouseY) * -0.05f;
                Cam3D.move(deltay);
                Cam3D.strife(deltax);
            }
            oldMouseX = x;
            oldMouseY = y;
    }
    if (TT.placeTilesButton.pressed){//drawing

        if ((x < (int)SDL.width())&&(x >= 0)
            &&(y < (int)SDL.height()) && (y >= 0)){

            int mx, my;
            if (button & SDL_BUTTON(1)){
                mx =(unsigned)(pos.x() / TileWidth3D);
                my =(unsigned)(pos.z() / TileWidth3D);

                LeftButtonDraw(mx, my, ActiveLayer); 
            }


            if (button & SDL_BUTTON(3)){
                mx =(unsigned)(pos.x() / TileWidth3D);
                my =(unsigned)(pos.z() / TileWidth3D);
                RightButtonDraw(mx, my, ActiveLayer);
            }
        }
    }

    if (TT.moveAssetButton.pressed){
        /*some asset movement while draging one of them with the mouse:
         Mouse should point on one of the selected assets
         if Mouse button 1 not up, folow the mouse in the plane pralel to near view plane*/
        if (button & SDL_BUTTON(1)){
            if (!Drag){
                int i = -1;
                i = whichAssetUnderMouse(x, y, 1.0f, 500.0f, PlaneDistance);
                if (i > -1){
                    if (SelectedItems.isSelected((unsigned)i) != -1){
                        OldPos = PointOnFarPlane(x, y, PlaneDistance);
                        OriginPos = OldPos;
                        Drag = true;
                        CalclObjectBoundaries(SelectedItems.getArray(),
                                              SelectedItems.count(),
                                              MinB,
                                              MaxB);
                        //puts("drag");
                    }
                }
            }
            else{
                NewPos = PointOnFarPlane(x, y, PlaneDistance);
                Vector3D shift = NewPos - OldPos;
                Vector3D _shift = NewPos - OriginPos;

                if (WhichAxis > -1){//tie up on axis
                    Vector3D terminator;
                    float value = 0;

                    switch(WhichAxis){
                        case 0:{
                            terminator = Vector3D(1,0,0);
                            value = _shift.v[0];
                        }
                        break;
                        case 1:{
                            terminator = Vector3D(0,1,0);
                            value = _shift.v[1];
                        }
                        break;
                        case 2:{
                            terminator = Vector3D(0,0,1);
                            value = _shift.v[2];
                        }
                        break;
                    }

                    shift = Vector3D(shift.v[0] * terminator.v[0],
                                     shift.v[1] * terminator.v[1],
                                     shift.v[2] * terminator.v[2]);
                    _shift = Vector3D(_shift.v[0] * terminator.v[0],
                                     _shift.v[1] * terminator.v[1],
                                     _shift.v[2] * terminator.v[2]);
                    char buf[255];
                    sprintf(buf, "%f", value);
                    XYZEdit.settext(buf);

                }


                //----------
                if (VD.snapToGrid.pressed){
                    float gap = 0.25f;
                    Vector3D _MinB = MinB + shift;
                    Vector3D _MaxB = MaxB + shift;

                    int minX = (int)round(_MinB.v[0] / TileWidth3D);
                    int maxX = (int)round(_MaxB.v[0] / TileWidth3D);

                    if ((_MinB.v[0] > minX * TileWidth3D - gap) && (_MinB.v[0] < minX * TileWidth3D + gap)){
                        shift.v[0] = minX * TileWidth3D - MinB.v[0];
                    }
                    else
                        if ((_MaxB.v[0] > maxX * TileWidth3D - gap) && (_MaxB.v[0] < maxX * TileWidth3D + gap))
                            shift.v[0] = maxX * TileWidth3D - MaxB.v[0];
                    //---
                    int minZ = (int)round(_MinB.v[2] / TileWidth3D);
                    int maxZ = (int)round(_MaxB.v[2] / TileWidth3D);
                    if ((_MinB.v[2] > minZ * TileWidth3D - gap) && (_MinB.v[2] < minZ * TileWidth3D + gap))
                        shift.v[2] = minZ * TileWidth3D - MinB.v[2];
                    else
                        if ((_MaxB.v[2] > maxZ * TileWidth3D - gap) && (_MaxB.v[2] < maxZ * TileWidth3D + gap))
                            shift.v[2] = maxZ * TileWidth3D - MaxB.v[2];
                }

                //-----------
                MinB = MinB + shift;
                MaxB = MaxB + shift;

                SelectedItems.moveSelected(shift, GMap);


                OldPos = NewPos;
                //restores old position
                if (button & SDL_BUTTON(3)){
                    Drag = false;
                    _shift = Vector3D(-_shift.v[0], -_shift.v[1], -_shift.v[2]);
                    SelectedItems.moveSelected(_shift, GMap);
                    WhichAxis = -1;
                    XYZEdit.reset();
                    XYZEdit.deactivate();

                    return;
                }
            }
        }
        else{
            if (Drag){
                //puts("drop");
                Drag = false;
                WhichAxis = -1;
                XYZEdit.reset();
                XYZEdit.deactivate();
            }
        }
    }
}

//----------------------------------------------------
void MouseMove(int x, int y, int button){

    if (TT.isPointerOnTop(_MouseX, _MouseY))
        return;
    if (MHD.isPointerOnTop(_MouseX, _MouseY))
        return;

    if (VD.isPointerOnTop(_MouseX, _MouseY))
        return;

    if (!GMap.is3D())
        OnMouseMove2D(x, y, button);
    else{
        OnMouseMove3D(x, y, button);
    }  
}

//----------------------------------
void SetupGL2D(unsigned width, unsigned height){

    glDisable(GL_DEPTH_TEST);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    glOrtho(0, width, height, 0, -100.0f, 100.0);

    glMatrixMode( GL_MODELVIEW );
    glDisable(GL_LIGHTING);
    //glLoadIdentity( );
    glColor3f(1,1,1);

}
//----------------------------------
void ConfGL(){

    //glCullFace (GL_BACK); //do not draw back faces
    //glEnable (GL_CULL_FACE);
    glFrontFace(GL_CW);   //front face is clock wise
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glEnable (GL_TEXTURE_2D);
    
    glMatrixMode( GL_PROJECTION );

    glViewport (0, 0, SDL.width(), SDL.height());
    glClearColor (0.5f, 0.5f, 0.5f, 1.0);
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glLoadIdentity();

    GLfloat aspect = (GLfloat) SDL.width() / SDL.height();

    float zoom = 60.0f;
    MatrixOrtho(-zoom, zoom, -zoom, zoom,
                -500.0f, 500.0f, OrthoMatrix);
    MatrixPerspective(60.0, aspect, 1.0f, 500.0f, PerspectiveMatrix);

    if (IsPerspective)
        glMultMatrixf(PerspectiveMatrix);
    else
        glMultMatrixf(OrthoMatrix);

    glMatrixMode( GL_MODELVIEW );

    glDisable(GL_LIGHTING);

}
//--------------------------------

void DrawGrid3D(float tilewidth){

    int count = 0;

    float * vertices = 0;

    glDisable(GL_DEPTH_TEST);

    vertices = new float[GMap.width() * 6 + GMap.height() * 6 + 12];

    glBindTexture(GL_TEXTURE_2D, 0);

    for (unsigned i = 0; i < GMap.width() + 1; i++){

        vertices[i*6] = i * tilewidth;
        vertices[i*6 + 1] = 0.0f;
        vertices[i*6 + 2]   = 0.0f;

        vertices[i*6 + 3] = i * tilewidth;
        vertices[i*6 + 4] = 0.0f;
        vertices[i*6 + 5] = GMap.height() * tilewidth;

        count ++;
    }

    for (unsigned i = 0; i < GMap.height() + 1; i++){

        vertices[(GMap.width() + 1)  * 6 + i * 6    ] = 0.0f;
        vertices[(GMap.width() + 1) * 6 + i * 6 + 1] = 0.0f;
        vertices[(GMap.width() + 1) * 6 + i * 6 + 2] = i * tilewidth;

        vertices[(GMap.width() + 1) * 6 + i * 6 + 3] = GMap.width() * tilewidth;
        vertices[(GMap.width() + 1) * 6 + i * 6 + 4] = 0.0f;
        vertices[(GMap.width() + 1) * 6 + i * 6 + 5] = i * tilewidth;

        count ++;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, count*2 );
    glDisableClientState(GL_VERTEX_ARRAY);

    delete []vertices;
    glEnable(GL_DEPTH_TEST);

}
//--------------------------------
void DrawGrid2D(float tilewidth){

    int count = 0;

    float * vertices = 0;


    vertices = new float[GMap.width() * 4 + GMap.height() * 4 + 8];

    glBindTexture(GL_TEXTURE_2D, 0);

    for (unsigned i = 0; i < GMap.width() + 1; i++){

        vertices[i*4] = i * tilewidth - MapX;
        vertices[i*4+1] = 0 - MapY;

        vertices[i*4 + 2] = i * tilewidth - MapX;
        vertices[i*4 + 3] = GMap.height() * tilewidth - MapY;
        count ++;
    }

    for (unsigned i = 0; i < GMap.height() + 1; i++){

        vertices[(GMap.width() + 1) * 4 + i * 4] = 0 - MapX;
        vertices[(GMap.width() + 1) * 4 + i * 4 + 1] = i * tilewidth - MapY;

        vertices[(GMap.width() + 1) * 4 + i * 4 + 2] = GMap.width() * tilewidth - MapX;
        vertices[(GMap.width() + 1) * 4 + i * 4 + 3] = i * tilewidth - MapY;

        count ++;

    }


    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_LINES, 0, count*2 );

    glDisableClientState(GL_VERTEX_ARRAY);

    delete []vertices;
}

//----------------------------------
void Drawmap2D(){

    if (TT.showLayer0.pressed)
        GMap.draw( 0 - MapX,
                   0 - MapY,
                   Pics,
                   0,
                   0,
                   SDL.width(),
                   SDL.height()
                 );
    if (TT.showLayer1.pressed)
        GMap.draw( 0 - MapX,
                   0 - MapY,
                   Pics,
                   0,
                   1,
                   SDL.width(),
                   SDL.height()
                );


    for (unsigned a = 0; a < GMap.height(); a++){
        for (unsigned i = 0; i < GMap.width(); i++){

            if ((!GMap.isWalkable(i, a)) && (TT.showCollision.pressed)){

                int shiftX = i * 32 - MapX ;
                int shiftY = a * 32 - MapY;

                if ( ((shiftX > -32) && ( shiftX < (int)SDL.width() )) &&
                     ((shiftY > -32) && ( shiftY < (int)SDL.height() ))
                   )

                    Pics.draw(-1,
                              i * 32 - MapX,
                              a * 32 - MapY,
                              0,
                              false,
                              32.0f,
                              32.0f, 0.0f,
                              COLOR(1.0f, 0.0f, 0.0f, 0.5f),
                              COLOR(1.0f, 0.0f, 0.0f, 0.5f));
            }

            if (GMap.getMonsterGroup(i,a)){
                char buf[20];
                sprintf(buf, "%u", GMap.getMonsterGroup(i, a));
                WriteText( i * 32 - MapX,
                            a * 32 - MapY,
                            Pics,
                            1,
                            buf
                        );
            }

        }
    }


    Pics.drawBatch(0, 0, 666);
    glColor3f(1,1,1);
    if (ShowGrid)
        DrawGrid2D(32.0f);

}
//----------------------------------
void DrawNpcs3D(NpcArray& npcnation){

    glColor4f(1,1,1,1);
    for (unsigned man = 0; man < npcnation.count(); man++){
        Npc * dud = 0;
        dud = npcnation.getNpc(man);
        if (dud){
            if (dud->modelIndex < npcnation.modelCount()){
                //printf("tex %u\n", npcnation.getTextureName(man));
                dud->draw3D(npcnation.getTextureName(man),
                            npcnation.getModel(dud->modelIndex)->dud,
                            false);
            }
            else{
                dud->draw3D(0, DummyFighter, false);
            }
            glPopMatrix();
        }
    }
}

//----------------------------------
void Drawmap3D(){

    //if (IsPerspective){
        float m2[16];
        Vector3D target = Cam3D.pos() + Cam3D.dir();
        MatrixLookAt(Cam3D.pos(), target,  Cam3D.up(), m2);
        glMultMatrixf(m2);
    /*}
    else{
        glTranslatef(Cam3D.pos().v[0],Cam3D.pos().v[1],Cam3D.pos().v[2]);
    }*/

    glPushMatrix();

    if (TT.showLayer0.pressed)
        GMap.draw3DPlane(Pics, 0, 0, false, TT.showCollision.pressed);
    if (TT.showLayer1.pressed)
        GMap.draw3DPlane(Pics, 0, 1, false);
    glDisable(GL_LIGHTING);
    if (ShowGrid){
        glColor3f(1,1,1);
        DrawGrid3D(TileWidth3D);
        glEnable(GL_TEXTURE_2D);
    }


    if (ShowNpc)
        DrawNpcs3D(NpcNation);

    //----------------
    glDisable(GL_TEXTURE_2D);
    if ((WhichAxis > -1) && (XYZEdit.active())){
        glClear(GL_DEPTH_BUFFER_BIT);
        const unsigned * items = 0;
        items = SelectedItems.getArray();
        if (items){

            Vector3D pos(0, 0, 0);
            for (unsigned i = 0; i < SelectedItems.count(); i++){
                Asset* a = GMap.getAsset(items[i]);
                pos = Vector3D(pos.v[0] + a->position.v[0],
                               pos.v[1] + a->position.v[1],
                               pos.v[2] + a->position.v[2]);

            }
            pos = Vector3D(pos.v[0] / SelectedItems.count(),
                           pos.v[1] / SelectedItems.count(),
                           pos.v[2] / SelectedItems.count());

            switch(WhichAxis){
                case 0: DummyLines(Vector3D(1,0,0), pos); break;
                case 1: DummyLines(Vector3D(0,1,0), pos); break;
                case 2: DummyLines(Vector3D(0,0,1), pos); break;
            }
        }
    }
    DummyAARectangleXZ(MinB, MaxB, 0.05f);

    glEnable(GL_TEXTURE_2D);
    //----------------

    if (Tiles3D.count()){
        if (TT.showAssets.pressed){
    
            GMap.draw3DAssets(Tiles3D, Pics, 0, true);
            glClear(GL_DEPTH_BUFFER_BIT);
            GMap.draw3DAssetsSelected(Tiles3D, 
                                      SelectedItems.getArray(),
                                      SelectedItems.count());
        }
        if (TT.showLayer3D.pressed){
            Vector3D frustumCenter = CalcFrustumSphereCenter(1.0f,
                                                             100.0f,
                                                             Cam3D.pos(),
                                                             Cam3D.dir());
            GMap.draw3DTiles(frustumCenter,
                             300.0f,
                             Tiles3D, Pics, 0, 2, false, TT.showCollision.pressed);
        }
    }

    glPopMatrix();
}
//----------------------------
void DrawSavedMapDialog(){
    char buf[256];
    sprintf(buf, "Map %s saved !", FileName);
    float textScale = SDL.width() / 640.0f;
    int textWidth = strlen(buf)*(11 * textScale);

    int width = textWidth + 20;
    int x = (SDL.width() / 2) - width / 2;
    int y = (SDL.height() / 2) - 50;
    Pics.draw(-1, x, y, 0, false, width, 100, 0,
              COLOR(0.0f ,0.0f, 1.0f, 0.8f), 
              COLOR(0.0f, 0.0f, 0.0f, 0.5f));
    WriteText(x + 10, y + 50, Pics, 1, buf, textScale, SDL.height() / 480.0f);
}


//----------------------------
void Redraw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    if (GMap.is3D())
      Drawmap3D();

    glPushMatrix();
    SetupGL2D(SDL.width(), SDL.height());
    if (!GMap.is3D())
        Drawmap2D();


    TT.draw(Pics, Tiles3D, sq, _MouseX, _MouseY);
    MHD.draw(Pics);
    if (VD.active())
        VD.draw(Pics);

    if (XYZEdit.active())
        XYZEdit.draw(Pics, 1, 1, 
                     COLOR(0.5f,0.5f,0.5f,0.5f),
                     COLOR(0.5f,0.5f,0.5f,0.5f),
                     false,
                     false
                     );
    if (FileMenu.active())
        FileMenu.draw(Pics, 1, 3, 1, 0, 
                      COLOR(0.6f, 0.6f, 0.6f, 0.8f),
                      COLOR(0.6f, 0.6f, 0.6f, 0.8f),
                      false);

    if (JustSavedFile)
        DrawSavedMapDialog();
    Pics.drawBatch(0, 0, 666);
   
    //------------------------------back to 3d mode
    glEnable(GL_DEPTH_TEST);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    if (IsPerspective)
        glMultMatrixf(PerspectiveMatrix);
    else
        glMultMatrixf(OrthoMatrix);

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    //-------------------------------
    
    glFlush();

    SDL_GL_SwapBuffers( );

}


//----------------------------------
void AddFrame(){

    if (sq.count < 10){
        sq.count ++;
        sq.frames[sq.count - 1] = 0;
    }
}

//------------------------------------
void RemoveFrame(){
    if (sq.count > 0){
        sq.count --;
    }
}

//------------------------------------
void PrevFrame(){

    if (sq.tilenum > 0 ){
        sq.tilenum --;
    }
}

//------------------------------------
void NextFrame( ){

    if (sq.tilenum < sq.count - 1 ){
            sq.tilenum ++;
    }
}

    //-------------------------------------
  /*  static void ResizeMap(){

        GtkWidget *EditX;
        GtkWidget *EditY;

        resizedialog = gtk_dialog_new_with_buttons("Resize Map",
                                                  GTK_WINDOW(window),
                                                  (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
                                                  GTK_STOCK_OK,
                                                  GTK_RESPONSE_ACCEPT,
                                                  GTK_STOCK_CANCEL,
                                                  GTK_RESPONSE_REJECT,
                                                  NULL);
        gtk_widget_set_usize (GTK_WIDGET (resizedialog), 200, 200);
        gtk_window_set_resizable (GTK_WINDOW (resizedialog), FALSE);


        GtkWidget * content_area = GTK_DIALOG (resizedialog)->vbox;

        EditX = gtk_entry_new();
        EditY = gtk_entry_new();
        char buf[50];
        sprintf(buf, "%u", GMap.width());
        gtk_entry_set_text( GTK_ENTRY(EditX), buf);
        sprintf(buf, "%u", GMap.height());
        gtk_entry_set_text( GTK_ENTRY(EditY), buf);
        GtkWidget * title;
        title = gtk_label_new( "Width:");
        GtkWidget * title2;
        title2 = gtk_label_new( "Height:");



        gtk_box_pack_start(GTK_BOX(content_area), title,FALSE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(content_area), EditX,FALSE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(content_area), title2,FALSE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(content_area), EditY,FALSE, TRUE, 0);


        gtk_widget_show_all (resizedialog);


        gtk_window_set_position( GTK_WINDOW( resizedialog ),
                GTK_WIN_POS_CENTER_ON_PARENT );



        gint result = gtk_dialog_run(GTK_DIALOG(resizedialog));

        switch (result){
            case GTK_RESPONSE_ACCEPT:{
                    unsigned xx = atoi(gtk_entry_get_text(GTK_ENTRY(EditX)));
                    unsigned yy = atoi(gtk_entry_get_text(GTK_ENTRY(EditY)));

                    if ((xx > 0) && (yy > 0))
                        GMap.resize(xx, yy);

                }
            break;

        }
        gtk_widget_destroy (resizedialog);

    }*/

//---------------------------------
void TilesetLoad(const char* tilesetName){
    TilesInfo.load(tilesetName);
    Pics.destroy();
    char buff[255];

    sprintf(buff, "../%s", BasePath);
    puts(buff);
    Pics.loadFile(TilesInfo.tilesetpic, 0, 32, 32, buff);
    Pics.loadFile("charai.tga", 1, 16, 16, buff);
    Pics.loadFile("buttons.tga", 2, 32, 32);
    Pics.loadFile("smallbuttons.tga", 3, 16, 16);



    puts("load 3D tiles");
    sprintf(buff, "../%s/models/%s", BasePath, TilesInfo.tilesetmesh);
    Tiles3D.destroy();
    Tiles3D.load(buff, false);
    /*gtk_adjustment_set_value(GTK_ADJUSTMENT(TT.adjpn), 0);
    GTK_ADJUSTMENT(TT.adjpn)->lower = 0;
    GTK_ADJUSTMENT(TT.adjpn)->page_size = 1;
    GTK_ADJUSTMENT(TT.adjpn)->upper = Tiles3D.count();*/
    TilesetLoaded = true;


}


//---------------------------------------------
/*static void ChangeTiles(){

     GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new ("Open Map",
                        GTK_WINDOW(window),
                        GTK_FILE_CHOOSER_ACTION_OPEN,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                        NULL);
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "./tilesets/");
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
            char *filename;
            filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));


            TilesetLoad(filename);
            LoadTexForTT3D(TilesInfo.tilesetpic);
            TTUpdate(TT.visualArea3D, NULL);

            g_free (filename);
        }
        gtk_widget_destroy (dialog);


}*/

//---------------------------------------------
/*static void NewMap(){
    GMap.destroy();
    GMap.resize(10,10);

}*/

//------------------------------------------------
/*    static void Get3DTile(){
        TTUpdate(TT.visualArea3D, NULL);
        sq.frames[sq.tilenum] = gtk_adjustment_get_value((GtkAdjustment*)TT.adjpn);
    }
//----------------------------------------------------

static const char *ui_description =
"<ui>"
"  <menubar name='MainMenu'>"
"    <menu action='MapMenu'>"
"      <menuitem action='New'/>"
"      <menuitem action='Open'/>"
"      <menuitem action='Save'/>"
"      <menuitem action='SaveAs'/>"
"      <separator/>"
"      <menuitem action='Resize'/>"
"      <menuitem action='Header'/>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='TilesMenu'>"
"      <menuitem action='Replace'/>"
"      <menuitem action='Clear'/>"
"      <menuitem action='ChangeTileset'/>"
"    </menu>"
"    <menu action='ViewMenu'>"
"      <menuitem action='ShowGrid'/>"
"      <menuitem action='Minimap'/>"
"      <menuitem action='Collision'/>"
"      <menuitem action='ShowNpcs'/>"
"    </menu>"
"  </menubar>"
"</ui>";

static const GtkActionEntry entries[] = {
  { "MapMenu", NULL, "_Map" },
  { "New", NULL, "_New", "<control>N", "Empty map", NewMap },
  { "Open", NULL, "_Open", "<control>O", "Open a map", LoadMap },
  { "Save", NULL, "_Save", "<control>S", "Save a map", SaveMap },
  { "SaveAs", NULL, "Save _As", NULL, "Save map as", SaveMapAs },
  { "Resize", NULL, "Resize", NULL, "Resize", ResizeMap },
  { "Header", NULL, "Header", NULL, "Map header", MHDShow },
  { "Quit", NULL, "Quit", "<control>Q", "Exit the program", gtk_main_quit },
  { "TilesMenu", NULL, "_Tiles" },
  { "Replace", NULL, "_Replace", NULL, "Replace tile", MHDShow },
  { "Clear", NULL, "_Clear", NULL, "Clear some tiles", MHDShow },
  { "ChangeTileset", NULL, "Change _Tileset", NULL, "Load tileset", ChangeTiles },
  { "ViewMenu", NULL, "_View" },
};

// Toggle items
static const GtkToggleActionEntry toggle_entries[] = {
  { "ShowGrid", NULL, "Show _Grid", NULL, "Shows Grid lines", DisplayGrid, FALSE },
  { "Minimap", NULL, "Show _Minimap", NULL, "Shows mini map", NULL, FALSE },
  { "Collision", NULL, "Show _Collision", NULL, "Shows collision layer", ShowCollision, FALSE },
  { "ShowNpcs", NULL, "Show _Npcs", NULL, "Shows Npcs", ShowNpcs, FALSE }
};

    //------------------------------------------
    static void picktoggle(){

        TT.picktoggle();
    }
    //--------------------------------------------
    static gboolean CloseTT(){

        return 1;
    }
    */

//-----------------------------------------
void GLreshape(){

    glViewport(0, 0, SDL.width(), SDL.height());

    glMatrixMode( GL_PROJECTION );
    GLfloat aspect = (GLfloat) SDL.width() / SDL.height();

    glLoadIdentity();
    float zoom = 60.0f;
    MatrixOrtho(-zoom, zoom, -zoom, zoom,
                -500.0f, 500.0f, OrthoMatrix);
    MatrixPerspective(60.0, aspect, 1.0f, 500.0f, PerspectiveMatrix);
    if (IsPerspective)
        glMultMatrixf(PerspectiveMatrix);
    else
        glMultMatrixf(OrthoMatrix);

    glMatrixMode( GL_MODELVIEW );


 
}
//--------------------------------------------------------
static void process_events( void ){

    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {

        case SDL_VIDEORESIZE:{

            SDL.setMetrics((unsigned)event.resize.w,
                           (unsigned)event.resize.h);
            SDL.resetMode();

#ifdef WIN32
            ConfGL();
            if (!LoadMap(FileName))
                puts("fffffuuuuck can't load map");
#else
            GLreshape();
#endif
            MHD.set(5, SDL.height() - 105, 250, 100); 
            VD.set(SDL.width() - 205, SDL.height() - 105, 200, 100);
            XYZEdit.init(SDL.width() - 205, SDL.height() - 150, "", 50);
            unsigned itemC = TileCount;
            
            TT.set(5, 5, SDL.width() - 10, 110, itemC);
        
        } break;
        case SDL_KEYDOWN:

            globalKEY = (char)event.key.keysym.unicode;
            switch( event.key.keysym.sym ) {

                default:{}

            }
            break;
            case SDL_QUIT:{
                Works = false;
            }
        }
    }
}

//---------------------------------------
void CheckKeys(){

    Uint8 * keyz;

    keyz = SDL_GetKeyState ( NULL );


    OldMouseButton = MouseButton;
    MouseButton = SDL_GetRelativeMouseState ( &MouseX, &MouseY );
    SDL_GetMouseState(&_MouseX, &_MouseY);


    memcpy(Oldkeys, keys, 30);
    memset(keys,0, 30);

    //------------------------------
    if ( keyz[SDLK_UP] == SDL_PRESSED )
        keys[0] = 1;
    if ( keyz[SDLK_DOWN] == SDL_PRESSED )
        keys[1] = 1;
    if ( keyz[SDLK_RIGHT] == SDL_PRESSED )
        keys[3] = 1;
    if ( keyz[SDLK_LEFT] == SDL_PRESSED )
        keys[2] = 1;
    if ( keyz[SDLK_RETURN] == SDL_PRESSED )
        keys[4] = 1;
    if ( keyz[SDLK_ESCAPE] == SDL_PRESSED )
        keys[5] = 1;
    if (keyz[SDLK_DELETE] == SDL_PRESSED)
        keys[11] = 1;


    if ((keyz[SDLK_LALT] == SDL_PRESSED))
        keys[8] = 1;
    if (keyz[SDLK_LCTRL])
        keys[12] = 1;
    if (keyz[SDLK_LSHIFT])
        keys[18] = 1;

    if (keyz[SDLK_KP5])
        keys[13] = 1;
    if (keyz[SDLK_KP7])
        keys[14] = 1;
    if (keyz[SDLK_KP1])
        keys[15] = 1;
    if (keyz[SDLK_KP3])
        keys[16] = 1;

    if (keyz[SDLK_PAGEUP] == SDL_PRESSED) keys[7] = 1;
    if (keyz[SDLK_PAGEDOWN] == SDL_PRESSED) keys[9] = 1;


    if (keyz[SDLK_r] == SDL_PRESSED) keys[6] = 1;
    if (keyz[SDLK_s] == SDL_PRESSED) keys[10] = 1;
    if (keyz[SDLK_g] == SDL_PRESSED) keys[17] = 1;
    if (keyz[SDLK_d] == SDL_PRESSED) keys[19] = 1;


    if (keyz[SDLK_x] == SDL_PRESSED) keys[20] = 1;
    if (keyz[SDLK_y] == SDL_PRESSED) keys[21] = 1;
    if (keyz[SDLK_z] == SDL_PRESSED) keys[22] = 1;

    if (keyz[SDLK_a] == SDL_PRESSED) keys[23] = 1;
    if (keyz[SDLK_c] == SDL_PRESSED) keys[24] = 1;
    if (keyz[SDLK_p] == SDL_PRESSED) keys[25] = 1;

}
//-----------------------------------------------
void BuildFileMenu(){
    Lines l;
    GetFileList("base/maps", l);
    SMenu selection;
    for (unsigned i = 0; i < l.l.count(); i++){
        char ext[4];
        memcpy(ext, &l.l[i].t[strlen(l.l[i].t) - 3], 3);
        ext[3] = '\0';
        if (strcmp(ext, "wrd") == 0){
            MenuItem item;
            strncpy(item.name, l.l[i].t, 255);
            memcpy(item.value, l.l[i].t, strlen(l.l[i].t) - 4);
            item.value[strlen(l.l[i].t) - 4] = 0;
            //puts(item.value);
            selection.add(item);
        }
    }
    l.l.destroy();

    int _height = (SDL.height() / 4) * 3;

    FileMenu.init(
                  SDL.width()/2 - 200,
                  SDL.height()/2 - (_height/2),
                  selection,
                  0, _height);
    FileMenu.arrowFrames[0] = 5;
    FileMenu.arrowFrames[1] = 2;
    FileMenu.arrowFrames[2] = 3;
    FileMenu.setTitle("Open map...");
    selection.destroy();
    FileMenu.activate();
}

//-----------------------------------------------
void OnMouseUp(){
    if (TT.moveCameraButton.isPointerOnTop(_MouseX, _MouseY))
        TT.moveCameraButton_clicked();
    if (TT.placeTilesButton.isPointerOnTop(_MouseX, _MouseY))
        TT.placeTilesButton_clicked();
    if (TT.pickTilesButton.isPointerOnTop(_MouseX, _MouseY))
        TT.pickTilesButton_clicked();
    if (TT.moveAssetButton.isPointerOnTop(_MouseX, _MouseY))
        TT.moveAssetButton_clicked();
    if (TT.rotateAssetButton.isPointerOnTop(_MouseX, _MouseY))
        TT.rotateAssetButton_clicked();
    if (TT.scaleAssetButton.isPointerOnTop(_MouseX, _MouseY))
        TT.scaleAssetButton_clicked();


    TT.picktiles = (TT.pickTilesButton.pressed) ? true : false;

    if (MHD.is3D.isPointerOnTop(_MouseX, _MouseY)){
        MHD.toggle3D(GMap);
        MHD.is3D.shiftstate();
    }

    if (TT.Layer0.isPointerOnTop(_MouseX, _MouseY)){
        TT.layer0Button_clicked();
        ActiveLayer = 0;
        TT.resetAssetBrowser(TileCount);
    }
    if (TT.Layer1.isPointerOnTop(_MouseX, _MouseY)){
        TT.layer1Button_clicked();
        ActiveLayer = 1;
        TT.resetAssetBrowser(TileCount);
    }
    if (TT.Layer3D.isPointerOnTop(_MouseX, _MouseY)){
        ActiveLayer = 2;
        TT.layer3DButton_clicked();
        TT.resetAssetBrowser(Tiles3D.count());
    }
    if (TT.Collision.isPointerOnTop(_MouseX, _MouseY)){
        ActiveLayer = 4;
        TT.collisionButton_clicked();
    }
    if (TT.MonsterGroups.isPointerOnTop(_MouseX, _MouseY)){
        ActiveLayer = 5;
        TT.monsterGroupButton_clicked();
    }
    if (TT.Assets.isPointerOnTop(_MouseX, _MouseY)){
        TT.assetButton_clicked();
        ActiveLayer = 6;
        TT.resetAssetBrowser(Tiles3D.count());
    }

    if (TT.showLayer0.isPointerOnTop(_MouseX, _MouseY))
        TT.showLayer0.shiftstate();
    if (TT.showLayer1.isPointerOnTop(_MouseX, _MouseY))
        TT.showLayer1.shiftstate();
    if (TT.showLayer3D.isPointerOnTop(_MouseX, _MouseY))
        TT.showLayer3D.shiftstate();
    if (TT.showCollision.isPointerOnTop(_MouseX, _MouseY))
        TT.showCollision.shiftstate();
    if (TT.showMonsterGroups.isPointerOnTop(_MouseX, _MouseY))
        TT.showMonsterGroups.shiftstate();
    if (TT.showAssets.isPointerOnTop(_MouseX, _MouseY))
        TT.showAssets.shiftstate();

    if (TT.animAddFrame.isPointerOnTop(_MouseX, _MouseY))
        AddFrame();
    if (TT.animRemoveFrame.isPointerOnTop(_MouseX, _MouseY))
        RemoveFrame();
    if (TT.prevFrame.isPointerOnTop(_MouseX, _MouseY))
        PrevFrame();
    if (TT.nextFrame.isPointerOnTop(_MouseX, _MouseY))
        NextFrame();

    if (TT.saveButton.isPointerOnTop(_MouseX, _MouseY)){
        char mapName[512];
        sprintf(mapName, "%s.wrd", FileName);
        if (!GMap.save(mapName))
            puts("Oi! Some kind of error!");
        else
            JustSavedFile = true;
    }
    if (TT.openButton.isPointerOnTop(_MouseX, _MouseY)){
        if (!FileMenu.active()){
            BuildFileMenu();
        }
    }


    if (MHD.CHeightButton.isPointerOnTop(_MouseX, _MouseY)){
        if (!XYZEdit.active()){
            XYZEdit.activate();
            char buf[256];
            sprintf(buf, "%3.5f", MHD.getCamHeight());
            XYZEdit.settext(buf);
            WhichCamParam = 1;
        }
    }
    if (MHD.CDistanceButton.isPointerOnTop(_MouseX, _MouseY)){
        if (!XYZEdit.active()){
            XYZEdit.activate();
            char buf[256];
            sprintf(buf, "%3.5f", MHD.getCamDistance());
            XYZEdit.settext(buf);
            WhichCamParam = 2;
        }
    }

    if (MHD.CPitchButton.isPointerOnTop(_MouseX, _MouseY)){
        if (!XYZEdit.active()){
            XYZEdit.activate();
            char buf[256];
            sprintf(buf, "%3.5f", MHD.getCamPitch());
            XYZEdit.settext(buf);
            WhichCamParam = 3;
        }
    }

    if (VD.active()){

        if (VD.showGrid.isPointerOnTop(_MouseX, _MouseY)){
            DisplayGrid();
            VD.showGrid.shiftstate();
        }
        if (VD.showNpc.isPointerOnTop(_MouseX, _MouseY)){
            ShowNpcs();
            VD.showNpc.shiftstate();
        }

        if (VD.snapToGrid.isPointerOnTop(_MouseX, _MouseY)){
            VD.snapToGrid.shiftstate();
        }
    }
    int result = -1;
    result = TT.AB.onMouseUp(_MouseX, _MouseY);
    if (result > -1){
        sq.frames[sq.tilenum] = result;
        //printf("%u\n", result);
    }
}
//-----------------------------------------------
void AddAsset(int mouseX, int mouseY, float distance){

    if ((ActiveLayer == 6) && (TT.draw3D)&&(!TT.picktiles)){

        Vector3D result;
        GLint viewport[4];
        float modelview[16];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
       
        float winx, winy;
        winx = (float)mouseX;
        winy = ( float ) viewport[3] - ( float ) mouseY;

        float m[16];
        GLfloat aspect = (GLfloat) SDL.width() / SDL.height();
        MatrixPerspective(60.0, aspect, 1.0f, distance, m);

        Asset newAsset;
        newAsset.position = Unproject(winx, winy,
                                      1.0f,
                                      modelview,
                                      m,
                                      viewport);

        newAsset.scale = Vector3D(1.0f, 1.0f, 1.0f);
        newAsset.rotations = Vector3D(0.0f, 0.0f, 0.0f);
        //printf("%u\n", sq.frames[sq.tilenum]);
        newAsset.assetId =  sq.frames[sq.tilenum];
        GMap.addAsset(newAsset);
    }

}
//-----------------------------------------------
int whichAssetUnderMouse(int mouseX, int mouseY,
                         float _near, float _far,
                         float & t){

    //make ray
    Vector3D rayPos;
    Vector3D rayDir;

    GLint viewport[4];
    float modelview[16];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    float winx, winy;
    winx = (float)mouseX;
    winy = ( float ) viewport[3] - ( float ) mouseY;

    float m[16];
    GLfloat aspect = (GLfloat) SDL.width() / SDL.height();
    MatrixPerspective(60.0, aspect, -1.0f, 1.0f, m);

    rayPos = Unproject(winx, winy,
                       1.0f,
                       modelview,
                       m,
                       viewport);

    MatrixPerspective(60.0, aspect, 1.0f, 500.0f, m);

    Vector3D tmp = Unproject(winx, winy,
                             1.0f,
                             modelview,
                             m,
                             viewport);

    rayDir = tmp - rayPos;
    rayDir.normalize();

    ORG = rayPos;
    POS = tmp;
    //----------------

    float minT = 0.0;
    int minTIndex = -1;
    bool first = true;
    for (unsigned long i = 0; i < GMap.assetCount(); i++){
        Asset * a = 0;
        a = GMap.getAsset(i);
        if (a){
            Subset * s = 0;
            s = Tiles3D.getSubset(a->assetId);
            if (s){
                FlatMatrix fm;
                GMap.buildAssetMatrix(i, fm);
                Matrix m;
                MatrixFlat2Matrix(fm, m);

                t = 0.0f;

                if (RayIntersectsMesh(&s->mesh,
                                      rayDir,
                                      rayPos,
                                      500,
                                      t, m
                                      )){
                    if (first){
                        minT = t;
                        minTIndex = i;
                        first = false;
                    }
                    else{
                        if (t < minT){
                            minT = t;
                            minTIndex = i;
                        }

                    }

                }
            }
        }
    }



    t = minT;
    return minTIndex;
}

//-----------------------------------------------
bool RayIntersectsMesh(Model * mesh,
                       Vector3D rayDir,
                       Vector3D raypos,
                       float dist, 
                       float &t, Matrix m){
    Vector3D v0;
    Vector3D v1;
    Vector3D v2;

    for (unsigned long i = 0; i < mesh->faceCount();i++){

        unsigned ind = mesh->indice(i * 3);
        v0.set(mesh->vertexcoord(ind * 3),
        mesh->vertexcoord(ind * 3 + 1),
        mesh->vertexcoord(ind * 3 + 2));

        ind = mesh->indice(i * 3 + 1);
        v1.set(mesh->vertexcoord(ind * 3),
        mesh->vertexcoord(ind * 3 + 1),
        mesh->vertexcoord(ind * 3 + 2));

        ind = mesh->indice(i * 3 + 2);
        v2.set(mesh->vertexcoord(ind * 3),
        mesh->vertexcoord(ind * 3 + 1),
        mesh->vertexcoord(ind * 3 + 2));

        v0.transform(m);
        v1.transform(m);
        v2.transform(m);
        V0 = v0;
        V1 = v1;
        V2 = v2;
        //printf("%2.3f %2.3f %2.3f\n", V0.v[0], V0.v[1], V0.v[2]);


        if (CollisionRay_Traingle(raypos, rayDir,
                                  v0,v1,v2,
                                  t)){

            //puts("hit");

            if ((t>0)&&(t<dist)) //??????
                return true;
        }
    }

    return false;
}


//-----------------------------------------------
void TransformAssets(Vector3D move,
                     Vector3D rotate,
                     Vector3D scale){
    if (TT.moveAssetButton.pressed)
        SelectedItems.moveSelected(move, GMap);
    if (TT.rotateAssetButton.pressed)
        SelectedItems.rotateSelected(rotate, GMap);
    if (TT.scaleAssetButton.pressed)
        SelectedItems.scaleSelected(scale, GMap);
}
//-----------------------------------------------
void EnterValue(){
    XYZEdit.getInput((unsigned)globalKEY);
    if (globalKEY == 'z'){
        XYZEdit.reset();
        WhichAxis = 2;
    }
    if (globalKEY == 'x'){
        XYZEdit.reset();
        WhichAxis = 0;
    }
    if (globalKEY == 'y'){
        XYZEdit.reset();
        WhichAxis = 1;
    }
    if (XYZEdit.entered){

        float value = atof(XYZEdit.text());

        if (WhichCamParam == 0){
            Vector3D v;
            float zero = 0.0f;
            if (TT.scaleAssetButton.pressed)
                zero = 1.0f;
            switch(WhichAxis){
                case 0: v = Vector3D(value, zero, zero); break;
                case 1: v = Vector3D(zero, value, zero);break;
                case 2: v = Vector3D(zero, zero, value);break;
            }
            TransformAssets(v, v, v);
        }
        else{
            switch(WhichCamParam){
                case 1: {
                    GMap.camHeight = value;
                    MHD.setCamHeight(value);
                }
                break;
                case 2:{
                    GMap.camDistance = value;
                    MHD.setCamDistance(value);
                }
                break;
                case 3: {
                    GMap.camPitch = value;
                    MHD.setCamPitch(value);
                }
                break;
            }
        }

        XYZEdit.deactivate();
        XYZEdit.reset();
        WhichAxis = -1;
    }

}
//-----------------------------------------------
void ArrowKeysPressed(){
    float moveStep = 0.05f;
    float rotateStep = 0.05f;

    if (keys[12])
        rotateStep = 1.57079633f;

    float scaleStep = 0.05f;
    float zero = 0.0f;
    if (TT.scaleAssetButton.pressed)
        zero = 1.0f;
    //Z axis
    if (keys[0]){
        TransformAssets(Vector3D(zero, zero, moveStep),
                        Vector3D(zero, zero, rotateStep),
                        Vector3D(zero, zero, 1.0f + scaleStep));
    }
    if (keys[1]){
        TransformAssets(Vector3D(zero, zero, -moveStep),
                        Vector3D(zero, zero, -rotateStep),
                        Vector3D(zero, zero, 1.0f - scaleStep));
    }
    //-------
    //X axis
    if (keys[2]){
        TransformAssets(Vector3D(moveStep, zero, zero),
                        Vector3D(rotateStep, zero, zero),
                        Vector3D(1.0f + scaleStep, zero, zero));
    }
    if (keys[3]){
        TransformAssets(Vector3D(-moveStep, zero, zero),
                        Vector3D(-rotateStep, zero, zero),
                        Vector3D(1.0f - scaleStep, zero, zero));
    }
    //-------
    //Y axis
    if (keys[7]){
        TransformAssets(Vector3D(zero, moveStep, zero),
                        Vector3D(zero, rotateStep, zero),
                        Vector3D(zero, 1.0f + scaleStep, zero));
    }
    if (keys[9]){
        TransformAssets(Vector3D(zero, -moveStep, zero),
                        Vector3D(zero, -rotateStep, zero),
                        Vector3D(zero, 1.0f - scaleStep, zero));
    }

}

//-----------------------------------------------
void PickAsset(){

    int i = -1;
    float t;
    i = whichAssetUnderMouse(_MouseX, _MouseY, 1.0f, 500.0f, t);
    if (i > -1){ //selects something
        unsigned index = (unsigned)i;
        int res = SelectedItems.isSelected(index); 
        if (res == -1){
            SelectedItems.add(index);
        }
        else{
            SelectedItems.remove(res);
        }
        
    }
    else{
        SelectedItems.destroy();
    }
}

//-----------------------------------------------
void Logic(){

    if (FileMenu.active()){
        SoundSystem SS;
        if (!FileMenu.selected){
            FileMenu.getInput(globalKEY, SS, _MouseX, _MouseY);
            if (FileMenu.selected){
                char path[256];
                sprintf(path, "%s/maps/%s", BasePath, 
                        FileMenu.selection.items[FileMenu.state].value);
                LoadMap(path);
            }
        }
        else{
            FileMenu.deactivate();
            FileMenu.destroy();
        }

        return;

    }

    if (XYZEdit.active()){
        EnterValue();
        return;
    }
    //-------------------------

    if (!keys[20] && Oldkeys[20]){
        if (keys[18]){
            SelectedItems.alignSelectedToFirstX(GMap);
        }
        else{
            if (!XYZEdit.active()){
                XYZEdit.activate();
                WhichAxis = 0;
            }
        }
    }
    if (!keys[21] && Oldkeys[21]){
        if (keys[18]){
            SelectedItems.alignSelectedToFirstY(GMap);
        }
        else{

            if (!XYZEdit.active()){
                XYZEdit.activate();
                WhichAxis = 1;
            }
        }
    }
    if (!keys[22] && Oldkeys[22]){
        if (keys[18]){
            SelectedItems.alignSelectedToFirstZ(GMap);
        }
        else{
            if (!XYZEdit.active()){
                XYZEdit.activate();
                WhichAxis = 2;
            }
        }
    }
    //----------------------------------------

    if ((!keys[10]) && (Oldkeys[10])){
        if ((keys[8]) || (Oldkeys[8])) //hold alt to reset scale
            SelectedItems.setSelectedScale(Vector3D(1, 1, 1), GMap);
        else
            TT.scaleAssetButton_clicked();
    }

    if ((!keys[24]) && (Oldkeys[24])){
        TT.moveCameraButton_clicked();
    }
    if ((!keys[25]) && (Oldkeys[25])){
        TT.pickTilesButton_clicked();
    }



    if ((!keys[6]) && (Oldkeys[6])){
        if ((keys[8]) || (Oldkeys[8]))//hold alt to reset rotation
            SelectedItems.setSelectedRotation(Vector3D(0,0,0), GMap);
        else
            TT.rotateAssetButton_clicked();
    }


    if ((!keys[17]) && (Oldkeys[17]))
        TT.moveAssetButton_clicked();

    //--------------------------------
    if ((!keys[13])&&(Oldkeys[13]))
        IsPerspective = !IsPerspective;
    if ((!keys[14])&&(Oldkeys[14])){
        IsPerspective = false;
        Cam3D.setdir(Vector3D(0, -1, 0));
        Cam3D.setup(Vector3D(0, 0, -1));
        Cam3D.setright(Vector3D(1, 0, 0));
    }
        
    if ((!keys[15])&&(Oldkeys[15])){
        IsPerspective = false;
        Cam3D.setdir(Vector3D(0, 0, 1));
        Cam3D.setup(Vector3D(0, 1, 0));
        Cam3D.setright(Vector3D(1, 0, 0));
    }

    if ((!keys[16])&&(Oldkeys[16])){
        IsPerspective = false;
        Cam3D.setdir(Vector3D(1, 0, 0));
        Cam3D.setup(Vector3D(0, 1, 0));
        Cam3D.setright(Vector3D(0, 0, 1));
    }


    ArrowKeysPressed();
    //-------

    //delete
    if (keys[11])
        SelectedItems.deleteSelected(GMap);

    //select/deselect
    if ((!keys[23])&&(Oldkeys[23])){
        if (!SelectedItems.allSelected)
            SelectedItems.selectAll(GMap);
        else
            SelectedItems.destroy();
    }

    //duplicate stuff
    if ( (!keys[19]) && (Oldkeys[19])){
        if ((keys[18]) || (Oldkeys[18]))
            SelectedItems.duplicateSelected(GMap);
    }


    TT.AB.onMouseMove(_MouseX, _MouseY);
    if ((OldMouseButton & SDL_BUTTON ( 1 ) ) && (!MouseButton)){
        OnMouseUp();
        if (TT.isPointerOnTop(_MouseX, _MouseY))
            return;
        if (MHD.isPointerOnTop(_MouseX, _MouseY))
            return;
        if (VD.isPointerOnTop(_MouseX, _MouseY))
            return;
        AddAsset(_MouseX, _MouseY, 50.0f);

        //---
        if ((TT.picktiles)&&(ActiveLayer == 6))
            PickAsset();
        //----
    }
}


//-----------------------------------------------
bool LoadMap(const char * baseFileName){

    char buf[255];
    char buff[255];
    sprintf(buff, "%s", BasePath);

    char mapName[512];
    sprintf(mapName, "%s.wrd", baseFileName);

    GMap.destroy();

    if (!GMap.load(mapName)){
        puts(mapName);
        return false;
    }

    Pics.destroy();

    sprintf(buf, "%s.tga", GMap.tileset);
    Pics.loadFile(buf, 0, 32, 32, buff);

    PicData* d = Pics.getInfo(0);
    TileCount = ceilf(d->width / 32) * (d->height / 32);
    printf("TileCount: %d\n", TileCount);

    Pics.loadFile("charai.tga", 1, 16, 16, buff);
    Pics.loadFile("edmapSDL/buttons.tga", 2, 32, 32);
    Pics.loadFile("edmapSDL/smallbuttons.tga", 3, 16, 16);
    
    sprintf(buff, "%s/models/%s.rm2", BasePath, GMap.tileset);
    Tiles3D.destroy();
    Tiles3D.load(buff, false);

    NpcNation.destroy();
  
    sprintf(buf, "%s.npc", baseFileName);
    if (!NpcNation.load(buf, "base/", false))
                printf("failed while loading %s\n",buf);

    strncpy(FileName, baseFileName, 255);
    MHD.setTileset(GMap.tileset);
    MHD.setMusic(GMap.music);
    MHD.setCamPitch(GMap.camPitch);
    MHD.setCamDistance(GMap.camDistance);
    MHD.setCamHeight(GMap.camHeight);

    return true;
}


//-----------------------------------------------
int main( int   argc, char *argv[] ){

    Cam3D.setpos(Vector3D(0,5,0));
    Cam3D.setdir(Vector3D(0,0,1));
    Cam3D.setup(Vector3D(0,1,0));
    Cam3D.setright(Vector3D(-1,0,0));

    sq.count = 1;
    memset(sq.frames, 0, sizeof(unsigned)*10);


    if (!SDL.InitWindow("EdmapSDL", "edmapSDL/icon.bmp", true)){
        Works = false;
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    LoadExtensions();
    ConfGL();

    //------------dialog

    DummyFighter.load("base/models/minifighter.rm2", false);
    char path[255];

    if (argc > 1)
        sprintf(path, "%s/maps/%s", BasePath, argv[1]);
    else
        sprintf(path, "%s/maps/inn", BasePath);
    if (!LoadMap(path))
        Works = false;

    TT.set(5, 5, SDL.width()-10, 110, TileCount);
    VD.set(SDL.width() - 205, SDL.height() - 105, 200, 100);
    VD.activate();
    XYZEdit.init(SDL.width() - 205, SDL.height() - 150,"", 50);
    MHD.set(5, SDL.height() - 105, 250, 100); 
    if (GMap.is3D())
        MHD.is3D.shiftstate();


    static unsigned long tick = SDL_GetTicks();

    while (Works){


        if ((SDL_GetTicks() > tick)){
            CheckKeys();
            if (keys[0]) globalKEY = SDLK_UP;
            if (keys[1]) globalKEY = SDLK_DOWN;
            if (keys[2]) globalKEY = SDLK_LEFT;
            if (keys[3]) globalKEY = SDLK_RIGHT;
            if (keys[4]) globalKEY = SDLK_RETURN;
            if (keys[5]) globalKEY = SDLK_ESCAPE;
            if (keys[6]) globalKEY = SDLK_SPACE;

            if (JustSavedFile){
                if ((OldMouseButton & SDL_BUTTON ( 1 ) ) && (!MouseButton)){
                    JustSavedFile = false;
                }
            }
            else{
                MouseMove(_MouseX, _MouseY, MouseButton);
                Logic();
            }
            Redraw();
            globalKEY = 0;

            tick = SDL_GetTicks() + 10;


        }
        SDL_Delay(1);

        process_events();
    }


    GMap.destroy();
    Tiles3D.destroy();
    NpcNation.destroy();
    DummyFighter.destroy();
    TT.destroy();
    SelectedItems.destroy();

    SDL.Quit();
    return 0;
}



