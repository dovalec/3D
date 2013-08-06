
attribute	highp		vec3	myVertex; 
attribute	mediump		vec3	myNormal; 
attribute	mediump		vec2	myUV; 
attribute	highp		vec4	myBoneWeights; 
attribute	mediump		vec4	myBoneIndex; 
uniform		highp		mat3	myModelViewIT;
uniform		mediump		mat4	myViewProjMatrix; 
uniform		highp		mat4	myBoneMatrixArray[12]; 
uniform		mediump		vec3	myLightDirection1;
uniform		mediump		vec3	myLightDirection2;
varying		lowp		float	varLightIntensity;

varying mediump vec2	varTexCoord; 

void main(void)
{
	mediump ivec4 boneIndex = ivec4(myBoneIndex); 
	mediump vec4 boneWeights = myBoneWeights; 
	highp vec4 vert = vec4(myVertex, 1.0);
	
	highp vec4 position = myBoneMatrixArray[boneIndex.x] * vert * boneWeights.x + 
						  myBoneMatrixArray[boneIndex.y] * vert * boneWeights.y + 
						  myBoneMatrixArray[boneIndex.z] * vert * boneWeights.z + 
						  myBoneMatrixArray[boneIndex.w] * vert * boneWeights.w;  
						  
	highp vec3 normal = normalize(myModelViewIT * myNormal);
	
	gl_Position = myViewProjMatrix * position; 
	//varLightIntensity = 0.5 * (  dot(normal, myLightDirection1) + dot(normal, myLightDirection2) );
	varLightIntensity = max( 0.55  , 0.4 * (  dot(normal, myLightDirection1) + dot(normal, myLightDirection2) ));
	varTexCoord = myUV.st; 
}