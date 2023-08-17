uniform sampler2D texture;
uniform sampler2D paletteTexture;

void main()
{
    vec4 pixel = texture2D(paletteTexture, gl_TexCoord[0].xy);
    


    gl_FragColor = vec4(1, 1, 0, 1);
}