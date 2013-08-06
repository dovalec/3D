
uniform sampler2D sampler2d;
varying mediump vec2 varTexCoord;
varying lowp	float  varLightIntensity; 
void main (void)
{
	gl_FragColor = texture2D(sampler2d,varTexCoord) * varLightIntensity;
} 