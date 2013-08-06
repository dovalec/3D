
attribute	highp		vec3	myVertex; 
attribute	mediump		vec3	myNormal; 
attribute	mediump		vec2	myUV; 
attribute	highp		vec4	myBoneWeights; 
attribute	mediump		vec4	myBoneIndex; 
uniform		mediump		mat4	myViewProjMatrix; 
uniform		highp		mat4	myBoneMatrixArray[12]; 

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
	
	gl_Position = myViewProjMatrix * position; 
	varTexCoord = myUV.st; 
}