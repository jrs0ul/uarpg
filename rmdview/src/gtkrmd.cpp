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

#endif


#include <cstdlib>
#include <cstring>

#include "gtkrmd.h"
#include "../../src/TextureLoader.h"
#include "../../src/models/ModelCollection.h"
#include "../../src/models/rm2.h"
#include "../../src/Camera.h"
#include "../../src/gui/Button.h"
#include "../../src/gui/Text.h"



enum {
        TARGET_INT32,
        TARGET_STRING,
        TARGET_ROOTWIN
};

static GtkTargetEntry target_list[] = {
        { "INTEGER",    0, TARGET_INT32 },
        { "STRING",     0, TARGET_STRING },
        { "text/plain", 0, TARGET_STRING },
        { "application/x-rootwindow-drop", 0, TARGET_ROOTWIN }};


static guint n_targets = G_N_ELEMENTS (target_list);



    const char BasePath[] = "../base";

    PicsContainer Pics;


    GdkGLContext *GLContext;
    GdkGLDrawable *GLDrawable;
    GtkWidget *window;
    GtkWidget *dialog;
    GtkWidget* menubar;
    GtkWidget *main_vbox;
    GtkWidget *main_hbox;
    GtkWidget *DrawingArea;

    
    int oldMouseX = 0;
    int oldMouseY = 0;

Vector3D ObjectPos = Vector3D(0.0f,0.0f,0.0f);
float    ObjectSize = 1.0f;
Vector3D ObjectRot = Vector3D(0.0f,0.0f,0.0f);
unsigned ObjectFrame = 0;

bool MButtonOld = false;
bool MButton = false;

ModelCollection model;
SubsetCollection model_v2;

Button* Buttons = 0;
unsigned ButtonsCount = 0;


//-----------------------------------------------
static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data ){
   
    puts("Deleting pics...");
    Pics.destroy();
    
    model.destroy();
    model_v2.destroy();
    
    

    return FALSE;
}
//-----------------------------------------------

static gint scroll_event(GtkWidget *widget,
  GdkEventScroll *event)
{
    if (event->direction){
        ObjectSize += 0.1f;
    }
    else
        if (ObjectSize > 0.1f)
            ObjectSize -= 0.1f;
  return FALSE;
}

//----------------------------------------------------
static gboolean motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
  int x, y;
  GdkModifierType state;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &x, &y, &state);
  else
    {
      x = event->x;
      y = event->y;
      state = (GdkModifierType)event->state;
    }

    
    if (state & GDK_BUTTON1_MASK){
        
        float deltax = (x - oldMouseX) * 0.05f;
        float deltay = (y - oldMouseY) * 0.05f;

        ObjectRot.set(ObjectRot.x()+deltax, ObjectRot.y() + deltay, 0.0f);

         
    }

    if (state & GDK_BUTTON3_MASK){
        
        float deltax = (x - oldMouseX) * 0.05f;
        float deltay = (y - oldMouseY) * -0.05f;

        ObjectPos.set( ObjectPos.x() + deltax, ObjectPos.y() + deltay, 0);
         
    }
    
    
    
   
    oldMouseX = x;
    oldMouseY = y;
  
  return TRUE;
}

//----------------------------------------------------
static gboolean button_press_event( GtkWidget *widget, GdkEventButton *event ){

    int mx = 0, my = 0;
    mx = event->x;
    my = event->y;

    if (event->button ==  1){
        if (Buttons){
            for (unsigned i = 0; i < ButtonsCount; i++){
                if (Buttons[i].isPointerOnTop(mx, my)){
                    Buttons[i].pressed = !Buttons[i].pressed;
                }
            }
        }

    }
    return TRUE;
}



//-----------------------------------------------------
static void destroy( GtkWidget *widget, gpointer   data ){
    gtk_main_quit ();
}


//----------------------------------
static void ConfGL(GtkWidget *DA){

    GLContext = gtk_widget_get_gl_context(DA);
    GLDrawable = gtk_widget_get_gl_drawable(DA);
    gdk_gl_drawable_gl_begin(GLDrawable, GLContext);
    
    
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_TEXTURE_2D);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_DEPTH_TEST);
    
    glFrontFace(GL_CW);
    glEnable (GL_CULL_FACE);

    
    glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );
    glClearDepth( 1.0f );
    
    glViewport (0, 0, DA->allocation.width, DA->allocation.height);
            
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    GLfloat aspect = (GLfloat) DA->allocation.width / DA->allocation.height;


    float m[16];
    MatrixPerspective(60.0, aspect, 1.0f, 500.0f, m);
    glMultMatrixf(m);

    glMatrixMode( GL_MODELVIEW );
    
    
    gdk_gl_drawable_gl_end(GLDrawable);

    if (ButtonsCount > 0)
        BuildButtons(ButtonsCount);
}



//--------------------------------
static gboolean UpdateGL(gpointer user_data)
{
    GtkWidget *da = GTK_WIDGET (user_data);
    

        unsigned theCount = 0;
        if (model_v2.count())
            theCount = model_v2.framecount(0);
        else
            theCount = model.frameCount();

        static int bybys = 0;
        bybys ++;
        if (bybys == 30){
            ObjectFrame ++;
            if (ObjectFrame >= theCount )
                ObjectFrame = 0;
            bybys = 0;
        }

   
    gdk_window_invalidate_rect (da->window, &da->allocation, FALSE);
    gdk_window_process_updates (da->window, FALSE);

    return TRUE;
}
//----------------------------
void Redraw(){
   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glLoadIdentity();
  
    float m2[16];
    Vector3D target = Vector3D(0.0f,0.0f,0.0f);
    MatrixLookAt(Vector3D(0,0,20.0f), target,  Vector3D(0,1.0f,0), m2);
    glMultMatrixf(m2);

    glColor3f(1.0f, 1.0f, 1.0f);
    
               
    GLfloat lightposition[] = { 0.1, -1.0, -0.5, 0.0 };
    //jei paskutinis elementas 0 reiškia directional, jei 1 - point
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 0.2 };
    GLfloat mat_difuse[] = { 1.0, 1.0, 1.0, 1.0 };
  
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_difuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_difuse);
   

    glPushMatrix();
    glTranslatef(ObjectPos.x(), ObjectPos.y(), ObjectPos.z());
    glRotatef(ObjectRot.x()*60, 0, 1, 0);
    glRotatef(ObjectRot.y()*60, 1, 0, 0);
    glScalef(ObjectSize, ObjectSize, ObjectSize);
   
    glBindTexture(GL_TEXTURE_2D, Pics.getname(0));
   
    for (unsigned i = 0; i < model.count(); i++){

        if (Buttons[i].pressed){
            model.transform(i);
            model.drawsubset(i, ObjectFrame, false);
        }
    }
        
    for (unsigned i = 0; i < model_v2.count(); i++){

        if (Buttons[i].pressed)
            model_v2.draw(i, ObjectFrame);
    }

   
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glLoadIdentity ();  
    gluOrtho2D (0.0, (GLdouble)DrawingArea->allocation.width,  (GLdouble) DrawingArea->allocation.height, 0.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
        
    char buf[255];
    char tmpl[] = "sub:%3u faces:%4lu Frames:%3lu";
    for (unsigned i = 0; i < model_v2.count(); i++){
            sprintf(buf, tmpl,
                    i, model_v2.facecount(i), (unsigned long)model_v2.framecount(i));
            if (Buttons)
                Buttons[i].draw(Pics, 2, (Buttons[i].pressed ? 1 : 0));
            WriteText(Buttons[i].getX() + 20,
                      Buttons[i].getY(), Pics, 1, buf, 0.7f);
    }
    //------
    for (unsigned i = 0; i < model.count(); i++){
            sprintf(buf, tmpl,
                         i, model.getSubset(i)->faceCount(), model.frameCount());
            if (Buttons)
                Buttons[i].draw(Pics, 2, (Buttons[i].pressed ? 1 : 0));
            WriteText(Buttons[i].getX() + 20,
                      Buttons[i].getY(), Pics, 1, buf, 0.7f);
    }

    ShaderProgram * p1 = 0;
    ShaderProgram * p2 = 0;
    Pics.drawBatch(p1, p2, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode (GL_PROJECTION);

    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );

    glFlush();

    gdk_gl_drawable_swap_buffers(GLDrawable);
   
}

//----------------------------------

static void Update( GtkWidget *widget, gpointer   data ){

    gdk_gl_drawable_gl_begin(GLDrawable, GLContext);
    
    Redraw();
    gdk_gl_drawable_gl_end(GLDrawable);
}

//--------------------------------
void BuildButtons(unsigned count){
    
    if (Buttons){
        delete []Buttons;
        Buttons = 0;
    }
    if (count){

        Buttons = new Button[count];
        int x = 5;
        int y = 10;
        for (unsigned i = 0; i < count; i++){
            Buttons[i].init(x, y, 16, 16);
            y += 16;
            if (y >  (DrawingArea->allocation.height) - 50){
                x += 290;
                y = 10;
            }

        }
        Buttons[0].pressed = true;

        ButtonsCount = count;
    }
}

//---------------------------------
static void LoadModel(){

        GtkWidget *dialog;
        char path[255];
        sprintf(path, "%s/models/", BasePath);
        dialog = gtk_file_chooser_dialog_new ("Open Model",
                        GTK_WINDOW(window),
                        GTK_FILE_CHOOSER_ACTION_OPEN,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                        NULL);
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), path);
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
            char *filename;
            filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
            
            model.destroy();
            model_v2.destroy();
            if (!model_v2.load(filename, false)){
                model.load(filename, false);
                BuildButtons(model.count());
            }
            else{
                BuildButtons(model_v2.count());
            }
            
            g_free (filename);
        }
        gtk_widget_destroy (dialog);
}
//------------------------------------------------------

static void LoadTexture(){

    GtkWidget *dialog;
    char path[255];
    sprintf(path, "%s/pics/", BasePath);
    dialog = gtk_file_chooser_dialog_new ("Open Texture",
                        GTK_WINDOW(window),
                        GTK_FILE_CHOOSER_ACTION_OPEN,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                        NULL);
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), path);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        Pics.loadFile(filename, 0, 512, 1);
        g_free (filename);
    }
    gtk_widget_destroy (dialog);
}
//--------------------------------------------------
static void SplitAllSubsets(){

    for (unsigned i = 0; i < ButtonsCount; i++){
        SubsetCollection s;
        s.addSubset(*model_v2.getSubset(i), false);
        char buf[255];
        sprintf(buf, "./subset%u.rm2", i);
        s.save(buf);
        s.destroy();
    }
}
//-------------------------------------------------
static void RemoveCheckedSubsets(){
    
    SubsetCollection newmod;
    for (unsigned i = 0; i < ButtonsCount; i++){
        if (!Buttons[i].pressed){
            newmod.addSubset(*model_v2.getSubset(i), false);
        }
    }
    model_v2.destroy();
    for (unsigned i = 0; i < newmod.count(); i++){
        model_v2.addSubset(*newmod.getSubset(i), false);
    }
    newmod.save("./Striped.rm2");
    BuildButtons(newmod.count());
    newmod.destroy();
}

//-------------------------------------------------
static void MergeOtherSubsets(){

    GtkWidget *dialog;
    char path[255];
    sprintf(path, "%s/models/", BasePath);
    dialog = gtk_file_chooser_dialog_new ("Open Model",
                        GTK_WINDOW(window),
                        GTK_FILE_CHOOSER_ACTION_OPEN,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                        NULL);
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), path);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT){
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

        SubsetCollection modelv2;
        modelv2.load(filename, false);
        for (unsigned long i = 0; i <  modelv2.count(); i++){
            model_v2.addSubset(*modelv2.getSubset(i), false);
        }
        modelv2.destroy();
        BuildButtons(model_v2.count());
        model_v2.save("./Merged.rm2");
        

        g_free (filename);
    }
    gtk_widget_destroy (dialog);

}


//-------------------------------------------------
static const char *ui_description =
"<ui>"
"  <menubar name='MainMenu'>"
"    <menu action='ModelMenu'>"
"      <menuitem action='Open'/>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='TextureMenu'>"
"      <menuitem action='Load'/>"
"    </menu>"
"    <menu action='SubsetMenu'>"
"       <menuitem action='SplitAll'/>"
"       <menuitem action='RemoveChecked'/>"
"       <menuitem action='Merge'/>"
"    </menu>"
"  </menubar>"
"</ui>";

static const GtkActionEntry entries[] = {
  { "ModelMenu", NULL, "_Model" },
  { "Open", NULL, "_Open", "<control>O", "Load model", LoadModel },
  { "Quit", NULL, "Quit", "<control>Q", "Exit the program", gtk_main_quit },
  { "TextureMenu", NULL, "_Texture" },
  { "Load", NULL, "_Load", "<control>L", "Load Texture", LoadTexture },
  { "SubsetMenu", NULL, "_Subsets" },
  { "SplitAll", NULL, "Split _all", "<control>S", "splits all subsets to different files", SplitAllSubsets },
  { "RemoveChecked", NULL, "_Remove checked", "<control>C", "removes cheked subsets", RemoveCheckedSubsets },
  { "Merge", NULL, "_Merge from file...", "<control>M", "Merge subsets from another file", MergeOtherSubsets },
};


//-------------------------------------------------

void FileDroped(GtkWidget *widget, GdkDragContext *context
        , gint x, gint y, GtkSelectionData *data, guint info, guint time){
        
        
    gchar **file_list;
        
    if (data->length < 0) {
        g_warning("File drag failed");
        gtk_drag_finish(context, FALSE, FALSE, time);
        return;
    }
        
    file_list = g_uri_list_extract_uris((gchar *)data->data);
    if(file_list[0] != NULL) {
        model.destroy();
        model_v2.destroy();
        if (!model_v2.load(g_filename_from_uri(file_list[0], NULL, NULL), false)){
            model.load(g_filename_from_uri(file_list[0], NULL, NULL), false);
            BuildButtons(model.count());
        }
        else{
            BuildButtons(model_v2.count());
        }
    }
    gtk_drag_finish(context, TRUE, FALSE, time);
}


//-----------------------------------------------
int main( int   argc,
          char *argv[] ){
    
    

    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_default_icon_from_file("icon.png", NULL);

    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "RatModel Viewer");
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

    GError *error;
    gtk_ui_manager_add_ui_from_string (ui_manager, ui_description, -1, &error);

    menubar = gtk_ui_manager_get_widget (ui_manager, "/MainMenu");
    gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, FALSE, 0);

    main_hbox = gtk_hbox_new (FALSE, 1);
    gtk_container_border_width (GTK_CONTAINER (main_hbox), 0);
    gtk_container_add (GTK_CONTAINER (main_vbox), main_hbox);


    //----------------------
    //Drawing
    
    DrawingArea = gtk_drawing_area_new();
    //gtk_widget_set_size_request(DrawingArea, 320, 240);
  
    
    gtk_widget_set_events (DrawingArea, GDK_EXPOSURE_MASK);
    
    GdkGLConfig *GLConfiguration;
    
    GLConfiguration = gdk_gl_config_new_by_mode(static_cast<GdkGLConfigMode>(GDK_GL_MODE_RGBA |
                                                         GDK_GL_MODE_DOUBLE |
                                         GDK_GL_MODE_ALPHA |
                                                                             GDK_GL_MODE_DEPTH));
    gtk_widget_set_gl_capability(DrawingArea, GLConfiguration, NULL, true, GDK_GL_RGBA_TYPE);
    
    g_signal_connect (DrawingArea, "configure-event", G_CALLBACK (ConfGL), NULL);
    g_signal_connect(DrawingArea, "expose-event", G_CALLBACK(Update), NULL);
    gtk_signal_connect(GTK_OBJECT (DrawingArea), "motion_notify_event",
                       (GtkSignalFunc)motion_notify_event, NULL);
    gtk_signal_connect (GTK_OBJECT (DrawingArea), "button_press_event",
                        (GtkSignalFunc) button_press_event, NULL);
    gtk_signal_connect (GTK_OBJECT (DrawingArea), "scroll_event",
                (GtkSignalFunc) scroll_event, NULL);

                           
    gtk_widget_set_events (DrawingArea, GDK_EXPOSURE_MASK
             | GDK_LEAVE_NOTIFY_MASK
             | GDK_BUTTON_PRESS_MASK
             | GDK_POINTER_MOTION_MASK
             | GDK_POINTER_MOTION_HINT_MASK);
    gtk_drag_dest_set (DrawingArea, GTK_DEST_DEFAULT_ALL, target_list,
               n_targets,
               GDK_ACTION_COPY);
    g_signal_connect (GTK_OBJECT (DrawingArea), "drag_data_received",
              G_CALLBACK (FileDroped), NULL);
    
    
    //----------------
    gtk_container_add(GTK_CONTAINER(main_hbox), DrawingArea);

    
    
    gtk_widget_show_all(window);
    
    char path[255];
    sprintf(path, "%s/models/lildude.rm2", BasePath);
    if (!model_v2.load(path, false)){
        puts("loading old format");
        model.load(path, false);
        BuildButtons(model.count());
    }
    else
        BuildButtons(model_v2.count());
    puts("model loaded");
    Pics.loadFile("chuman.tga", 0, 512, BasePath, 1);
    Pics.loadFile("charai.tga", 1, 16, BasePath, 1);
    Pics.loadFile("../edmapSDL/smallbuttons.tga", 2, 16);
  
    g_timeout_add (1000 / 60, UpdateGL, DrawingArea);
    
    
    gtk_main ();
    
    
    Pics.destroy();
    model_v2.destroy();
    model.destroy();
    delete []Buttons;

    return 0;
}



