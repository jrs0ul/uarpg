#include <cstdlib>
#include <cstring>
#include "Extensions.h"
#include "VertexObject.h"


void VertexObject::init(void * _data, unsigned format,
                        unsigned long _vertexCount,
                        bool useDynamic, bool _useVBO){

    vertexFormat = (VertexFormat)format;

    _vertSize = 0;
    _vertSize += (vertexFormat & VPOSITION)  ? (3 * sizeof(float)) : 0;
    _vertSize += (vertexFormat & VNORMAL)    ? (3 * sizeof(float)) : 0;
    _vertSize += (vertexFormat & VCOLOR)     ? (4 * sizeof(float)) : 0;
    _vertSize += (vertexFormat & VTEXCOORD0) ? (2 * sizeof(float)) : 0;

    _vertCount = _vertexCount;

    if (data)
        free(data);
    data = (char * )malloc(_vertSize * _vertCount);
    memcpy(data, _data, _vertSize * _vertCount);

    useVBO = _useVBO;

    if (_useVBO){
        glGenBuffers(1, &vboId);
        
        bufferType = (useDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, _vertSize * _vertCount, data, bufferType);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


}
//--------------------------------------------
void VertexObject::bind(){


    char * offset = NULL;
    if (useVBO){
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        //puts("use vbo");
    }
    else
        offset = data;

    if (vertexFormat & VPOSITION){
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, _vertSize, offset);
        offset += (3 * sizeof(float));
    }
    if (vertexFormat & VNORMAL){
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, _vertSize, offset);
        offset += (3 * sizeof(float));
    }
    if (vertexFormat & VCOLOR){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, _vertSize, offset);
        offset += (4 * sizeof(float));
    }
    if (vertexFormat & VTEXCOORD0){
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, _vertSize, offset);
        offset += (2 * sizeof(float));
    }
}
//--------------------------------------------
void VertexObject::unbind(){


    if (useVBO)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (vertexFormat & VPOSITION){
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    if (vertexFormat & VNORMAL){
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (vertexFormat & VCOLOR){
        glDisableClientState(GL_COLOR_ARRAY);
    }
    if (vertexFormat & VTEXCOORD0){
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }


}
//--------------------------------------------
float * VertexObject::lock(){
    if (useVBO){
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        return (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }
    return (float*)data;
}
//--------------------------------------------
void VertexObject::unlock(){
    if (useVBO){
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

//--------------------------------------------
void VertexObject::destroy(){

    if ((useVBO)&&(vboId)){
        glDeleteBuffers(1, &vboId);
        vboId = 0;
    }
    if (data){
        free(data);
        data = 0;
    }
    vertexFormat = VEMPTY;
    _vertSize = 0;
    _vertCount = 0;
}
//--------------------------------------------
void VertexObject::copyFrom(VertexObject& obj){

    useVBO = obj.useVBO;
    vertexFormat = obj.vertexFormat;
    bufferType = obj.bufferType;
    _vertSize = obj._vertSize;
    _vertCount = obj._vertCount;
    if (data)
        free(data);
    data = (char * )malloc(_vertSize * _vertCount);
    memcpy(data, obj.data, _vertSize * _vertCount);
    if (useVBO){
        glGenBuffers(1, &vboId);
        
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, _vertSize * _vertCount, data, bufferType);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}
//--------------------------------------------
