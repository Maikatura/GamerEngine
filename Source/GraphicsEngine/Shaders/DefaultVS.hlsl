#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	float4 vertexPos = input.myPosition;


	float4x4 skinningMatrix = { 1,0,0,0,0, 1,0,0,0,0,1,0,0,0,0,1 };
	if(OB_HasBones)
	{
		skinningMatrix = 0;
		skinningMatrix += input.myBoneWeights.x * OB_BoneData[input.myBoneIDs.x];
		skinningMatrix += input.myBoneWeights.y * OB_BoneData[input.myBoneIDs.y];
		skinningMatrix += input.myBoneWeights.z * OB_BoneData[input.myBoneIDs.z];
		skinningMatrix += input.myBoneWeights.w * OB_BoneData[input.myBoneIDs.w];
	}

	// Move the vertex from Object Space (Local) to World space.
	// (Incidentally moving it from world to object space is the inverse of this)
	const float4 vertexWorldPosition = mul(OB_ToWorld, mul(input.myPosition, skinningMatrix));

	// Move the vertex from World to View Space (Camera space)
	const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);

	// And finally move the vertex from View Space to Clip space (Projection space).
	const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

	// Normals are in Local Space. We need to move them to World Space
	// and also account for the normal direction changes from animations.
	// Extract the rotation 3x3 from both matrices
	const float3x3 worldNormalRotation = (float3x3)OB_ToWorld;
	const float3x3 skinNormalRotation = (float3x3)skinningMatrix;

	result.myTangent = mul(worldNormalRotation, mul(input.myTangent, skinNormalRotation));
	result.myBinormal = mul(worldNormalRotation, mul(input.myBinormal, skinNormalRotation));
	result.myNormal = mul(worldNormalRotation, mul(input.myNormal, skinNormalRotation));
	// The resulting VX position is the position on screen.
	result.myPosition = vertexProjectionPosition;

	// We have to pass values through from here to use them
	// in the Pixel Shader.
	// Move the vertex colors over from the input.



	

	result.myVxColor = input.myVxColor;
	result.myVxColor2 = input.myVxColor2;
	result.myVxColor3 = input.myVxColor3;
	result.myVxColor4 = input.myVxColor4;

	result.myUV = input.myUV;
	result.myUV2 = input.myUV2;
	result.myUV3 = input.myUV3;
	result.myUV4 = input.myUV4;

	return result;
}

