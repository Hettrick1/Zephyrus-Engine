#include "PrefabFactory.h"
#include "rapidjson/document.h"
#include "Log.h"
#include "ComponentFactory.h"
#include "JSONUtils.h"
#include "SceneManager.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <filesystem>

Actor* PrefabFactory::SpawnActorFromPrefab(const std::string& pPrefabName, const Vector3D& pInitialPos, const Vector3D& pInitialRot, const Vector3D& pInitialSize)
{
    std::string fullPath;
    // TODO -> only work with full path, no longer spawn prefab with name
    if (pPrefabName.find(".prefab") == std::string::npos)
    {
        fullPath = "../Content/Prefabs/" + pPrefabName + ".prefab";
    }
    else
    {
        fullPath = pPrefabName;
    }

    std::ifstream file(fullPath);

    if (!file.is_open())
    {
        ZP_CORE_ERROR("Impossible to open the prefab : " + fullPath);
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());

    if (doc.HasParseError()) {
        ZP_CORE_ERROR(pPrefabName + " Parsing JSON failed !");
        return nullptr;
    }

    auto actor = new Actor();
    actor->SetPrefab(pPrefabName);

    actor->Deserialize(doc);

    // creates all the components
    if (auto arr = Serialization::Json::ReadArrayObject(doc, "components"))
    {
        for (auto& component : *arr)
        {
            CreateAndAttachComponent(*component, actor);
        }
    }

    ZP_LOAD("Prefab " + actor->GetName() + " loaded");

    actor->SetPosition(pInitialPos);
    actor->SetRotation(Quaternion(pInitialRot));
    actor->SetSize(pInitialSize);

    SceneManager::ActiveScene->AddActor(actor);
    return actor;
}

Actor* PrefabFactory::InitPrefab(const std::string& pPrefabName)
{
    std::string fullPath;

    // TODO -> only work with full path, no longer spawn prefab with name
    if (pPrefabName.find(".prefab") == std::string::npos)
    {
        fullPath = "../Content/Prefabs/" + pPrefabName + ".prefab";
    }
    else
    {
        fullPath = pPrefabName;
    }

    std::ifstream file(fullPath);

    if (!file.is_open())
    {
        ZP_CORE_ERROR("Impossible to open the prefab : " + fullPath);
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());

    if (doc.HasParseError()) {
        ZP_CORE_ERROR(pPrefabName + " Parsing JSON failed !");
        return nullptr;
    }

    auto actor = new Actor();
    actor->SetPrefab(pPrefabName);

    actor->Deserialize(doc);

    // creates all the components
    if (auto arr = Serialization::Json::ReadArrayObject(doc, "components"))
    {
        for (auto& component : *arr)
        {
            CreateAndAttachComponent(*component, actor, false);
        }
    }

    ZP_LOAD("Prefab " + actor->GetName() + " loaded");
    return actor;
}

std::vector<std::string> PrefabFactory::GetPrefabFiles(const std::string& folderPath)
{
    std::vector<std::string> prefabs;
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            auto path = entry.path();
            if (path.extension() == ".prefab")
            {
                prefabs.push_back(path.stem().string());
            }
        }
    }
    return prefabs;
}

Component* PrefabFactory::CreateAndAttachComponent(const rapidjson::Value& componentJson, Actor* actor, bool doDeserialize)
{
    std::string type = *Serialization::Json::ReadString(componentJson, "type");
    Component* c = ComponentFactory::Instance().Create(type, actor);

    if (!c) {
        ZP_EDITOR_ERROR("Component " + type + " is invalid !");
        return nullptr;
    }
    if (auto id = Serialization::Json::ReadString(componentJson, "componentId"))
    {
        c->SetId(*id);
    }
    if (doDeserialize)
    {
        if (auto properties = Serialization::Json::ReadObject(componentJson, "properties")) {
            // sets all the components properties from the prefab file
            c->Deserialize(*properties);
        }
    }

    actor->AddComponent(c);
    ZP_CORE_LOAD("Component " + type + " loaded and attached to " + actor->GetName());
    return c;
}
