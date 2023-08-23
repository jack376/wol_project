uniform sampler2D texture;
uniform sampler2D paletteTexture;
uniform float paletteYOffset;

void main() 
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    float brightness = color.r;

    vec4 paletteColor = texture2D(paletteTexture, vec2(brightness, paletteYOffset));
    paletteColor.a = color.a;
    gl_FragColor = paletteColor;
}
