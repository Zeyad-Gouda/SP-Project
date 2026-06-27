// codes get impelemented in the GUI not in The CPU (for animation)
#version 120
uniform sampler2D texture;
uniform float time;

void main() {
    // ???????? ?????? (?? 0 ?? 1)
    vec2 tc = gl_TexCoord[0].xy;
    
    // equation of motion (animation of weeds)
    float sway = sin(time * 0.8 + tc.y * 6.0) * 0.0035;

    // put the new pixels in the new position
    tc.x += sway;
    
    // Draw the new pixels
    gl_FragColor = texture2D(texture, tc);
}