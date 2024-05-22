#pragma once
#include <Layers/Layer.h>
#include "Math/MathTypes.hpp"
#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"



class EditorSettingsPanel : public Layer
{
public:

	EditorSettingsPanel();

	void OnAttach() override;
	void OnImGuiRender() override;
	void OnUpdate() override;

	bool OnDetach() override;


	

    static void LoadConfig();
	static void SaveConfig();

	static void SaveVector4File(const std::string& aPath, Vector4f aColor);
	static void SaveVector3File(const std::string& aPath, Vector3f aColor);
	static void LoadVector4File(const std::string& aPath, Vector4f& aColor);
	static void LoadVector3File(const std::string& aPath, Vector3f& aColor);

	static void WriteVector4(rapidjson::Writer<rapidjson::OStreamWrapper>& aWriter,const std::string& aName, Vector4f aVector4);
	static void WriteVector3(rapidjson::Writer<rapidjson::OStreamWrapper>& aWriter,const std::string& aName, Vector3f aVector3);

	template<typename T>
	static Vector4<T> LoadVector4(rapidjson::Document& aDocument,const std::string& aName);

	template<typename T>
	static Vector3<T> LoadVector3(rapidjson::Document& aDocument, const std::string& aName);
	

private:

	void RenderEditorSettings();
	void RenderSceneSettings();
	void RenderDebugSettings();

	inline static const std::string myConfigPath = "Editor\\Settings\\";

	int myCurrentTab = 0;


};

