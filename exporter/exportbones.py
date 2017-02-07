import bpy
import Blender
from array import array
from Blender import Mathutils
#--------------------------------------------

def ObjectCount(objs, type):
    objectcount = 0
    for ob in objs :
        if (ob.getType() == type):
            objectcount+=1
            
    return objectcount
#---------------------------------------------
def getRootBone(armature):
    rootBone = None
    for bone in armature.bones.values():
        if bone.parent == None :
            if rootBone == None:
                rootBone = bone
            else:
                print"Multiple root armature not handled"
    return rootBone


#---------------------------------------------
#writes bone geometry to file
def ExportGeometry(mesh, out):
    
    vertex = array('f')
    normals = array('f')
    for vert in mesh.verts :
        vertex.append(vert.co.x)
        vertex.append(vert.co.y)
        vertex.append(vert.co.z)

        normals.append(-vert.no.x)
        normals.append(-vert.no.y)
        normals.append(-vert.no.z)
    vertc = array('i')
    vertc.append(len(mesh.verts))
    vertc.tofile(out)
    vertex.tofile(out)
    normals.tofile(out)

    faces = array('i')
    facecount=0
    for face in mesh.faces :
        # is triangle
        if (len(face.v)==3):
            faces.append(face.v[0].index) 
            faces.append(face.v[2].index) 
            faces.append(face.v[1].index)
            facecount+=1
        #not triangle ?
        else :
            faces.append(face.v[2].index)
            faces.append(face.v[1].index)
            faces.append(face.v[0].index)
            faces.append(face.v[0].index)
            faces.append(face.v[2].index)
            faces.append(face.v[3].index)
            facecount+=2
    #end for
    facec = array('i')
    facec.append(facecount)
    facec.tofile(out)
    print facecount
    faces.tofile(out)
#--------------------------------------------
def PrintBones(bone, depth = 0):
    print (' ' *  depth) + bone.name + ', children('+str(len(bone.children))+')[ '
    if (bone.hasChildren):
        depth += 1
        for chil in bone.children:
            PrintBones(chil, depth)
    depth -= 1
    print (depth * ' ') + ']'

#-------------------------------------------
#recursive subroutine, writes bone data to file
#bone,name, head and tail positions and assigned
#vertices and their weights
def WriteBoneTree(bone, mesh, out):
    nameLen = array('i')
    nameLen.append(len(bone.name))
    nameLen.tofile(out)
    name = array('c')
    name.fromstring(bone.name)
    name.tofile(out)
    floats = array('f')
    floats.append(bone.head['ARMATURESPACE'].x)
    floats.append(bone.head['ARMATURESPACE'].y)
    floats.append(bone.head['ARMATURESPACE'].z)

    floats.append(bone.tail['ARMATURESPACE'].x)
    floats.append(bone.tail['ARMATURESPACE'].y)
    floats.append(bone.tail['ARMATURESPACE'].z)

    floats.tofile(out)
    #gets vertices assigned to bone and their weights
    verticelist = mesh.getVertsFromGroup(bone.name, 1)
    bvertices = array('i')
    weights = array('f')
    #puts count of vertices
    bvertices.append(len(verticelist))
    print bone.name
    print verticelist
    for v in verticelist:
        bvertices.append(v[0])
        weights.append(v[1])
    #writes verices and weights to file
    bvertices.tofile(out)
    weights.tofile(out)

    childCount = array('i')
    childCount.append(len(bone.children))
    childCount.tofile(out)
    #goes to children
    if (bone.hasChildren):
        for chil in bone.children:
            WriteBoneTree(chil, mesh, out)

#----------------------------------------
def matrixToEuler( matrix ):
    euler = matrix.toEuler()
    return Mathutils.Vector( -euler.x, euler.y, -euler.z )

def vector_by_matrix( m, p ):
    return Mathutils.Vector( [ p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0] + m[3][0],
                     p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1] + m[3][1],
                     p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2] + m[3][2] ] )


#----------------------------------------
#writes bone state in frame i
def exportFrame(_armature, i, bone, out):
    print "===Frame :"+ str(i)+"========"
    Blender.Set("curframe",i)
    pose = _armature.getPose()

    tail = pose.bones[bone.name].tail
    head = pose.bones[bone.name].head
    print "tail:"
    print tail
    print "head:"
    print head

    initial = bone.matrix['ARMATURESPACE']
    initial.invert()
    
    flipMatrix = Mathutils.Matrix([1, 0, 0, 0], [0, 0, 1, 0], [0, 1, 0, 0], [0, 0, 0, 1])

    finalmatrix = Mathutils.Matrix(bone.matrix['ARMATURESPACE']) * pose.bones[bone.name].poseMatrix  #* flipMatrix#initial * matrix 
    
    
    print "Euler:"
    print matrixToEuler(finalmatrix)
    print "Pos:"
    print vector_by_matrix(finalmatrix, Mathutils.Vector( [ 0, 0, 0 ] ))

    

    floats = array('f')
    #print finalmatrix
    for a in range(4):
        for b in range(4):
            floats.append(finalmatrix[a][b])

    #floats = array('f')
    floats.append(tail[0])
    floats.append(tail[1])
    floats.append(tail[2])

    floats.append(head[0])
    floats.append(head[1])
    floats.append(head[2])

    floats.tofile(out)


#--------------------------------------------
def export(filename):


    out = file(filename,'wb+')
    scene = bpy.data.scenes.active
    Objects = scene.objects    
    
    ints = array('i')
    
    ints.append(ObjectCount(Objects, 'Mesh'))
    ints.tofile(out)
    ints.pop
    
    for ob in Objects:
        if (ob.getType()== 'Mesh'):
            mesh = ob.getData()

            ExportGeometry(mesh, out)

            if (ob.parent != None):
                parent = ob.parent

                if (parent.getType() == 'Armature'):
                    ArmatureData = parent.getData()

                    RootBone = getRootBone(ArmatureData)
                    print 'root bone name is:' + RootBone.name
                    PrintBones(RootBone)
                    WriteBoneTree(RootBone, mesh, out)

                    Bones = ArmatureData.bones
                    BoneData = Bones.values()
                    

                    totalFrames = Blender.Get("endframe") -\
                                  Blender.Get("staframe") + 1

                    framec = array('i')
                    framec.append(totalFrames)
                    framec.tofile(out)
                    framec.pop


                    for bone in BoneData:
                        bnameLen = array('i')
                        bnameLen.append(len(bone.name))
                        bnameLen.tofile(out)
                        bname = array('c')
                        bname.fromstring(bone.name)
                        print '-----------------------------'
                        print bname
                        bname.tofile(out)

                        floats = array('f')
                        floats.append(bone.head['ARMATURESPACE'].x)
                        floats.append(bone.head['ARMATURESPACE'].y)
                        floats.append(bone.head['ARMATURESPACE'].z)

                        floats.append(bone.tail['ARMATURESPACE'].x)
                        floats.append(bone.tail['ARMATURESPACE'].y)
                        floats.append(bone.tail['ARMATURESPACE'].z)
                        floats.tofile(out)

                        print "--------------------------"
                        m = Mathutils.Matrix(bone.matrix['ARMATURESPACE'])
                        m = m.invert()
                        fm = array('f')
                        for a in range(4):
                                for b in range(4):
                                    fm.append(m[a][b])
                        fm.tofile(out)
                        print "Euler values:"
                        print matrixToEuler(m)
                        print "translation:"
                        print vector_by_matrix(m, Mathutils.Vector( [ 0, 0, 0 ] ))
                        print "----------------------------"


                        for i in range(Blender.Get("staframe"),Blender.Get("endframe")+1):
                            exportFrame(parent, i, bone, out)
                          
                        #for each frame

                #if armature
    out.close()


#----------------------------------------------

Blender.Window.FileSelector(export,"Export")
