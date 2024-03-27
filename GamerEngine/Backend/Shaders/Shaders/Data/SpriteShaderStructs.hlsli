struct SpriteVertexData
{
	float3 myPosition		:	POSITION;
	float myZRotation		:	ROTATION;
	float2 myScale			:	SCALE;
	float2 myPivot			:	CENTER;
	float4 myColor			:	COLOR;
};

struct SpriteGeometryToPixel
{
	float4 myPosition		:	SV_POSITION;
	float4 myColor			:	COLOR;
	float2 myUV				:   TEXCOORD;
};

struct SpritePixelOutput
{
	float4 myColor			:	SV_TARGET;
};

Texture2D particleTexture		:	register(t0);