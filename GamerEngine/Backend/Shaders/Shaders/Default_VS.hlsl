#include "Data/ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	float4x4 skinningMatrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	if(OB_HasBones)
	{
		skinningMatrix = 0;
        skinningMatrix += mul(input.BoneWeights.x, OB_BoneData[input.BoneIDs.x]);
		skinningMatrix += mul(input.BoneWeights.y, OB_BoneData[input.BoneIDs.y]);
		skinningMatrix += mul(input.BoneWeights.z, OB_BoneData[input.BoneIDs.z]);
		skinningMatrix += mul(input.BoneWeights.w, OB_BoneData[input.BoneIDs.w]);
	}


    float4 vertexWorldPosition;
    if (OB_IsInstanced)
    {
        vertexWorldPosition = mul(input.Offset, mul(input.Position, skinningMatrix));
    }
    else
    {
        vertexWorldPosition = mul(OB_ToWorld, mul(input.Position, skinningMatrix));
    }

    float4 viewPosition = mul(FB_ToView, vertexWorldPosition);

    result.Position = mul(FB_ToProjection, viewPosition);
    result.ViewPosition = viewPosition.xyz;
    result.WorldNormal = mul((float3x3) OB_ToWorld, input.Normal); // Assuming Normal is in object space
    result.WorldPosition = vertexWorldPosition.xyz;


    float3x3 worldNormalRotation = (float3x3)OB_ToWorld;
    //if (OB_IsInstanced)
    //{
    //    //worldNormalRotation = (float3x3) instanceBuffer[instanceArgs[OB_StartInstanceOffset + 3] + input.InstanceID].Transform;
    //}
	
    float3x3 skinNormalRotation = (float3x3)skinningMatrix;

    result.Normal = mul(worldNormalRotation, mul(input.Normal, skinNormalRotation));
	result.Tangent = mul(worldNormalRotation, mul(input.Tangent, skinNormalRotation));
	result.Binormal	= mul(worldNormalRotation, mul(input.Binormal, skinNormalRotation));

	result.VxColor = input.VxColor;
	result.VxColor2 = input.VxColor2;
	result.VxColor3 = input.VxColor3;
	result.VxColor4 = input.VxColor4;
	result.UV = input.UV;
	result.UV2 = input.UV2;
	result.UV3 = input.UV3;
	result.UV4 = input.UV4;

	return result;
}

