// TestImporter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cassert>

#include "FBXImporter.h"


std::vector<std::string> GetAllJointNames(TGA::FBXSkeleton& aSkeleton)
{
	std::vector<std::string> result;
	for (auto It = aSkeleton.BoneNameToIndex.begin(); It != aSkeleton.BoneNameToIndex.end(); ++It)
	{
		result.push_back(It->first);
	}

	return result;
}

std::string PadString(const std::string& aString, size_t size = 32)
{
	if (size <= aString.length())
		return aString;

	std::string result = aString;
	result.resize(size, ' ');
	return result;
}

void CompareFiles(const std::string& aFile, const std::string& anotherFile)
{


	TGA::FBXModel fbxModel;
	std::cout << "Trying to load" << aFile.c_str() << "....";
	if(TGA::FBXImporter::LoadModel(aFile, fbxModel))
	{
		std::cout << "Done!" << std::endl;

		std::cout << "Found " << std::to_string(fbxModel.Meshes.size()) << " meshes." << std::endl;
		for(size_t m = 0; m < fbxModel.Meshes.size(); m++)
		{
			std::cout << "Mesh " << std::to_string(m);
			if(!fbxModel.Meshes[m].MeshName.empty()) std::cout << " (" << fbxModel.Meshes[m].MeshName << ")";
			std::cout << " has " << std::to_string(fbxModel.Meshes[m].Vertices.size()) << " verts and " << std::to_string(fbxModel.Meshes[m].Indices.size()) << " indices. ";
			std::cout << "Uses material " << fbxModel.Materials[fbxModel.Meshes[m].MaterialIndex].MaterialName << " at index " << std::to_string(fbxModel.Meshes[m].MaterialIndex) << ". ";
			if(fbxModel.Skeleton.GetRoot())
			{
				std::cout << "Mesh has a skeleton";
				for(auto vert : fbxModel.Meshes[m].Vertices)
				{
					if(vert.BoneWeights[0] <= 0)
					{
						std::cout << " but some vertices have no skin weights" << std::endl;
						break;
					}
				}
				std::cout << ".";
			}			 	
		}

		std::cout << std::endl << std::endl;

		std::cout << "Trying to load" << anotherFile.c_str() << "....";
		TGA::FBXModel anotherFbxModel;
		if(TGA::FBXImporter::LoadModel(aFile, anotherFbxModel))
		{
			std::cout << "Done!" << std::endl;

			for(size_t m = 0; m < anotherFbxModel.Meshes.size(); m++)
			{
				std::cout << "Mesh " << std::to_string(m);
				if(!anotherFbxModel.Meshes[m].MeshName.empty()) std::cout << " (" << anotherFbxModel.Meshes[m].MeshName << ")";
				std::cout << " has " << std::to_string(anotherFbxModel.Meshes[m].Vertices.size()) << " verts and " << std::to_string(anotherFbxModel.Meshes[m].Indices.size()) << " indices. ";
				std::cout << "Uses material " << anotherFbxModel.Materials[fbxModel.Meshes[m].MaterialIndex].MaterialName << " at index " << std::to_string(anotherFbxModel.Meshes[m].MaterialIndex) << ". ";
				if(anotherFbxModel.Skeleton.GetRoot())
				{
					std::cout << "Mesh has a skeleton";
					for(auto vert : anotherFbxModel.Meshes[m].Vertices)
					{
						if(vert.BoneWeights[0] <= 0)
						{
							std::cout << " but some vertices have no skin weights" << std::endl;
							break;
						}
					}
					std::cout << ".";
				}			 	
			}

			std::cout << std::endl << std::endl;

			std::cout << "Comparing..." << std::endl;
			if(fbxModel.Name != anotherFbxModel.Name)
			{
				std::cout << "Names do not match." << std::endl;
			}
			else
			{
				std::cout << "Names match." << std::endl;
			}

			if(fbxModel.Meshes.size() != anotherFbxModel.Meshes.size())
			{
				std::cout << "They do not have the same amount of meshes." << std::endl;
			}
			else
			{
				std::cout << "Comparing meshes:" << std::endl;
				for(size_t m = 0; m < fbxModel.Meshes.size(); m++)
				{
					TGA::FBXModel::FBXMesh meshA = fbxModel.Meshes[m];
					TGA::FBXModel::FBXMesh meshB = anotherFbxModel.Meshes[m];

					if(meshA.MeshName != meshB.MeshName)
					{
						std::cout << "Mesh names do not match." << std::endl;
					}
					//if(meshA.Materials[fbxModel.Meshes[m].MaterialIndex].MaterialName != meshB.MaterialName)
					//{
					//	std::cout << "Material names do not match." << std::endl;
					//}
					if(meshA.MaterialIndex != meshB.MaterialIndex)
					{
						std::cout << "Material indices do not match." << std::endl;
					}
					if(meshA.Indices != meshB.Indices)
					{
						std::cout << "Indices do not match!" << std::endl;
					}
					if(meshA.Vertices != meshB.Vertices)
					{
						std::cout << "Vertices do not match!" << std::endl;
					}
				}
			}
		}
		else
		{
			std::cout << "Failed! Reason: " << TGA::FBXImporter::GetLastError() << std::endl;
		}
	}
	else
	{
		std::cout << "Failed! Reason: " << TGA::FBXImporter::GetLastError() << std::endl;
	}

	TGA::FBXImporter::UnloadImporter();
}

void Test(const std::string& SkeletalMeshFileName, const std::string& AnimationFileName)
{
	TGA::FBXModel fbxModel;
	std::cout << "Trying to load " << SkeletalMeshFileName.c_str() << std::endl;
	if(TGA::FBXImporter::LoadModel(SkeletalMeshFileName, fbxModel))
	{
	     std::vector<std::string> JointNames = GetAllJointNames(fbxModel.Skeleton);
	     if(!AnimationFileName.empty())
	     {
			 std::cout << "Found " << std::to_string(fbxModel.Meshes.size()) << " meshes." << std::endl;
			 for(size_t m = 0; m < fbxModel.Meshes.size(); m++)
			 {
			 	std::cout << "Mesh " << std::to_string(m);
			 	if(!fbxModel.Meshes[m].MeshName.empty()) std::cout << " (" << fbxModel.Meshes[m].MeshName << ")";
			 	std::cout << " has " << std::to_string(fbxModel.Meshes[m].Vertices.size()) << " verts and " << std::to_string(fbxModel.Meshes[m].Indices.size()) << " indices. ";
			 	std::cout << "Uses material " << fbxModel.Materials[fbxModel.Meshes[m].MaterialIndex].MaterialName << " at index " << std::to_string(fbxModel.Meshes[m].MaterialIndex) << ". ";
				if(fbxModel.Skeleton.GetRoot())
				{
					std::cout << "Mesh has a skeleton";
					for(auto vert : fbxModel.Meshes[m].Vertices)
					{
						if(vert.BoneWeights[0] <= 0)
						{
							std::cout << " but some vertices have no skin weights" << std::endl;
							break;
						}
					}
					std::cout << ".";
				}			 	
			 }

			 std::cout << std::endl << std::endl;

	         // Load the animation as a skeleton.
	         TGA::FBXModel fbxAnimTemp;
	         if(TGA::FBXImporter::LoadSkeleton(AnimationFileName, fbxAnimTemp))
	         {
	             // Verify the skeleton.
	         	std::cout << "Verifying skeleton against Anim File..." << std::endl;
			 	std::cout << "Num Joints: " << PadString(std::to_string(fbxModel.Skeleton.Bones.size()), 3) << ((fbxModel.Skeleton.Bones.size() == fbxAnimTemp.Skeleton.Bones.size()) ? " == " : " != ") << fbxAnimTemp.Skeleton.Bones.size() << std::endl;
			 	std::cout << "Joint Names:" << std::endl;

			 	// To print this with a nice formatting we need to find the longest string first
			 	size_t padding = 32;
			 	for(auto [first, second] : fbxModel.Skeleton.BoneNameToIndex)
			 	{
			 		if(first.size() > padding)
			 			padding = first.size();
			 	}

			 	if(fbxModel.Skeleton.Bones.size() >= fbxAnimTemp.Skeleton.Bones.size())
			 	{
			 		for(size_t j = 0; j < fbxModel.Skeleton.Bones.size(); j++)
			 		{
			 			std::cout << PadString(fbxModel.Skeleton.Bones[j].Name, padding);
			 			if(j < fbxAnimTemp.Skeleton.Bones.size())
			 			{
			 				std::cout << ((fbxModel.Skeleton.Bones[j].Name == fbxAnimTemp.Skeleton.Bones[j].Name) ? " == " : " != ") << fbxAnimTemp.Skeleton.Bones[j].Name;
			 			}
			 			else
			 			{
			 				std::cout << PadString(" != <Not found>", padding);
			 			}

			 			std::cout << std::endl;
			 		}
			 	}
			 	else
			 	{
			 		for(size_t j = 0; j < fbxAnimTemp.Skeleton.Bones.size(); j++)
			 		{
			 			std::cout << PadString(fbxModel.Skeleton.Bones[j].Name);
			 			if(j < fbxModel.Skeleton.Bones.size())
			 			{
			 				std::cout << PadString("<Not found>", padding);
			 				std::cout << " != " << fbxAnimTemp.Skeleton.Bones[j].Name;
			 			}
			 			else
			 			{
			 				std::cout << PadString(fbxModel.Skeleton.Bones[j].Name, padding);
			 				std::cout << ((fbxModel.Skeleton.Bones[j].Name == fbxAnimTemp.Skeleton.Bones[j].Name) ? " == " : " != ") << fbxAnimTemp.Skeleton.Bones[j].Name;
			 			}

			 			std::cout << std::endl;
			 		}
			 	}
	         }

	         TGA::FBXAnimation fbxAnimation;
	         TGA::FBXSkeleton fbxSkeleton;
		     if(!TGA::FBXImporter::LoadAnimation(AnimationFileName, JointNames, fbxAnimation))
		     {
		     	std::cout << "Error loading FBX Animation!" << std::endl;
			 	std::cout << TGA::FBXImporter::GetLastError() << std::endl;
		     }
	         //else
	         //{
	         //    TGA::FBXImporter::DumpJointTransforms(fbxModel, fbxAnimation, "shooter_rigg_18:pelvis");
	         //}
	     }
	 }
	 else
	 {
	     std::cout << "Error loading FBX Model!" << std::endl;
	     std::cout << TGA::FBXImporter::GetLastError() << std::endl;
	 }

	TGA::FBXImporter::UnloadImporter();
}

void GetVxAndWeightsForBone(const std::string& aFile, const std::string& aBoneName)
{
	TGA::FBXModel fbxModel;
	std::cout << "Trying to load " << aFile.c_str() << std::endl;
	if(TGA::FBXImporter::LoadModel(aFile, fbxModel))
	{
		size_t Idx = fbxModel.Skeleton.BoneNameToIndex.find(aBoneName)->second;
		for(size_t m = 0; m < fbxModel.Meshes.size(); m++)
		{
			std::cout << "Searching weights for bone " << aBoneName << " in " << fbxModel.Meshes[m].MeshName << "..." << std::endl;
			for(size_t v = 0; v < fbxModel.Meshes[m].Vertices.size(); v++)
			{
				if(fbxModel.Meshes[m].Vertices[v].BoneIDs[0] == Idx ||
					fbxModel.Meshes[m].Vertices[v].BoneIDs[1] == Idx ||
					fbxModel.Meshes[m].Vertices[v].BoneIDs[2] == Idx ||
					fbxModel.Meshes[m].Vertices[v].BoneIDs[3] == Idx)
				{
					std::cout << "Vx " << std::to_string(v) << " has bone data: {";
					std::cout << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneIDs[0]) << ": " << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneWeights[0]) << ", ";
					std::cout << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneIDs[1]) << ": " << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneWeights[1]) << ", ";
					std::cout << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneIDs[2]) << ": " << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneWeights[2]) << ", ";
					std::cout << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneIDs[3]) << ": " << std::to_string(fbxModel.Meshes[m].Vertices[v].BoneWeights[3]);
					std::cout << "}" << std::endl;
				}
			}
		}
	}

	TGA::FBXImporter::UnloadImporter();
}

int main()
{
	std::cout << "TGA FBX File Checker!" << std::endl;
	std::cout << "=====================" << std::endl;

	std::cout << std::endl;
	std::cout << "Verifying FBX files...\n";
	std::cout << std::endl;

	TGA::FBXModel Model1;
	TGA::FBXImporter::LoadModel("enemy_mod.fbx", Model1);

	//Test("flyingEnemy_sk.fbx", "flyingEnemy_anim_idle_WITH_MESH.fbx");

	//TGA::FBXModel failModel;
	//TGA::FBXImporter::LoadSkeleton("SM_Particle_Chest.fbx", failModel);

	//
	//TGA::FBXImporter::LoadModel("gremlin_sk.fbx", Model1);
	//Sleep(2000);
	//TGA::FBXModel Model2;
	//TGA::FBXImporter::LoadModel("gremlin_sk.fbx", Model2);
	//Sleep(2000);
	//TGA::FBXModel Model3;
	//TGA::FBXImporter::LoadModel("gremlin_sk.fbx", Model3);
	//Sleep(2000);
	//std::vector<std::string> Names = GetAllJointNames(Model1.Skeleton);

	//TGA::FBXAnimation Animation1;
	//TGA::FBXImporter::LoadAnimation("gremlin@walk.fbx", Names, Animation1);
	//Sleep(2000);

	//TGA::FBXAnimation Animation2;
	//TGA::FBXImporter::LoadAnimation("gremlin@walk.fbx", Names, Animation2);
	//Sleep(2000);

	//TGA::FBXAnimation Animation3;
	//TGA::FBXImporter::LoadAnimation("gremlin@walk.fbx", Names, Animation3);

	//Sleep(15000);

	std::cout << "Done!" << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
 