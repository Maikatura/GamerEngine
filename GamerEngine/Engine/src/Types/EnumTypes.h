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
	SSAO,
	Metalness,
	Roughness,
	Emission,
	DirectLightNoAlbedo,
	AmbientLightNoAlbedo,
	PointLightNoAlbedo,
	SpotLightNoAlbedo,
	COUNT
};


enum class VREye 
{
	None,
	Left,
	Right
};