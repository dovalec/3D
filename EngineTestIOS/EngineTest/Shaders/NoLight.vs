
attribute highp vec3	myVertex;
attribute mediump vec2	myUV;
uniform mediump mat4	myWorldMatrix;
uniform mediump mat4	myUVMtx;
uniform mediump mat4	myViewProjMatrix;
uniform mediump vec3	myLightDirection;
varying mediump vec2	varTexCoord;
varying lowp    float	varLightIntensity;

void main(void)
{
	vec4 uv4 = vec4(myUV.st,0,1);
	varTexCoord = (myUVMtx * uv4).xy;
	
	gl_Position =  myViewProjMatrix * vec4(myVertex, 1.0);
	//varTexCoord = myUV.st;
}
 