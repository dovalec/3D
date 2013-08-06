
uniform			sampler2D sampler2d;

varying mediump vec2  varTexCoord;
varying lowp    vec3  varDiffuseLight;
varying lowp    vec3  varSpecularLight;

void main()
{
    lowp vec3 texColor  = vec3(texture2D(sampler2d, varTexCoord));
	lowp vec3 color = (texColor * varDiffuseLight) + varSpecularLight;
	gl_FragColor = vec4(color, 1.0);
}
