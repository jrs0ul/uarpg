#include "AssetBrowser.h"

void AssetBrowser::build(int _x, int _y,
               int w, int h,
               int iSize, unsigned iCount){

    x = _x;
    y = _y;
    width = w;
    height = h;
    itemSize = iSize;
    itemCount = iCount;
    if (angles)
        free(angles);

    angles = (float*)malloc(sizeof(float)*iCount);

    printf("%d/%d = %d\n", w, itemSize, (int)floorf((width * 1.0f) / (itemSize * 1.0f)));

    unsigned pSize = (unsigned)((int)(floorf((width * 1.0f) / (itemSize * 1.0f)))
                                *
                                (height / itemSize));
    pageSize = (pSize >= iCount)? iCount : pSize;

    currentPage = 0;

    prevPage.init(x + width + 3, y , 16, 16);
    nextPage.init(x + width + 3, y + 18, 16, 16);

};
//-----------------------------
void AssetBrowser::drawAssets(SubsetCollection& models,
                        PicsContainer& pics, unsigned textureIndex,
                        int mouseX, int mouseY, 
                        int selectedAsset){
        
    pics.draw(-1, x, y, 0, false, width, height, 0.0f,
              COLOR(0.0f, 0.0f, 0.0f, 0.6f),
              COLOR(0.0f, 0.0f, 0.0f, 0.6f));

    prevPage.draw(pics, 3, 2);
    nextPage.draw(pics, 3, 3);

    if (itemCount < 1)
        return;

    unsigned rowSize = (unsigned)floorf((width * 1.0f) / (itemSize * 1.0f));
    unsigned rowCount = ceil(pageSize / (rowSize * 1.0f));
    if (rowSize > pageSize){
        rowSize = pageSize;
        rowCount = 1;
    }

    int itemX = -1;
    int itemY = -1;
    if ((mouseX > x) && (mouseX < x + width)
        &&(mouseY > y)&&(mouseY < y + height)){
        itemX = (mouseX - x) / itemSize;
        itemY = (mouseY - y) / itemSize;

    }


    pics.drawBatch(0, 0, 666);

    glBindTexture(GL_TEXTURE_2D, pics.getname(textureIndex));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glPushMatrix();
    unsigned index = currentPage * pageSize;
    for (unsigned i = 0; i < rowCount; i++){
        for (unsigned a = 0; a < rowSize; a++){

            if (((itemX > -1) && (itemY > -1))
                &&((itemX == (int)a) && (itemY == (int)i))){
                pics.draw(-1, x + (a * itemSize),
                          y + (i * itemSize),
                          0, false, itemSize, itemSize, 0.0f,
                COLOR(1.0f, 0.0f, 0.0f, 0.2f),
                COLOR(1.0f, 0.0f, 0.0f, 0.2f));
            }

            if (selectedAsset == (int)index)
                pics.draw(-1, x + (a * itemSize),
                          y + (i * itemSize),
                          0, false, itemSize, itemSize, 0.0f,
                COLOR(0.0f, 1.0f, 0.5f, 0.2f),
                COLOR(0.0f, 1.0f, 0.5f, 0.2f));



            if (index < itemCount){
                glEnable(GL_DEPTH_TEST);
                glLoadIdentity();
                FlatMatrix stm;
                MatrixTranslationScale( Vector3D(x + (a * itemSize + itemSize/2),
                                        y + (i * itemSize + itemSize), 0), 
                                        Vector3D(2.5f, 2.5f, 2.5f), stm.m);
                FlatMatrix rmx, rmy, rmz;
                MatrixRotationAxis(3.14, Vector3D(1.0f, 0, 0), rmx.m);
                MatrixRotationAxis(angles[index] * 0.01745f , Vector3D(0, 1.0f, 0), rmy.m);
                MatrixRotationAxis(0, Vector3D(0, 0, 1.0f), rmz.m);

                FlatMatrix rot = rmx * rmy * rmz;
                FlatMatrix fm = rot * stm;

                glMultMatrixf(fm.m);

                models.draw(index);

                index ++;
                glDisable(GL_DEPTH_TEST);
            }
        }
    }
    glPopMatrix();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}
//--------------------------------------------
void AssetBrowser::drawTiles(PicsContainer& pics, unsigned textureIndex,
                             int mouseX, int mouseY,
                             int selectedAsset){

    pics.draw(-1, x, y, 0, false, width, height, 0.0f,
              COLOR(0.0f, 0.0f, 0.0f, 0.6f),
              COLOR(0.0f, 0.0f, 0.0f, 0.6f));

    prevPage.draw(pics, 3, 2);
    nextPage.draw(pics, 3, 3);

    unsigned rowSize = (unsigned)floorf((width * 1.0f) / (itemSize * 1.0f));
    unsigned rowCount = ceil(pageSize / (rowSize * 1.0f));
    if (rowSize > pageSize){
        rowSize = pageSize;
        rowCount = 1;
    }

    int itemX = -1;
    int itemY = -1;
    if ((mouseX > x) && (mouseX < x + width)
        &&(mouseY > y)&&(mouseY < y + height)){
        itemX = (mouseX - x) / itemSize;
        itemY = (mouseY - y) / itemSize;

    }

    pics.drawBatch(0, 0, 666);

    unsigned index = currentPage * pageSize;
    for (unsigned i = 0; i < rowCount; i++){
        for (unsigned a = 0; a < rowSize; a++){

            if (index < itemCount){
                pics.draw(textureIndex, x + (a * itemSize),
                          y + (i * itemSize), index, false);

                index ++;
            }
             if (selectedAsset == (int)index)
                pics.draw(-1, x + (a * itemSize),
                          y + (i * itemSize),
                          0, false, itemSize, itemSize, 0.0f,
                COLOR(0.0f, 1.0f, 0.5f, 0.2f),
                COLOR(0.0f, 1.0f, 0.5f, 0.2f));


            if (((itemX > -1) && (itemY > -1))
                &&((itemX == (int)a) && (itemY == (int)i))){
                pics.draw(-1, x + (a * itemSize),
                          y + (i * itemSize),
                          0, false, itemSize, itemSize, 0.0f,
                COLOR(1.0f, 0.0f, 0.0f, 0.5f),
                COLOR(1.0f, 0.0f, 0.0f, 0.5f));
            }

        }
    }

    pics.drawBatch(0, 0, 666);
}

//--------------------------------------------
void AssetBrowser::onMouseMove(int mouseX, int mouseY){
    unsigned rowSize = (unsigned)floorf((width * 1.0f)/ (itemSize * 1.0f));
    int itemX = -1;
    int itemY = -1;
    if ((mouseX > x) && (mouseX < x + width)
        &&(mouseY > y)&&(mouseY < y + height)){
        itemX = (mouseX - x) / itemSize;
        itemY = (mouseY - y) / itemSize;

    }

    if ((itemX > -1) && (itemY > -1)){
        int index = (pageSize * currentPage) + (itemY * rowSize) + itemX;
       
        if (index < (int)itemCount){
            angles[index] += 1.0f;
            if (angles[index] >= 360.0f )
                angles[index] = 0.0f;
        }

    }

}
//--------------------------------------------
int AssetBrowser::onMouseUp(int mouseX, int mouseY){

    if (itemCount < 1)
        return 0;
    unsigned pageCount = (unsigned)ceil(itemCount / pageSize * 1.0f);
    unsigned rowSize = (unsigned)floorf((width * 1.0f) / (itemSize * 1.0f));
    //unsigned rowCount = ceil(pageSize / (rowSize * 1.0f));

    if (prevPage.isPointerOnTop(mouseX, mouseY)){
        if (currentPage > 0)
            currentPage--;
    }
    if (nextPage.isPointerOnTop(mouseX, mouseY)){
        if (currentPage < pageCount)
            currentPage++;
    }
    
    int itemX = -1;
    int itemY = -1;
    if ((mouseX > x) && (mouseX < x + width)
        &&(mouseY > y)&&(mouseY < y + height)){
        itemX = (mouseX - x) / itemSize;
        itemY = (mouseY - y) / itemSize;

    }

    if ((itemX > -1) && (itemY > -1)){
        return (pageSize * currentPage) + (itemY * rowSize) + itemX;
    }

    return -1;

}
