attribute highp   vec3  myVertex;
attribute mediump vec3  myNormal;

uniform highp   mat4  myViewProjMatrix;
uniform mediump mat3  myModelWorld;
uniform mediump vec3  myEyePosModel;

varying mediump vec3  varReflectDir;

void main()
{
	// Transform position
	gl_Position = myViewProjMatrix * vec4(myVertex, 1.0);
	
	// Calculate eye direction in model space
	mediump vec3 eyeDir = normalize(myVertex - myEyePosModel);
	
	// reflect eye direction over normal and transform to world space
	varReflectDir = myModelWorld * reflect(eyeDir, myNormal);
}