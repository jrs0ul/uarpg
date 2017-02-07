#include "Skybox.h"

    void Skybox::build(float size){

        float r = size;
        float cz = -0.0f,cx = 1.0f;

        vertices[0] = r ; vertices[1] = -r;  vertices[2] =  r;
        vertices[3] = r ; vertices[4] = -r;  vertices[5] = -r;
        vertices[6] = r ; vertices[7] =  r;  vertices[8] = -r;
        vertices[9] = r ; vertices[10] = r; vertices[11] =  r;
        uvs[0] = cx; uvs[1] = cz;
        uvs[2] = cz; uvs[3] = cz;
        uvs[4] = cz; uvs[5] = cx;
        uvs[6] = cx; uvs[7] = cx;
        //----
        vertices[12] = -r ; vertices[13] = -r;  vertices[14] =  r;
        vertices[15] = -r ; vertices[16] = r;   vertices[17] =  r;
        vertices[18] = -r ; vertices[19] =  r;  vertices[20] = -r;
        vertices[21] = -r ; vertices[22] = -r;  vertices[23] = -r;
        uvs[8]  = cz; uvs[9]  = cz;
        uvs[10] = cz; uvs[11]  = cx;
        uvs[12] = cx; uvs[13] = cx;
        uvs[14] = cx; uvs[15] = cz;
        //----
        vertices[24] = -r ; vertices[25] =  r;  vertices[26] =  -r;
        vertices[27] = -r ; vertices[28] =  r;  vertices[29] =   r;
        vertices[30] =  r ; vertices[31] =  r;  vertices[32] =   r;
        vertices[33] =  r ; vertices[34] = r;   vertices[35] =  -r;
        uvs[16] = cz; uvs[17] = cx;
        uvs[18] = cx; uvs[19] = cx;
        uvs[20] = cx; uvs[21] = cz;
        uvs[22] = cz; uvs[23] = cz;
        //----
        vertices[36] = -r ;  vertices[37] = -r;  vertices[38] = -r;
        vertices[39] =  r ;  vertices[40] = -r;  vertices[41] = -r;
        vertices[42] =  r ;  vertices[43] = -r;  vertices[44] = r;
        vertices[45] = -r ;  vertices[46] = -r;  vertices[47] =  r;
        uvs[24] = cx; uvs[25] = cz;
        uvs[26] = cx; uvs[27] = cx;
        uvs[28] = cz; uvs[29] = cx;
        uvs[30] = cz; uvs[31] = cz;
        //----
        vertices[48] =  r ; vertices[49]  = -r;  vertices[50] =  r;
        vertices[51] =  r ; vertices[52]  =  r;  vertices[53] =  r;
        vertices[54] = -r ; vertices[55]  =  r;  vertices[56] =  r;
        vertices[57] = -r ; vertices[58] = -r;  vertices[59] = r;
        uvs[32] = cz; uvs[33] = cz;
        uvs[34] = cz; uvs[35] = cx;
        uvs[36] = cx; uvs[37] = cx;
        uvs[38] = cx; uvs[39] = cz;
        //----
        vertices[60] =  r ; vertices[61] =  -r;  vertices[62] =  -r;
        vertices[63] = -r ; vertices[64] =  -r;  vertices[65] =  -r;
        vertices[66] = -r ; vertices[67] =   r;  vertices[68] =  -r;
        vertices[69] =  r ; vertices[70] =  r; vertices[71] =  -r;
        uvs[40] = cx; uvs[41] = cz;
        uvs[42] = cz; uvs[43] = cz;
        uvs[44] = cz; uvs[45] = cx;
        uvs[46] = cx; uvs[47] = cx;
        
    }

    //------------------------------------
    void Skybox::draw(PicsContainer& picsai){

        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        
        glColor4f(1.0, 1.0, 1.0,1.0f);
        glBindTexture(GL_TEXTURE_2D, picsai.getname(picsai.findByName("sky/neg_x.tga")));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glDrawArrays(GL_QUADS, 0, 4);
        //----

        glBindTexture(GL_TEXTURE_2D, picsai.getname(picsai.findByName("sky/pos_x.tga")));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glDrawArrays(GL_QUADS, 4, 4);
        //----
        glBindTexture(GL_TEXTURE_2D, picsai.getname(picsai.findByName("sky/pos_y.tga")));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glDrawArrays(GL_QUADS, 8, 4);
        //-----
        glBindTexture(GL_TEXTURE_2D, picsai.getname(picsai.findByName("sky/neg_y.tga")));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glDrawArrays(GL_QUADS, 12, 4);
        //-----
        glBindTexture(GL_TEXTURE_2D, picsai.getname(picsai.findByName("sky/pos_z.tga")));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glDrawArrays(GL_QUADS, 16, 4);
        //----
        glBindTexture(GL_TEXTURE_2D, picsai.getname(picsai.findByName("sky/neg_z.tga")));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, uvs);
        glDrawArrays(GL_QUADS, 20, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glEnable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);

    }
