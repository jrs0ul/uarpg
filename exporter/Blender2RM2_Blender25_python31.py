from array import array
import mathutils
#--------------------------------------------

def ObjectCount(objs, type):
    objectcount = 0
    for ob in objs :
        if (ob.type == type):
            objectcount+=1
            
    return objectcount

#--------------------------------------------
#writes bone geometry to file
def ExportGeometry(mesh, matrix, out, swapyz):
    
    vertex = array('f')
    normals = array('f')
    for vert in mesh.verts :
        vt = vert.co * matrix
        if swapyz == True:
            vertex.append(vt.x)
            vertex.append(vt.z)
            vertex.append(vt.y*-1)

            normals.append(vert.normal.x * -1)
            normals.append(vert.normal.z * -1)
            normals.append(vert.normal.y)
        else:
            vertex.append(vt.x)
            vertex.append(vt.y)
            vertex.append(vt.z)

            normals.append(vert.normal.x)
            normals.append(vert.normal.y)
            normals.append(vert.normal.z)

    vertc = array('i')
    vertc.append(len(mesh.verts))
    vertc.tofile(out)
    vertex.tofile(out)
    normals.tofile(out)

    faces = array('i')
    uvs = array('f')
    
    facecount=0
    for face in mesh.faces :
        # is triangle
        if (len(face.verts)==3):

            if (swapyz == True):
                faces.append(face.verts[2].index) 
                faces.append(face.verts[1].index) 
                faces.append(face.verts[0].index)
            else:
                faces.append(face.verts[0].index) 
                faces.append(face.verts[2].index) 
                faces.append(face.verts[1].index)


            facecount+=1

            if (mesh.hasFaceUV()==1):
                if (swapyz == True):
                    uvs.append(face.uv[2][0])
                    uvs.append(face.uv[2][1])
                    uvs.append(face.uv[1][0])
                    uvs.append(face.uv[1][1])
                    uvs.append(face.uv[0][0])
                    uvs.append(face.uv[0][1])
                else:
                    uvs.append(face.uv[0][0])
                    uvs.append(face.uv[0][1])
                    uvs.append(face.uv[2][0])
                    uvs.append(face.uv[2][1])
                    uvs.append(face.uv[1][0])
                    uvs.append(face.uv[1][1])
                            

        #not triangle ?
        else :

            faces.append(face.verts[2].index)

            if (mesh.hasFaceUV()==1):
                uvs.append(face.uv[2][0])
                uvs.append(face.uv[2][1])

            faces.append(face.verts[1].index)
            if (mesh.hasFaceUV()==1):
                uvs.append(face.uv[1][0])
                uvs.append(face.uv[1][1])

            faces.append(face.verts[0].index)
            if (mesh.hasFaceUV()==1):
                uvs.append(face.uv[0][0])
                uvs.append(face.uv[0][1])

            if (swapyz == True):
                faces.append(face.verts[3].index)
                if (mesh.hasFaceUV()==1):
                    uvs.append(face.uv[3][0])
                    uvs.append(face.uv[3][1])

                faces.append(face.verts[2].index)
                if (mesh.hasFaceUV()==1):
                    uvs.append(face.uv[2][0])
                    uvs.append(face.uv[2][1])

                faces.append(face.verts[0].index)
                if (mesh.hasFaceUV()==1):
                    uvs.append(face.uv[0][0])
                    uvs.append(face.uv[0][1])
            else:
                faces.append(face.verts[0].index)
                if (mesh.hasFaceUV()==1):
                    uvs.append(face.uv[0][0])
                    uvs.append(face.uv[0][1])

                faces.append(face.verts[2].index)
                if (mesh.hasFaceUV()==1):
                    uvs.append(face.uv[2][0])
                    uvs.append(face.uv[2][1])

                faces.append(face.verts[3].index)
                if (mesh.hasFaceUV()==1):
                    uvs.append(face.uv[3][0])
                    uvs.append(face.uv[3][1])


            facecount+=2
    #end for
    facec = array('i')
    facec.append(facecount)
    facec.tofile(out)
    print(facecount)
    faces.tofile(out)
    uvsc = array('i')
    if (mesh.hasFaceUV()==1):
        uvsc.append(1)
    else:
        uvsc.append(0)
    uvsc.tofile(out)
    if (mesh.hasFaceUV()==1):
        uvs.tofile(out)
    
#-----------------------------------------------
def exportFrame(_armature, i, bone, out, swap):
    
    bpy.data.scenes[0].set_frame(i)
    pose = _armature.pose
    armpos = _armature.matrix

    identity = mathutils.Matrix([1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1])
    rotation = mathutils.RotationMatrix(-90, 4, 'X')
    swapyz = rotation * identity


    if (swap == True):
        initial = mathutils.Matrix(bone.matrix_local) * armpos * swapyz
        initial.invert()
        posematrix = mathutils.Matrix(pose.bones[bone.name].matrix) * armpos * swapyz
    else:
        initial = mathutils.Matrix(bone.matrix_local) * armpos
        initial.invert()
        posematrix = mathutils.Matrix(pose.bones[bone.name].matrix) * armpos

    
    finalmatrix = initial * posematrix

    
    floats = array('f')
   
    for a in range(4):
        for b in range(4):
            floats.append(finalmatrix[a][b])

    floats.tofile(out)
#-----------------------------------------------
def WriteInfluences(BoneList, mesh, out):

    for vert in mesh.verts:
        #influences = mesh.getVertexInfluences(vert.index)

        #sum_influences = sum([influence[1] for influence in influences])
        
        boneIndices = array('i')
        boneWeights = array('f')

        #boneIndices.append(len(influences))

        #for influence in influences:
        #    boneIndices.append(BoneList.index(influence[0]))
        #    boneWeights.append(influence[1]/sum_influences)

        boneIndices.tofile(out)
        boneWeights.tofile(out)
#--------------------------------------------
def export(filename):

    out = open(filename,'wb+')
    scene = bpy.data.scenes[0]
    Objects = scene.objects

    magic = array('b')
    magic.fromstring('RM2')
    magic.tofile(out)

    
    ints = array('i')
    
    ints.append(ObjectCount(Objects, 'MESH'))
    ints.tofile(out)
    ints.pop

    swapyz = True
    
    for ob in Objects:
        if (ob.type == 'MESH'):
            mesh = ob.data

            m = mathutils.Matrix(ob.matrix)

            ExportGeometry(mesh, m, out, swapyz)

            if (ob.parent != None):
                parent = ob.parent

                if (parent.type == 'ARMATURE'):
                    ArmatureData = parent.data
                    Bones = ArmatureData.bones
                    BoneData = Bones.values()

                    bonec = array('i')
                    bonec.append(len(BoneData))
                    bonec.tofile(out)

                    BoneList = []
                    for bone in BoneData:
                        BoneList.append(bone.name)

                    WriteInfluences(BoneList, mesh, out)

                    totalFrames = scene.frame_end -\
                                  scene.frame_start + 1

                    framec = array('i')
                    framec.append(totalFrames)
                    framec.tofile(out)
                    framec.pop

                    for bone in BoneData:
                        for i in range(scene.frame_start, scene.frame_end + 1):
                            exportFrame(parent, i, bone, out, swapyz)
                        #for each frame

                #if armature
            else:
                bonec = array('i')
                bonec.append(0)
                bonec.append(0)
                bonec.tofile(out)

    out.close()



export("testas.lol")
