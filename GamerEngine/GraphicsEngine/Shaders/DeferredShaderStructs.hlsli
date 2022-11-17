struct DeferredVertexInput
{
	unsigned int myIndex	:	SV_VertexID;
};

struct DeferredVertexToPixel
{
	float4 myPosition		:	SV_POSITION;
	float2 myUV				:	TEXCOORD;
};

struct DeferredPixelOutput
{
	float4 myColor			:	SV_TARGET;
};

