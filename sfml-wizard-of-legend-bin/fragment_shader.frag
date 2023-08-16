uniform sampler2D texture;
uniform sampler2D paletteTexture;

void main()
{
	vec4 grayColor = texture2D(texture, gl_TexCoord[0].xy);
	float grayValue = grayColor.r / 255.0;
	float paletteWidth = float(textureSize(paletteTexture).x);
	float paletteHeight = float(textureSize(paletteTexture).y);
	vec4 paletteColor = texture2D(paletteTexture, vec2(grayValue * (paletteWidth - 2.0) / paletteWidth, 0.5));
	gl_FragColor = paletteColor;
}