
	
attribute highp vec3	myVertex;
attribute mediump vec3	myNormal;
attribute mediump vec2	myUV;
uniform mediump mat4	myWorldMatrix;
uniform highp mat3		myModelViewIT;
uniform mediump mat4	myUVMtx;
uniform mediump mat4	myViewProjMatrix;
uniform mediump vec3	myLightDirection;
varying mediump vec2	varTexCoord;
varying lowp    float	varLightIntensity;
void main(void)
{
	gl_Position =  myViewProjMatrix * vec4(myVertex, 1.0);
	highp vec3 normal = normalize(myModelViewIT * myNormal);
	
	varLightIntensity = dot(normal, myLightDirection);
	
	vec4 uv4 = vec4(myUV.st,0,1);
	varTexCoord = (uv4 * myUVMtx).xy;
	
	//varTexCoord = myUV.st;
}