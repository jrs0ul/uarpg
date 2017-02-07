uniform sampler2D texture0;
void main(){
    vec4 texel = texture2D(texture0, gl_TexCoord[0].xy);
    gl_FragColor = clamp(gl_Color * texel, 0.0, 1.0);

}

