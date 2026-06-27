
uniform sampler2D texture; uniform float time;        
void main()
{
        vec2 pos = gl_TexCoord[0].xy;

                float swayAmount = 0.008;     float speed = 3.0;       
        float wave = sin(time * speed) * swayAmount * (1.0 - pos.y);

        pos.x += wave;

        gl_FragColor = texture2D(texture, pos);
}