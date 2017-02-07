    
    uniform sampler2D tex;
    varying vec3 normal, lightDir;
    varying vec4 ambient, diffuse;


    void main(){

        float intensity_array[3];
        intensity_array[0] = 0.25;
        intensity_array[1] = 0.5;
        intensity_array[2] = 0.95;

        float minred = float((diffuse.r - ambient.r)/4.0);
        float mingreen = float((diffuse.g - ambient.g)/4.0);
        float minblue = float((diffuse.b - ambient.b)/4.0);

        float shades_red[4];
        shades_red[0] = minred; shades_red[1] = minred * 2.0; 
        shades_red[2] = minred * 3.0;  shades_red[3] = minred * 4.0;
        float shades_green[4];
        shades_green[0] = mingreen; shades_green[1] = mingreen * 2.0; 
        shades_green[2] = mingreen * 3.0;  shades_green[3] = mingreen * 4.0;
        float shades_blue[4];
        shades_blue[0] = minblue; shades_blue[1] = minblue * 2.0; 
        shades_blue[2] = minblue * 3.0;  shades_blue[3] = minblue * 4.0;


        float intensity = dot(lightDir, normalize(normal));

        vec3 color;
        if (intensity > intensity_array[2])
            color = vec3(ambient.r + shades_red[3],
                     ambient.g + shades_green[3],
                     ambient.b + shades_blue[3]);
        else if (intensity > intensity_array[1])
            color = vec3(ambient.r + shades_red[2],
                     ambient.g + shades_green[2],
                     ambient.b + shades_blue[2]); 
        else if (intensity > intensity_array[0])
           color = vec3(ambient.r + shades_red[1],
                     ambient.g + shades_green[1],
                     ambient.b + shades_blue[1]);
        else
            color = vec3(ambient.r + shades_red[0],
                     ambient.g + shades_green[0],
                     ambient.b + shades_blue[0]);

        vec4 texel = texture2D(tex, gl_TexCoord[0].xy);
        gl_FragColor = clamp((vec4(color, 1.0)) * texel, 0.0, 1.0);
    } 

