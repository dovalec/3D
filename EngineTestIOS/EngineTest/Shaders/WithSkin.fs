
uniform sampler2D sampler2d;
varying mediump vec2 varTexCoord;
void main (void)
{
	gl_FragColor = texture2D(sampler2d,varTexCoord);
} 