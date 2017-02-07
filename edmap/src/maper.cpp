#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <gdk/gdk.h>



#ifdef WIN32
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
#else
    #include <GL/glx.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    //#include <GL/glu_mangle.h>

#endif


#include <cstdlib>
#include <cstring>

#include "../../src/TextureLoader.h"
#include "../../src/Worldmap.h"
#include "../../src/Camera.h"
#include "TilesetInfo.h"
#include "MapHeaderDlg.h"
#include "TileTool.h"
#include "gtkUtils.h"

#include "mapper.h"


    const char BasePath[] = "base";

    PicsContainer Pics;
    PicsContainer Pics2;
    SubsetCollection Tiles3D;

    CTilesetInfo TilesInfo;


    GdkGLContext *GLContext;
    GdkGLDrawable *GLDrawable;
    GtkWidget *window;
    GtkWidget* menubar;
    GtkWidget *main_vbox;
    GtkWidget *main_hbox;
    GtkWidget *DrawingArea;


    GtkWidget *VScrollbar;
    GtkWidget *HScrollbar;
    GtkObject *adjWidth;
    GtkObject *adjHeight;

    GtkWidget *resizedialog;


    bool ShowGrid = false;
    bool Collision = false;


    Square sq;

    //unsigned pickedTile = 0;

    int oldMouseX = 0;
    int oldMouseY = 0;

    //Vector3D ObjectPos = Vector3D(0.0f,0.0f,0.0f);
    //float    ObjectSize = 1.0f;
    //Vector3D ObjectRot = Vector3D(0.0f,0.0f,0.0f);
    //unsigned ObjectFrame = 0;


    DCamera         Cam3D;


    Vector3D POS;
    Vector3D rayPos;
    Map GMap;


    MapHeadDlg MHD;
    TileTool TT;

    char * FileName = 0;


 //----------------------------------------------
   static void UpdateToolAnim( GtkWidget *widget, gpointer   data ){

        TT.update_anim(widget, sq.frames[sq.tilenum]);
    }

//---------------------------------
    void WriteText(unsigned int x,unsigned int y, PicsContainer& pics, int font, const char* s,
        float alpha = 1.0f, float scalex = 1.0f, float scaley = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f){

         //puts(s);
        for (unsigned int i = 0; i < strlen(s); i++){
            pics.draw(font, x+11*i, y, s[i]-32, false, scalex, scaley, 0.0f, COLOR(alpha, r, g, b));
        }

    }

//----------------------------------
    Vector3D Unproject(float x, float y, float z, 
                       float * modelview, float* projection,
                       int * viewport){

        Vector3D in, out;

        in = Vector3D((x - viewport[0]) * 2 / viewport[2] - 1.0f,
            (y - viewport[1]) * 2 / viewport[3] - 1.0f,
            2 * z - 1.0f);

        FlatMatrix mw;
        FlatMatrix proj;
        memcpy(mw.m, modelview, sizeof(float) * 16);
        memcpy(proj.m, projection, sizeof(float) * 16);

        Matrix modw, project, a, inv;
        MatrixFlat2Matrix(mw, modw);
        MatrixFlat2Matrix(proj, project);

        MatrixMultiply(a, project, modw);
        MatrixInverse(a, inv);
        in.transform(inv);

        if (in.v[3] != 0.0f){
            out.v[0] = in.v[0]/in.v[3];
            out.v[1] = in.v[1]/in.v[3];
            out.v[2] = in.v[2]/in.v[3];
        }
        
        return out; 
    }

  //------------------------------------------
    void LoadTexForTT3D(){

        GdkGLDrawable * glD = TT.getGLDrawable();
        GdkGLContext * glC = TT.getGLContext();
        gdk_gl_drawable_gl_begin(glD, glC);
        Pics2.destroy();
        Pics2.loadFile(TilesInfo.tilesetpic, 0, 32, "../base/");
        gdk_gl_drawable_gl_end(glD);

    }



//---------------------------------
    Vector3D PointOnPlane3D(int mouseX, int mouseY, float _near, float _far, Vector3D plane){

        Vector3D result;
        GLint viewport[4];
        double modelview[16];
        float modelviewf[16];
        double projection[16];
        float projectionf[16];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetFloatv(GL_MODELVIEW_MATRIX, modelviewf);
        glGetFloatv(GL_PROJECTION_MATRIX, projectionf);
            
        float winx, winy;
        winx = (float)mouseX;
        winy = ( float ) viewport[3] - ( float ) mouseY;
        GLdouble posX, posY, posZ;
        GLdouble posX2, posY2, posZ2;
            
        gluUnProject( (GLdouble)winx,
                      (GLdouble)winy,
                       _near,
                      (const GLdouble *)modelview,
                      (const GLdouble *)projection,
                       viewport,
                       &posX, &posY, &posZ);
        //Vector3D rayPos = Unproject(winx,winy,near, modelviewf, projectionf, viewport);
        gluUnProject( (GLdouble)winx,
                      (GLdouble)winy,
                      _far, 
                      modelview, projection, viewport, &posX2, &posY2, &posZ2);

        Vector3D rayPos;
        Vector3D ray;
        rayPos.set(posX, posY, posZ);
        ray.set(rayPos.v[0] - posX2, rayPos.v[1] - posY2, rayPos.v[2] - posZ2);

        ray.normalize();

        Vector3D PointOnPlane = Vector3D(0,0,0);
        PointOnPlane = PointOnPlane - rayPos;
        float cosAlpha = plane * ray;
        float t = 0.0;
        if (cosAlpha != 0.0f){
            float delta = plane * PointOnPlane;
            t = delta/cosAlpha;
        }

        result = Vector3D(rayPos.v[0] + ray.v[0]*t,
                          rayPos.v[1] + ray.v[1]*t,
                          rayPos.v[2] + ray.v[2]*t);

        return result;
    }



//---------------------------------
static void MHDaction(){


    MHD.getTileset(GMap.tileset);
    MHD.getMusic(GMap.music);
    printf("is3D: %u\n", (GMap.isMap3D)?1:0);
    GMap.isMap3D = (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(MHD.is3D)) == TRUE) ? true : false;
    printf("is3D: %u\n", (GMap.isMap3D)?1:0);
    if (!GMap.isMap3D)
        SetupAdjustments();

    MHD.hide();

}
//------------------------------------
static void MHDDestroy(){
    MHD.build();
    MHD.setTileset(GMap.tileset);
    MHD.setMusic(GMap.music);
    g_signal_connect (G_OBJECT(MHD.ok_button), "clicked", G_CALLBACK(MHDaction), NULL);
    g_signal_connect (G_OBJECT (MHD.dialog), "destroy", G_CALLBACK (MHDDestroy), NULL);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(MHD.is3D), (GMap.isMap3D) ? TRUE : FALSE );
   /* gtk_signal_connect (GTK_OBJECT (MHD.is3D),"toggled",G_CALLBACK(toggle3D),
                            NULL);*/




}
//-----------------------------------
static void MHDShow(){
    MHD.show();

}
//----------------------------------------------


static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data ){

    puts("Deleting pics...");
    Pics.destroy();



    return FALSE;
}
//-------------------------------------------------
    static void DisplayGrid(){
        if (ShowGrid)
            ShowGrid = false;
        else
            ShowGrid = true;


    }
//-------------------------------------------------
     static void ShowCollision(){
        if (Collision)
            Collision = false;
        else
            Collision = true;


    }

//-------------------------------------------------

    static gboolean TTMouseClick( GtkWidget *widget, GdkEventButton *event ){
        if (event->button == 1){
            unsigned x = 0, y = 0;
            int mx = event->x + (int)GTK_ADJUSTMENT(TT.adj)->value;
            int my = event->y + (int)GTK_ADJUSTMENT(TT.adjv)->value;
            x = (unsigned)mx/32;
            y = (unsigned)my/32;

            if (
                ( mx < gdk_pixbuf_get_width (TT.pixbuf))
                &&
                 ( my < gdk_pixbuf_get_height (TT.pixbuf))
               ){
                char buf[100];
                sprintf(buf, "Tiles X:%d Y:%d", x, y);
                gtk_window_set_title(GTK_WINDOW(TT.dialog), buf);

                sq.frames[sq.tilenum] = y*(gdk_pixbuf_get_width (TT.pixbuf)/32) + x;

                UpdateToolAnim(TT.anim_DA, NULL);


            }
        }


        return TRUE;
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

    UpdateToolAnim(TT.anim_DA, NULL);
    char buf[50];
    sprintf(buf, "%u", sq.count);
    gtk_label_set_text(GTK_LABEL(TT.anim_count), buf);


}

    //-------------------------------------------------
    void OnMBPress2D(unsigned mx, unsigned my, GdkEventButton *event){

        int comboindex = gtk_combo_box_get_active(GTK_COMBO_BOX(TT.LayerSelect));
        if (event->button == 1){
            unsigned x = 0, y = 0;
            x = mx/32;
            y = my/32;
            LeftButtonDraw(x, y, comboindex);
        }//button 1

        if (event->button == 3){
            unsigned x = 0, y = 0;
            x = mx/32;
            y = my/32;
            RightButtonDraw(x, y, comboindex);
        }
    }
    //---------------------------------------------------


static gboolean button_press_event( GtkWidget *widget, GdkEventButton *event ){

    int mx = 0, my = 0;
    mx = (event->x + (int)GTK_ADJUSTMENT(adjWidth)->value);
    my = (event->y + (int)GTK_ADJUSTMENT(adjHeight)->value);



    if (!GMap.isMap3D)
        OnMBPress2D(mx, my, event);
    else{

        if ((TT.draw3D)&&(mx < widget->allocation.width)&&(mx >= 0)
                        &&(my < widget->allocation.height) && (my >= 0)){

            int comboindex = gtk_combo_box_get_active(GTK_COMBO_BOX(TT.LayerSelect));
            POS = PointOnPlane3D(event->x, event->y, 1.0f, 500.0f, Vector3D(0,1,0) );

            char buf[255];
            sprintf(buf, "%s - Map Editor X:%d Y:%d lol",
                    GMap.name,
                    (int)(POS.x()/8.0f), (int)(POS.z()/8.0f)
                    );
            //puts(buf);

            if (event->button ==  1){
                mx =(unsigned)(POS.x()/8.0f);
                my =(unsigned)(POS.z()/8.0f);
                LeftButtonDraw(mx, my, comboindex);
            }

            if (event->button ==  3){
                mx =(unsigned)(POS.x()/8.0f);
                my =(unsigned)(POS.z()/8.0f);
                RightButtonDraw(mx, my, comboindex);
            }

        }
    }
   
  return TRUE;
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
    void LeftButtonDraw(unsigned mx, unsigned my, int comboindex){

        if ((mx < GMap.width()) && (my < GMap.height())){

            //first and second layers
            if ((comboindex > -1) && (comboindex < 3)){

                if (!TT.picktiles){
                    Square tmp = sq;
                    for (unsigned i = 0; i < tmp.count; i++){
                        tmp.frames[i] += 30;
                    }
                    GMap.setTile(mx, my, comboindex, tmp);

                    if (comboindex < 2)
                        GMap.setWalkable(mx, my, TilesInfo.walkable[sq.frames[0]]);
                }
                else{
                    PickTile(mx, my, comboindex);
                }
            }
            //monster groups
            if (comboindex == 3){
                if (!TT.picktiles){
                    GMap.setMonsterGroup(mx, my, 
                                         atoi(gtk_entry_get_text(GTK_ENTRY(TT.MGEdit))));
                }
                else{
                    char buf[255];
                    sprintf(buf, "%u", GMap.getMonsterGroup(mx, my));
                    gtk_entry_set_text(GTK_ENTRY(TT.MGEdit), buf);
                }
            }
            //collision
            if (comboindex == 4){
                if (!TT.picktiles){
                    GMap.setWalkable(mx, my, 0);
                }
            }
        } //x and y are in map

    }
//-----------------------------------------------------
    void OnMouseMove2D(int x, int y, GdkModifierType& state){

        char buf[100];
        sprintf(buf, "%s - Map Editor X:%d Y:%d",
                GMap.name,
                (x + (int)GTK_ADJUSTMENT(adjWidth)->value)/32,
                (y + (int)GTK_ADJUSTMENT(adjHeight)->value)/32);
        gtk_window_set_title(GTK_WINDOW(window), buf);

        unsigned mx = 0, my = 0;
        mx = (unsigned)(x + (int)GTK_ADJUSTMENT(adjWidth)->value)/32;
        my = (unsigned)(y + (int)GTK_ADJUSTMENT(adjHeight)->value)/32;

        //which layer we shall use ?
        int comboindex = gtk_combo_box_get_active(GTK_COMBO_BOX(TT.LayerSelect));

        if (state & GDK_BUTTON1_MASK){ //left mouse button
            LeftButtonDraw(mx, my, comboindex);
        }
        //right mouse button
        if (state & GDK_BUTTON3_MASK){
            RightButtonDraw(mx, my, comboindex);
        }
    }

    //-----------------------------
    static gint scroll_event(GtkWidget *widget,
        GdkEventScroll *event){
            if (event->direction){
                Cam3D.fly(0.5f, false);
            }
            else
                Cam3D.fly(-0.5f, false);
        return FALSE;
    }

    //--------------------------------------------
    void OnMouseMove3D(int x, int y, GdkModifierType& state, GtkWidget *control){

        POS = PointOnPlane3D(x, y, 1.0f, 500.0f, Vector3D(0,1,0) );

        char buf[255];
        sprintf(buf, "%s - Map Editor X:%d Y:%d",
                GMap.name,
                (int)(POS.x()/8.0f), (int)(POS.z()/8.0f)
                );
        gtk_window_set_title(GTK_WINDOW(window), buf);

        if (!TT.draw3D){

            if (state & GDK_BUTTON1_MASK){
                float deltax = (x - oldMouseX) * 0.005f;
                float deltay = (y - oldMouseY) * 0.005f;
                Cam3D.pitch(deltax);
                Cam3D.yaw(deltay, false);
            }
            if (state & GDK_BUTTON3_MASK){
                float deltax = (x - oldMouseX) * 0.05f;
                float deltay = (y - oldMouseY) * -0.05f;
                Cam3D.move(deltay);
                Cam3D.strife(deltax);
            }
            oldMouseX = x;
            oldMouseY = y;
        }
        else{//drawing

            if ((x < control->allocation.width)&&(x >= 0)
                &&(y < control->allocation.height) && (y >= 0)){

                int mx, my;
                int comboindex = gtk_combo_box_get_active(GTK_COMBO_BOX(TT.LayerSelect));
                if (state & GDK_BUTTON1_MASK){
                    mx =(unsigned)(POS.x()/8.0f);
                    my =(unsigned)(POS.z()/8.0f);

                    LeftButtonDraw(mx, my, comboindex); 
                }


                if (state & GDK_BUTTON3_MASK){
                    mx =(unsigned)(POS.x()/8.0f);
                    my =(unsigned)(POS.z()/8.0f);
                    RightButtonDraw(mx, my, comboindex);
                }
            }

        }
    }



//----------------------------------------------------
    static gboolean motion_notify_event( GtkWidget *widget, GdkEventMotion *event ){

        int x, y;
        GdkModifierType state;

        if (event->is_hint)
            gdk_window_get_pointer (event->window, &x, &y, &state);
        else{
            x = event->x;
            y = event->y;
            state = (GdkModifierType)event->state;

        }


        if (!GMap.isMap3D)
            OnMouseMove2D(x, y, state);
        else{
            OnMouseMove3D(x, y, state, widget);
        }  
        return TRUE;
    }



//-----------------------------------------------------
static void destroy( GtkWidget *widget, gpointer   data ){
    gtk_main_quit ();
}

//-------------------------------
void SetupAdjustments(){
    gtk_adjustment_set_value(GTK_ADJUSTMENT(adjHeight), 0);
    GTK_ADJUSTMENT(adjHeight)->lower = 0;
    GTK_ADJUSTMENT(adjHeight)->upper = GMap.height() * 32;
    GTK_ADJUSTMENT(adjHeight)->page_size = DrawingArea->allocation.height;

    gtk_adjustment_set_value(GTK_ADJUSTMENT(adjWidth), 0);
    GTK_ADJUSTMENT(adjWidth)->lower = 0;
    GTK_ADJUSTMENT(adjWidth)->upper = GMap.width() * 32;
    GTK_ADJUSTMENT(adjWidth)->page_size = DrawingArea->allocation.width;

}
//----------------------------------
    void SetupGL2D(GtkWidget *DA){
        GLContext = gtk_widget_get_gl_context(DA);
        GLDrawable = gtk_widget_get_gl_drawable(DA);

        gdk_gl_drawable_gl_begin(GLDrawable, GLContext);
        glDisable(GL_DEPTH_TEST);
        
        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
        glLoadIdentity( );

        gluOrtho2D (0.0, (GLdouble) DA->allocation.width,  (GLdouble) DA->allocation.height, 0.0);

        glMatrixMode( GL_MODELVIEW );
        glDisable(GL_LIGHTING);
        glColor3f(1,1,1);
        gdk_gl_drawable_gl_end(GLDrawable);

    }
    
    //----------------------------------
    static void ConfGL(GtkWidget *DA){

        GLContext = gtk_widget_get_gl_context(DA);
        GLDrawable = gtk_widget_get_gl_drawable(DA);


        if (!GMap.isMap3D)
            SetupAdjustments();

        puts("Init...");
        gdk_gl_drawable_gl_begin(GLDrawable, GLContext);
        glEnable (GL_BLEND);
        glEnable (GL_TEXTURE_2D);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        glMatrixMode( GL_PROJECTION );
        GLfloat aspect = (GLfloat) DA->allocation.width / DA->allocation.height;

        glViewport (0, 0, DA->allocation.width, DA->allocation.height);
        glLoadIdentity();
        float m[16];
        MatrixPerspective(60.0, aspect, 1.0f, 500.0f, m);
        glMultMatrixf(m);

        glMatrixMode( GL_MODELVIEW );

        //glEnable(GL_RESCALE_NORMAL);
        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

        gdk_gl_drawable_gl_end(GLDrawable);
    }
    //--------------------------------
      static void TTConfGL(GtkWidget *DA){

        puts("TTConf");
        TT.initGLControl(DA);

        GdkGLDrawable * glD = TT.getGLDrawable();
        GdkGLContext * glC = TT.getGLContext();

        gdk_gl_drawable_gl_begin(glD, glC);
        glEnable (GL_BLEND);
        glEnable (GL_TEXTURE_2D);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode( GL_PROJECTION );
        GLfloat aspect = (GLfloat) DA->allocation.width / DA->allocation.height;

        glViewport (0, 0, DA->allocation.width, DA->allocation.height);
        glLoadIdentity();
        float m[16];
        MatrixPerspective(60.0, aspect, 1.0f, 500.0f, m);
        glMultMatrixf(m);

        glMatrixMode( GL_MODELVIEW );

        glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

        gdk_gl_drawable_gl_end(glD);
    }



    //--------------------------------
    static gboolean UpdateGL(gpointer user_data){
        GtkWidget *da = GTK_WIDGET (user_data);

        gdk_window_invalidate_rect (da->window, &da->allocation, FALSE);    
        gdk_window_process_updates (da->window, FALSE);

        return TRUE;
    }
    //--------------------------------
    static gboolean TTUpdateGL(gpointer user_data){

        GtkWidget *da = 0;
        da = GTK_WIDGET (user_data);

        if (da){

            gdk_window_invalidate_rect (da->window, &da->allocation, FALSE);    
            gdk_window_process_updates (da->window, FALSE);
        }

        return TRUE;
    }


    //--------------------------------
    void DrawGrid3D(float tilewidth){

         int count = 0;

         float * vertices = 0;

         glDisable(GL_DEPTH_TEST);


         vertices = new float[GMap.width() * 6 + GMap.height() * 6];

         glBindTexture(GL_TEXTURE_2D, 0);

         for (unsigned i = 0; i < GMap.width(); i++){

            vertices[i*6] = i * tilewidth;
            vertices[i*6 + 1] = 0.0f;
            vertices[i*6 + 2]   = 0.0f;

            vertices[i*6 + 3] = i * tilewidth;
            vertices[i*6 + 4] = 0.0f;
            vertices[i*6 + 5] = GMap.height() * tilewidth;

            count ++;
         }

         for (unsigned i = 0; i < GMap.height(); i++){

            vertices[GMap.width() * 6 + i * 6    ] = 0.0f;
            vertices[GMap.width() * 6 + i * 6 + 1] = 0.0f;
            vertices[GMap.width() * 6 + i * 6 + 2] = i * tilewidth;

            vertices[GMap.width() * 6 + i * 6 + 3] = GMap.width() * tilewidth;
            vertices[GMap.width() * 6 + i * 6 + 4] = 0.0f;
            vertices[GMap.width() * 6 + i * 6 + 5] = i * tilewidth;

            count ++;


         }


         //glLoadIdentity();
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


         vertices = new float[GMap.width() * 4 + GMap.height() * 4];

         glBindTexture(GL_TEXTURE_2D, 0);

         for (unsigned i = 0; i < GMap.width(); i++){

            vertices[i*4] = i * tilewidth - (int)GTK_ADJUSTMENT(adjWidth)->value;
            vertices[i*4+1] = 0;

            vertices[i*4 + 2] = i * tilewidth - (int)GTK_ADJUSTMENT(adjWidth)->value;
            vertices[i*4 + 3] = GMap.height() * tilewidth;

            count ++;
         }

         for (unsigned i = 0; i < GMap.height(); i++){

            vertices[GMap.width()*4 + i*4] = 0;
            vertices[GMap.width()*4 + i*4+1] = i * tilewidth - (int)GTK_ADJUSTMENT(adjHeight)->value;

            vertices[GMap.width() * 4 + i*4 + 2] = GMap.width() * tilewidth;
            vertices[GMap.width() * 4 + i*4 + 3] = i * tilewidth - (int)GTK_ADJUSTMENT(adjHeight)->value;

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

        GMap.draw( 0 - (int)GTK_ADJUSTMENT(adjWidth)->value,
                   0 - (int)GTK_ADJUSTMENT(adjHeight)->value,
                   Pics,
                   0,
                   0,
                   DrawingArea->allocation.width,
                   DrawingArea->allocation.height
                 );
        GMap.draw( 0 - (int)GTK_ADJUSTMENT(adjWidth)->value,
                   0 - (int)GTK_ADJUSTMENT(adjHeight)->value,
                   Pics,
                   0,
                   1,
                   DrawingArea->allocation.width,
                   DrawingArea->allocation.height
                 );


        for (unsigned a = 0; a < GMap.height(); a++){
            for (unsigned i = 0; i < GMap.width(); i++){

                if ((!GMap.isWalkable(i, a)) && (Collision)){

                    int shiftX = i * 32 - (int)GTK_ADJUSTMENT(adjWidth)->value ;
                    int shiftY = a * 32 - (int)GTK_ADJUSTMENT(adjHeight)->value ;

                    if ( ((shiftX > -32) && ( shiftX < DrawingArea->allocation.width )) &&
                         ((shiftY > -32) && ( shiftY < DrawingArea->allocation.height ))
                       )

                        Pics.draw(-1,
                                    i * 32 - (int)GTK_ADJUSTMENT(adjWidth)->value,
                                    a * 32 - (int)GTK_ADJUSTMENT(adjHeight)->value,
                                    0,
                                    false,
                                    32.0f,
                                    32.0f, 0.0f, COLOR(1.0f, 0.0f, 0.0f, 0.5f), COLOR(1.0f, 0.0f, 0.0f, 0.5f));
                }

                if (GMap.getMonsterGroup(i,a)){
                    char buf[20];
                    sprintf(buf, "%u", GMap.getMonsterGroup(i, a));
                    WriteText( i * 32 - (int)GTK_ADJUSTMENT(adjWidth)->value,
                               a * 32 - (int)GTK_ADJUSTMENT(adjHeight)->value,
                               Pics,
                               1,
                               buf
                            );
                }

            }
        }


        Pics.drawBatch(0, 0);
        glColor3f(1,1,1);
        if (ShowGrid)
            DrawGrid2D(32);

    }
    //----------------------------------
    void Drawmap3D(){

        float m2[16];
        Vector3D target = Cam3D.pos() + Cam3D.dir();
        MatrixLookAt(Cam3D.pos(), target,  Cam3D.up(), m2);
        glMultMatrixf(m2);

        glPushMatrix();

        GMap.draw3D(Pics, 0, 0, Collision);
        GMap.draw3D(Pics, 0, 1);
        glDisable(GL_LIGHTING);
        if (ShowGrid){
            glColor3f(1,1,1);
            DrawGrid3D(8.0f);
        }

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINES);
        glColor3f(1,1,1);
        glVertex3f(rayPos.v[0],rayPos.v[1], rayPos.v[2]);
        glVertex3f(POS.v[0], POS.v[1], POS.v[2]);
        glEnd();
        glEnable(GL_TEXTURE_2D);
       
        if (Tiles3D.count())
            GMap.draw3DTileset(Tiles3D, Pics, 0, 2, false, Collision);

        glPopMatrix();
            
    }


    //----------------------------
    void Redraw(){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        if (GMap.isMap3D)
            Drawmap3D();

        SetupGL2D(DrawingArea);
        if (!GMap.isMap3D)
            Drawmap2D();
        //------------------------------back to 3d mode
        glEnable(GL_DEPTH_TEST);
        glMatrixMode (GL_PROJECTION);
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );
        //-------------------------------

        glFlush();

        gdk_gl_drawable_swap_buffers(GLDrawable);
    }
    //------------------------------------
      void TTRedraw(){
       // puts("TT redraw");
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        float m2[16];
        MatrixLookAt(Vector3D(0,15.0f,15.0f), Vector3D(0,0,0), Vector3D(0,1,0), m2);
        glMultMatrixf(m2);

        glPushMatrix();
        
        //glColor3f(1,1,1);
        //glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Pics.getname(0));

        Tiles3D.draw((unsigned)gtk_adjustment_get_value((GtkAdjustment*)TT.adjpn), 0, false);

        glPopMatrix();

        glFlush();
        
        GdkGLDrawable * glD = TT.getGLDrawable();
        gdk_gl_drawable_swap_buffers(glD);
    }


    //----------------------------------

    static void Update( GtkWidget *widget, gpointer   data ){

        gdk_gl_drawable_gl_begin(GLDrawable, GLContext);
        Redraw();
        gdk_gl_drawable_gl_end(GLDrawable);
    }
    //-----------------------------------
     static void TTUpdate( GtkWidget *widget, gpointer   data ){

        if (widget){
            GdkGLDrawable * glD = TT.getGLDrawable();
            GdkGLContext * glC = TT.getGLContext();
            gdk_gl_drawable_gl_begin(glD, glC);
            TTRedraw();
            gdk_gl_drawable_gl_end(glD);
        }
    }


    //------------------------------------
    static void AddFrame( GtkWidget *widget, gpointer   data ){

        if (sq.count < 10){
            sq.count ++;
            sq.frames[sq.count - 1] = 0;
            char buf[50];
            sprintf(buf, "%u", sq.count);
            gtk_label_set_text(GTK_LABEL(TT.anim_count), buf);
        }
    }


//------------------------------------
    static void RemoveFrame( GtkWidget *widget, gpointer   data ){
        if (sq.count > 0){
            sq.count --;
            char buf[50];
            sprintf(buf, "%u", sq.count);
            gtk_label_set_text(GTK_LABEL(TT.anim_count), buf);
        }

    }

//------------------------------------
    static void PrevFrame( GtkWidget *widget, gpointer   data ){

        if (sq.tilenum > 0 ){
            sq.tilenum --;
            char buf[50];
            sprintf(buf, "%u", sq.tilenum);
            gtk_label_set_text(GTK_LABEL(TT.anim_current), buf);

            UpdateToolAnim(TT.anim_DA, NULL);
        }

    }

//------------------------------------
    static void NextFrame( GtkWidget *widget, gpointer   data ){

        if (sq.tilenum < sq.count - 1 ){
            sq.tilenum ++;
            char buf[50];
            sprintf(buf, "%u", sq.tilenum);
            gtk_label_set_text(GTK_LABEL(TT.anim_current), buf);

            UpdateToolAnim(TT.anim_DA, NULL);
        }
    }


//------------------------------------
    static void UpdateTool( GtkWidget *widget, gpointer   data ){

        TT.update(widget);
    }

//-----------------------------------

    static void ResizeTool( GtkWidget *widget, gpointer   data ){

        TT.resize(widget);
    }



    //-------------------------------------
    static void ResizeMap(){

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

    }

//----------------------------------
    static void SaveMapAs(){
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new ("Save Map as",
                                               GTK_WINDOW(window),
                                               GTK_FILE_CHOOSER_ACTION_SAVE,
                                               GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                               GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                               NULL);
        gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
        GtkFileFilter *map_filter = gtk_file_filter_new(); 
        gtk_file_filter_set_name(map_filter, "Map files");
        gtk_file_filter_add_pattern(map_filter,"*.wrd");

        GtkFileFilter *all_filter = gtk_file_filter_new(); 
        gtk_file_filter_set_name(all_filter, "All files");
        gtk_file_filter_add_pattern(all_filter,"*");


        char path[255];
        sprintf(path, "../%s/maps/", BasePath);

        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), path);
        gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "untitled.wrd");

        //gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
            
            FileName = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
            GMap.save(FileName);
         }
        gtk_widget_destroy (dialog);

    }
//-------------------------------
    static void SaveMap(){

        if (FileName){
            GMap.save(FileName);
        }
        else
            SaveMapAs();

    }

//---------------------------------
    static void LoadMap(){

        GtkWidget *dialog;
        char path[255];
        sprintf(path, "../%s/maps/", BasePath);
        dialog = gtk_file_chooser_dialog_new ("Open Map",
                        GTK_WINDOW(window),
                        GTK_FILE_CHOOSER_ACTION_OPEN,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                        NULL);
        GtkFileFilter *map_filter = gtk_file_filter_new(); 
        gtk_file_filter_set_name(map_filter, "Map files");
        gtk_file_filter_add_pattern(map_filter,"*.wrd");

        GtkFileFilter *all_filter = gtk_file_filter_new(); 
        gtk_file_filter_set_name(all_filter, "All files");
        gtk_file_filter_add_pattern(all_filter,"*");

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), map_filter); 
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), all_filter); 
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), path);
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){

            if (FileName)
                g_free(FileName);

            FileName = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
            GMap.destroy();
            GMap.load(FileName);
            SetupAdjustments();
            MHD.setTileset(GMap.tileset);
            MHD.setMusic(GMap.music);
            if (GMap.isMap3D == true){
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(MHD.is3D)) == FALSE)
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(MHD.is3D),TRUE);
            }
            else{
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(MHD.is3D)) == TRUE)
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(MHD.is3D), FALSE);
                puts("this map is 2d");
            }
        }
        gtk_widget_destroy (dialog);
    }

//---------------------------------
    static void TileDialogScrollBarChanged(GtkAdjustment *adj){
        TT.sbChanged();
    }
//---------------------------------
void TilesetLoad(const char* tilesetName){
    TilesInfo.load(tilesetName);
    Pics.destroy();
    char buff[255];

    sprintf(buff, "../%s", BasePath);
    puts(buff);
    Pics.loadFile(TilesInfo.tilesetpic, 0, 32, buff);
    //sprintf(buff, "../%s/pics/charai.tga", BasePath);
    Pics.loadFile("charai.tga", 1, 16, buff);

    sprintf(buff, "../%s/pics/%s", BasePath, TilesInfo.tilesetpic);

    TT.loadPixbuf((gchar *)buff);

    puts("load 3D tiles");
    sprintf(buff, "../%s/models/%s", BasePath, TilesInfo.tilesetmesh);
    Tiles3D.destroy();
    puts("Test");
    Tiles3D.load(buff, false);
    gtk_adjustment_set_value(GTK_ADJUSTMENT(TT.adjpn), 0);
    GTK_ADJUSTMENT(TT.adjpn)->lower = 0;
    GTK_ADJUSTMENT(TT.adjpn)->page_size = 1;
    GTK_ADJUSTMENT(TT.adjpn)->upper = Tiles3D.count();


}


//---------------------------------------------
static void ChangeTiles(){

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
            LoadTexForTT3D();
            TTUpdate(TT.visualArea3D, NULL);

            g_free (filename);
        }
        gtk_widget_destroy (dialog);


}

//---------------------------------------------
static void NewMap(){
    GMap.destroy();
    GMap.resize(10,10);

}

//------------------------------------------------
    static void Get3DTile(){
        TTUpdate(TT.visualArea3D, NULL);
        sq.frames[sq.tilenum] = gtk_adjustment_get_value((GtkAdjustment*)TT.adjpn);
    }
//-------------------------------------------
    static void MoveCameraToggle(){

        if (TT.draw3D){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(TT.placeTilesButton), FALSE);
            TT.draw3D = false;
        }
    }
//-------------------------------------------
    static void Paint3DToggle(){
        if (!TT.draw3D){
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(TT.moveCameraButton), FALSE);
            TT.draw3D = true;
        }
        puts("pirst");
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
  { "Collision", NULL, "Show _Collision", NULL, "Shows collision layer", ShowCollision, FALSE }
};

    //------------------------------------------
    static void picktoggle(){

        TT.picktoggle();
    }
    //--------------------------------------------
    static gboolean CloseTT(){

        return 1;
    }
    

      //-----------------------------------------------
int main( int   argc, char *argv[] ){

    Cam3D.setpos(Vector3D(0,5,0));
    Cam3D.setdir(Vector3D(0,0,1));
    Cam3D.setup(Vector3D(0,1,0));
    Cam3D.setright(Vector3D(-1,0,0));

    sq.count = 1;
    memset(sq.frames, 0, sizeof(unsigned)*10);

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_icon_from_file("icon.png", NULL);

    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Map Editor");
    gtk_widget_set_usize (GTK_WIDGET (window), 640, 480);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);



    //-----------menu
    GtkUIManager *ui_manager;
    GtkActionGroup *action_group;

    main_vbox = gtk_vbox_new (FALSE, 1);
    gtk_container_border_width (GTK_CONTAINER (main_vbox), 0);
    gtk_container_add (GTK_CONTAINER (window), main_vbox);


    ui_manager = gtk_ui_manager_new ();
    action_group = gtk_action_group_new ("MenuActions");
    gtk_ui_manager_insert_action_group (ui_manager, action_group, 0);
    gtk_action_group_add_actions (action_group, entries, G_N_ELEMENTS (entries), window);
    gtk_action_group_add_toggle_actions (action_group, toggle_entries, G_N_ELEMENTS (toggle_entries), window);
    //gtk_action_group_add_radio_actions (action_group, radio_entries, G_N_ELEMENTS (radio_entries), 0, radio_action_callback, window);

    GError *error;
    gtk_ui_manager_add_ui_from_string (ui_manager, ui_description, -1, &error);

    menubar = gtk_ui_manager_get_widget (ui_manager, "/MainMenu");
    gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, FALSE, 0);

    main_hbox = gtk_hbox_new (FALSE, 1);
    gtk_container_border_width (GTK_CONTAINER (main_hbox), 0);
    gtk_container_add (GTK_CONTAINER (main_vbox), main_hbox);

    //-------------------

    //Drawing
    
    BuildOpenGLArea(&DrawingArea);

    g_signal_connect (DrawingArea, "configure-event", G_CALLBACK (ConfGL), NULL);
    g_signal_connect(DrawingArea, "expose-event", G_CALLBACK(Update), NULL);
    g_signal_connect(DrawingArea, "motion_notify_event",
                     G_CALLBACK(motion_notify_event), NULL);
    g_signal_connect(DrawingArea, "button_press_event",
                     G_CALLBACK(button_press_event), NULL);
    g_signal_connect(DrawingArea, "scroll_event",
                      G_CALLBACK(scroll_event), NULL);
    //----------------
    
    gtk_container_add(GTK_CONTAINER(main_hbox), DrawingArea);

        adjHeight = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
        VScrollbar = gtk_vscrollbar_new (GTK_ADJUSTMENT (adjHeight));
        gtk_range_set_update_policy (GTK_RANGE (VScrollbar),
                                     GTK_UPDATE_CONTINUOUS);

        gtk_box_pack_end(GTK_BOX(main_hbox), VScrollbar, FALSE, TRUE, 0);


    //gtk_box_pack_start (GTK_BOX (main_vbox), DrawingArea, FALSE, TRUE, 0);

    //---------------------------------------------

     adjWidth = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
     HScrollbar = gtk_hscrollbar_new (GTK_ADJUSTMENT (adjWidth));
     gtk_range_set_update_policy (GTK_RANGE (HScrollbar),
                                 GTK_UPDATE_CONTINUOUS);



     gtk_box_pack_end(GTK_BOX(main_vbox), HScrollbar,FALSE, TRUE, 0);


    //------------dialog
        TT.build();

        g_signal_connect (TT.get3DWidget(), "configure-event", G_CALLBACK (TTConfGL), NULL);
        g_signal_connect(TT.get3DWidget(), "expose-event", G_CALLBACK(TTUpdate), NULL);
       /* g_signal_connect(DrawingArea, "motion_notify_event",
                         G_CALLBACK(motion_notify_event), NULL);
        g_signal_connect(DrawingArea, "button_press_event",
                         G_CALLBACK(button_press_event), NULL);
        g_signal_connect(DrawingArea, "scroll_event",
                         G_CALLBACK(scroll_event), NULL);*/
        g_signal_connect (G_OBJECT (TT.moveCameraButton), "clicked",
              G_CALLBACK (MoveCameraToggle), NULL);
        g_signal_connect (G_OBJECT (TT.placeTilesButton), "clicked",
              G_CALLBACK (Paint3DToggle), NULL);

        //----------------------------------------------------
        g_signal_connect (G_OBJECT(TT.adj), "value_changed",
                       G_CALLBACK(TileDialogScrollBarChanged),0);

        g_signal_connect (G_OBJECT(TT.adjpn), "value_changed",
                       G_CALLBACK(Get3DTile),0);

        g_signal_connect (G_OBJECT(TT.dialog), "delete-event",
                       G_CALLBACK(CloseTT),0);

        g_signal_connect (G_OBJECT(TT.adjv), "value_changed",
                       G_CALLBACK(TileDialogScrollBarChanged),0);
        g_signal_connect(G_OBJECT(TT.DA), "expose-event", G_CALLBACK(UpdateTool), NULL);
        g_signal_connect(G_OBJECT(TT.anim_DA), "expose-event", G_CALLBACK(UpdateToolAnim), NULL);


        g_signal_connect (G_OBJECT (TT.anim_addf), "clicked",
              G_CALLBACK (AddFrame), NULL);
        g_signal_connect (G_OBJECT (TT.anim_remf), "clicked",
              G_CALLBACK (RemoveFrame), NULL);
        g_signal_connect (G_OBJECT (TT.anim_prevf), "clicked",
              G_CALLBACK (PrevFrame), NULL);
        g_signal_connect (G_OBJECT (TT.anim_nextf), "clicked",
              G_CALLBACK (NextFrame), NULL);


        g_signal_connect (G_OBJECT(TT.DA), "configure-event", G_CALLBACK (ResizeTool), NULL);

        gtk_signal_connect (GTK_OBJECT (TT.picker),"toggled",G_CALLBACK(picktoggle),
                            NULL);

        gtk_signal_connect (GTK_OBJECT (TT.DA), "button_press_event",
              (GtkSignalFunc) TTMouseClick, NULL);


       
        gtk_widget_show_all(window);




    gdk_gl_drawable_gl_begin(GLDrawable, GLContext);
    TilesetLoad("tilesets/inn.tnf");
    gdk_gl_drawable_gl_end(GLDrawable);

    char path[255];
    sprintf(path, "../%s/maps/inn.wrd", BasePath);
    GMap.load(path);
    SetupAdjustments();
    gtk_widget_show_all (TT.dialog);
    TT.setupAdjustments();

    gtk_adjustment_set_value(GTK_ADJUSTMENT(TT.adjpn), 0);
    GTK_ADJUSTMENT(TT.adjpn)->lower = 0;
    GTK_ADJUSTMENT(TT.adjpn)->page_size = 1;
    GTK_ADJUSTMENT(TT.adjpn)->upper = Tiles3D.count();



    LoadTexForTT3D();


    MHDDestroy(); //rebuilds

    g_timeout_add (1000 / 60, UpdateGL, DrawingArea);
    g_timeout_add (1000 / 60, TTUpdateGL, TT.get3DWidget());

    gtk_main ();

    g_free (FileName);
    puts("Deleting map...");
    GMap.destroy();
    Tiles3D.destroy();

    return 0;
}



