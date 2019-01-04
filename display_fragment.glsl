#version 120
uniform sampler2D texture;
uniform float zoom;
uniform vec2 offset;

//applys zoom and offset and renders everything to the screen
void main(void)
{
    vec2 UV=gl_TexCoord[0].xy-vec2(.5);
    UV/=zoom;
    UV+=(offset);
    gl_FragColor=texture2D(texture,UV);
}
