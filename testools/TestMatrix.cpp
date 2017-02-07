#include "../src/Matrix.h"


int main(){

    FlatMatrix m;
    MatrixTranslationScale(Vector3D(1.0f, 2.0f, 3.0f),
                           Vector3D(0.5f, 0.25f, 0.125f),
                           m.m);
    MatrixDump(m.m);

    FlatMatrix m1;
    MatrixRotationAxis(90.0f, Vector3D(0, 1, 0), m1.m);
    MatrixDump(m1.m);

    Matrix m2;
    MatrixRotationAxis(90.0f, Vector3D(0, 1, 0), m2);
    MatrixDump(m2);


}
