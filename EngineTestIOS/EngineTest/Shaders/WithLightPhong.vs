


/****************************************************************************
* Vertex Shader
* This code is for educational purposes, not optimized for performance.
* For best performance, use different shaders for different light setups 
* and calculate lighting in model space.
*
* The use of several complex lights might significantly affect performance 
* on some mobile platforms.
*
* For fast transformation and lighting code, please see FastTnL example.
*****************************************************************************/

attribute highp vec3  myVertex; 
attribute highp vec3  myNormal;
attribute highp vec2  myUV;

uniform highp mat4  myViewProjMatrix;
uniform highp mat4  myModelView;
uniform highp mat3  myModelViewIT;
uniform lowp  int   myLightSel;
uniform highp vec3  myLightPosition;
uniform highp vec3  myLightDirection;
uniform lowp  vec3  myLightColor;

varying mediump vec2  varTexCoord;
varying lowp    vec3  varDiffuseLight;
varying lowp    vec3  varSpecularLight;

const highp float  cShininess = 30.0;
const highp float  cSpotCutoff = 0.9; 
const highp float  cSpotExp = 40.0;

// General Blinn-Phong lighting function
//
// Calculated light is added to the varyings varDiffuseLight and varSpecularLight
//
void Lighting(bool bSpecular, highp vec3 normal, highp vec3 eyeDir, highp vec3 lightDir, lowp vec3 lightColor)
{
	lowp float NdotL = max(dot(normal, lightDir), 0.0);
	varDiffuseLight += NdotL * lightColor;
	
	if (bSpecular && NdotL > 0.0)
	{
		highp vec3 halfVector = normalize(lightDir + eyeDir);
		highp float NdotH = max(dot(normal, halfVector), 0.0);		
		highp float specular = pow(NdotH, cShininess);
		varSpecularLight += specular * lightColor;
	}	
}

void DirectionalLight(bool bSpecular, highp vec3 normal, highp vec3 vertexPos)
{
	// eye direction is the normalized inverse of the vertex position in eye space
	highp vec3 eyeDir = -normalize(vertexPos);
	
	Lighting(bSpecular, normal, eyeDir, myLightDirection, myLightColor);
}

void PointLight(bool bSpecular, highp vec3 normal, highp vec3 vertexPos)
{
	// calculate normalized light direction
	highp vec3 lightDir = -normalize(vertexPos - myLightPosition);
	
	// eye direction is the normalized inverse of the vertex position in eye space
	highp vec3 eyeDir = -normalize(vertexPos);
	
	Lighting(bSpecular, normal, eyeDir, lightDir, myLightColor);
}

void SpotLight(bool bSpecular, highp vec3 normal, highp vec3 vertexPos)
{
	// calculate normalized light direction
	highp vec3 lightDir = -normalize(vertexPos - myLightPosition);
	
	// eye direction is the normalized inverse of the vertex position in eye space
	highp vec3 eyeDir = -normalize(vertexPos);
	
	// myLightDirection is spot direction here
	highp float spotDot = dot(lightDir, myLightDirection);
	highp float attenuation = 0.0;
	if (spotDot > cSpotCutoff)
	{
		attenuation = pow(spotDot, cSpotExp);
	}
	
	Lighting(bSpecular, normal, eyeDir, lightDir, attenuation * myLightColor);
}

void main()
{
	// transform normal to eye space
	highp vec3 normal = normalize(myModelViewIT * myNormal);
	
	// transform vertex position to eye space
	highp vec3 ecPosition = vec3(myModelView * vec4(myVertex, 1.0));
	
	// initalize light intensity varyings
	varDiffuseLight = vec3(0.0);
	varSpecularLight = vec3(0.0);
	
	// select the light function
	
	if (myLightSel == 0)		DirectionalLight(false, normal, ecPosition);
	else if (myLightSel == 1)	DirectionalLight(true, normal, ecPosition);
	else if (myLightSel == 2)	PointLight(false, normal, ecPosition);
	else if (myLightSel == 3)	PointLight(true, normal, ecPosition);
	else if (myLightSel == 4)	SpotLight(false, normal, ecPosition);
	else 						SpotLight(true, normal, ecPosition);
	
	// Transform position
	gl_Position = myViewProjMatrix * vec4(myVertex, 1.0);
	
	// Pass through texcoords
	varTexCoord = myUV;
}