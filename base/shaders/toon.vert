    varying vec3 normal, lightDir;
    varying vec4 ambient, diffuse;
    
    void main(){
    
        ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
        ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
        diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
        normal = normalize(gl_NormalMatrix * gl_Normal);
        lightDir = normalize(vec3(gl_LightSource[0].position));
        gl_TexCoord[0] = gl_MultiTexCoord0;
        gl_Position = ftransform();
    } 

