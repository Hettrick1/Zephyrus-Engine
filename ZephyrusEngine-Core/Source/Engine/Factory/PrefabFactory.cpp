#include "PrefabFactory.h"
#include "rapidjson/document.h"
#include "Log.h"
#include "JSONUtils.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <filesystem>


namespace Zephyrus::Factory {
    PrefabFactory::PrefabFactory(ISceneContext* pSceneContext, ComponentFactory* pComponentFactory)
        : mSceneContext{pSceneContext}, mComponentFactory{ pComponentFactory }
    {
    }
    Actor* PrefabFactory::SpawnActorFromPrefab(Scene* pScene, const std::string& pPrefabName, const Vector3D& pInitialPos, const Vector3D& pInitialRot, const Vector3D& pInitialSize)
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

        Serialization::Json::JsonReader reader;
        if (!reader.LoadDocument(fullPath))
        {
            ZP_CORE_ERROR("Impossible to open or parse the prefab: " + fullPath);
            return nullptr;
        }

        auto actor = new Actor(mSceneContext, *pScene);
        actor->SetPrefab(pPrefabName);

        actor->Deserialize(reader);

        if (reader.BeginObjectArray("components"))
        {
            while (reader.NextObjectElement())
            {
                CreateAndAttachComponent(reader, actor);
            }
            reader.EndObjectArray();
        }

        ZP_LOAD("Prefab " + actor->GetName() + " loaded");

        actor->SetPosition(pInitialPos);
        actor->SetRotation(Quaternion(pInitialRot));
        actor->SetSize(pInitialSize);

        pScene->AddActor(actor);
        return actor;
    }

    Actor* PrefabFactory::InitPrefab(Scene* pScene, const std::string& pPrefabName)
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

        Serialization::Json::JsonReader reader;
        if (!reader.LoadDocument(fullPath))
        {
            ZP_CORE_ERROR("Impossible to open or parse the prefab: " + fullPath);
            return nullptr;
        }

        auto actor = new Actor(mSceneContext, *pScene);
        actor->SetPrefab(pPrefabName);

        actor->Deserialize(reader);

        if (reader.BeginObjectArray("components"))
        {
            while (reader.NextObjectElement())
            {
                CreateAndAttachComponent(reader, actor, false);
            }
            reader.EndObjectArray();
        }

        ZP_LOAD("Prefab " + actor->GetName() + " loaded");

        pScene->AddActor(actor);
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

    Component* PrefabFactory::CreateAndAttachComponent(Serialization::IDeserializer& reader,Actor* actor, bool doDeserialize)
    {
        auto typeOpt = reader.ReadString("type");
        if (!typeOpt)
        {
            ZP_EDITOR_ERROR("Component type is missing !");
            return nullptr;
        }
        std::string type = *typeOpt;

        Component* c = mComponentFactory->Create(type, actor);
        if (!c)
        {
            ZP_EDITOR_ERROR("Component " + type + " is invalid !");
            return nullptr;
        }

        if (auto id = reader.ReadString("componentId"))
        {
            c->SetId(*id);
        }

        if (doDeserialize && reader.BeginObject("properties"))
        {
            c->Deserialize(reader);
            reader.EndObject();
        }

        actor->AddComponent(c);
        ZP_CORE_LOAD("Component " + type + " loaded and attached to " + actor->GetName());

        return c;
    }
}