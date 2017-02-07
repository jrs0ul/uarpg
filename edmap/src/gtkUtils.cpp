#include "gtkUtils.h"

void BuildOpenGLArea(GtkWidget ** Area3D, GdkGLContext* sharesWith ){
      *Area3D = gtk_drawing_area_new();
        //gtk_widget_set_size_request(Area3D, 320, 240);

        gtk_widget_set_events (*Area3D, GDK_EXPOSURE_MASK);

        GdkGLConfig *GLConfiguration;
        GLConfiguration = gdk_gl_config_new_by_mode(static_cast<GdkGLConfigMode>(
                                                    GDK_GL_MODE_RGBA |
                                                    GDK_GL_MODE_DOUBLE |
                                                    GDK_GL_MODE_ALPHA |
                                                    GDK_GL_MODE_DEPTH));

        if (sharesWith != NULL) puts("BYBYS");
        gtk_widget_set_gl_capability(*Area3D,
                                    GLConfiguration,
                                    sharesWith, 
                                    true, 
                                    GDK_GL_RGBA_TYPE);

        gtk_widget_set_events (*Area3D, 
                               GDK_EXPOSURE_MASK
                               | GDK_LEAVE_NOTIFY_MASK
                               | GDK_BUTTON_PRESS_MASK
                               | GDK_BUTTON_MOTION_MASK
                               | GDK_POINTER_MOTION_MASK
                               | GDK_POINTER_MOTION_HINT_MASK);

}
