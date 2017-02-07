uniform sampler2D tex;
varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;
    
    
    void main(){
        vec3 n,halfV;
        float NdotL,NdotHV;
        
        vec4 color = ambient;
        
        n = normalize(normal);
        NdotL = max(dot(n,lightDir),0.0);

        if (NdotL > 0.0) {
            color += diffuse * NdotL;
            halfV = normalize(halfVector);
            NdotHV = max(dot(n,halfV),0.0);
            /*color += gl_FrontMaterial.specular * 
                    gl_LightSource[0].specular * 
                    pow(NdotHV, gl_FrontMaterial.shininess);*/
        }



        vec4 texel = texture2D(tex, gl_TexCoord[0].xy);
    
        gl_FragColor = texel * color;
}

