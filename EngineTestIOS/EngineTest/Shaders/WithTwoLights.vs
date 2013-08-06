
	
attribute highp vec3	myVertex;
attribute mediump vec3	myNormal;
attribute mediump vec2	myUV;
uniform mediump mat4	myWorldMatrix;
uniform highp mat3		myModelViewIT;
uniform mediump mat4	myViewProjMatrix;
uniform mediump vec3	myLightDirection1;
uniform mediump vec3	myLightDirection2;
varying mediump vec2	varTexCoord;
varying lowp    float	varLightIntensity;
void main(void)
{
	gl_Position =  myViewProjMatrix * vec4(myVertex, 1.0);
	vec3 normal = normalize(myModelViewIT * myNormal);
	
	varTexCoord = myUV.st;
	varLightIntensity = max( 0.55  , 0.4 * (  dot(normal, myLightDirection1) + dot(normal, myLightDirection2) ));
	//varLightIntensity = 0.5 * (  dot(normal, myLightDirection1) + dot(normal, myLightDirection2) );
}