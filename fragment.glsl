#version 120
uniform sampler2D texture;
uniform float resolution;

//returns the state of the current texel + x,y. We just need the states "alive" or "dead".
//so we just return an integer 0 or 1
int get(float x, float y)
{
    return int(texture2D(texture, (gl_TexCoord[0].xy + vec2(x, y)/resolution)).b);
}

void main(void)
{
    //count the "living" neighbour texels
    int sum = get(-1, -1) +
              get(-1,  0) +
              get(-1,  1) +
              get( 0, -1) +
              get( 0,  1) +
              get( 1, -1) +
              get( 1,  0) +
              get( 1,  1);

    //if we have 3 living neighbours the current cell will live, if there are two,
    //we keep the current state. Otherwise the cell is dead.
    if (sum==3)
    {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else if (sum== 2)
    {
        float current = float(get(0, 0));
        gl_FragColor = vec4(current, current, current, 1.0);
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
