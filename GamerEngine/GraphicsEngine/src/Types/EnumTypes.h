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
	VertexColor1,
	VertexColor2,
	VertexColor3,
	VertexColor4,
	DiffuseLight,
	AmbientLight,
	AmbientOcclusion,
	Roughness,
	Metalness,
	Emissiveness,
	Diffuse,
	COUNT
};