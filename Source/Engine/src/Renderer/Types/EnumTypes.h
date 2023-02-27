#pragma once

enum class FileType
{
	Error,
	Folder,
	FBX,
	Shader,
	Model,
	Texture,
	Audio,
	Scene,
	Animation,
	SceneObject,
	Prefab,
	DLL,
	Text,
	Node,


	COUNT
};

enum class RenderMode
{
	Default,
	UV1,
	VertexColor,
	VertexNormal,
	PixelNormal,
	AlbedoMap,
	NormalMap,
	DirectLight,
	AmbientLight,
	PointLight,
	SpotLight,
	AmbientOcclusion,

	Roughness,
	Metalness,
	Emissiveness,
	Diffuse,
	Luminance,
	ViewNormal,
	ViewPosition,
	COUNT
};