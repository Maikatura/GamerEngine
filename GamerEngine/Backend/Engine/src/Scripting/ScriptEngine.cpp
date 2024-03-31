#include "GraphicsEngine.pch.h"
#include "ScriptEngine.h"

#include <fstream>
#include "ScriptUtils.h"


struct ScriptEngineData
{
	MonoDomain* RootDomain = nullptr;
	MonoDomain* AppDomain = nullptr;

    MonoAssembly* CoreAssembly = nullptr;
};


static ScriptEngineData* myScriptData;



void GamerEngine::ScriptEngine::Init()
{
	myScriptData = new ScriptEngineData();
	InitMono();
}

void GamerEngine::ScriptEngine::Shutdown()
{
	delete myScriptData;
}

void GamerEngine::ScriptEngine::InitMono()
{
	mono_set_assemblies_path("mono/lib");

	MonoDomain* rootDomain = mono_jit_init("GamerJITRuntime");
	if (rootDomain == nullptr)
	{
		return;
	}

	myScriptData->RootDomain = rootDomain;


	std::string appDomain = "GamerScriptRuntime";
	myScriptData->AppDomain = mono_domain_create_appdomain(appDomain.data(), nullptr);
	mono_domain_set(myScriptData->AppDomain, true);

    myScriptData->CoreAssembly = GamerEngine::LoadCSharpAssembly("Resources/Scripts/Gamer-ScriptCore.dll");
	GamerEngine::PrintAssemblyTypes(myScriptData->CoreAssembly);



    MonoImage* assemblyImage = mono_assembly_get_image(myScriptData->CoreAssembly);
    MonoClass* monoClass = mono_class_from_name(assemblyImage, "", "Main");


    MonoObject* instance = mono_object_new(myScriptData->AppDomain, monoClass);
    mono_runtime_object_init(instance);

    MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
    mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);


    MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
    int value = 5;
    void* param = &value;
    mono_runtime_invoke(printIntFunc, instance, &param, nullptr);


    MonoString* monoString = mono_string_new(myScriptData->AppDomain, "Hello World from C++!");

    void* stringParam = monoString;
    MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
    mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);
}



void GamerEngine::ScriptEngine::ShutdownMono()
{
    //mono_domain_unload(myScriptData->AppDomain);
    myScriptData->AppDomain = nullptr;

    //mono_jit_cleanup(myScriptData->RootDomain);
    myScriptData->RootDomain = nullptr;
}
