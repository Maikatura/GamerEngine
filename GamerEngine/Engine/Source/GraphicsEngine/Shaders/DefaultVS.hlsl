#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	float4 vertexPos = input.myPosition;

	if(OB_HasBones)
	{
		vertexPos = 0;
		vertexPos += input.myBoneWeights.x * mul(input.myPosition, OB_BoneData[input.myBoneIDs.x]);
		vertexPos += input.myBoneWeights.y * mul(input.myPosition, OB_BoneData[input.myBoneIDs.y]);
		vertexPos += input.myBoneWeights.z * mul(input.myPosition, OB_BoneData[input.myBoneIDs.z]);
		vertexPos += input.myBoneWeights.w * mul(input.myPosition, OB_BoneData[input.myBoneIDs.w]);
	}

	// Move the vertex from Object Space (Local) to World space.
	// (Incidentally moving it from world to object space is the inverse of this)
	const float4 vertexWorldPosition = mul(OB_ToWorld, vertexPos);

	// Move the vertex from World to View Space (Camera space)
	const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);

	// And finally move the vertex from View Space to Clip space (Projection space).
	const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

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

