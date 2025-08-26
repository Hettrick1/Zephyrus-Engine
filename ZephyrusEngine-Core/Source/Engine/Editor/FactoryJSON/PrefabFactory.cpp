#include "PrefabFactory.h"
#include "rapidjson/document.h"
#include "Log.h"
#include "ComponentFactory.h"
#include "FactoryJSON/JSONUtils.h"
#include "Scene.h"
#include <fstream>
#include <sstream>

EmptyActor* PrefabFactory::CreateActorFromPrefab(const std::string& pPrefabName)
{
    std::string fullPath = "../Content/Prefabs/" + pPrefabName + ".prefab";
    
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

    std::string actorName = doc.HasMember("name") ? doc["name"].GetString() : "UnnamedActor";

    auto actor = new EmptyActor();
    actor->SetName(actorName);

    // sets the actor state
    if (doc.HasMember("state") && doc["state"].IsString()) {
        std::string stateStr = doc["state"].GetString();
        actor->SetActive(StringToActorState(stateStr));
    }

    // sets the actor transform
    if (doc.HasMember("transform")) { 
        const auto& transform = doc["transform"];
        if (auto pos = ReadVector3D(transform, "position"))
        {
            actor->SetPosition(*pos);
        }

        if (auto size = ReadVector3D(transform, "size"))
        {
            actor->SetSize(*size);
        }

        if (auto rot = ReadVector3D(transform, "rotation"))
        {
            actor->SetRotation(Quaternion(*rot));
        }
    }

    if (doc.HasMember("tags") && doc["tags"].IsArray())
    {
        const auto& arr = doc["tags"].GetArray();

        if (!arr.Empty())
        {
            for (auto& element : arr)
            {
                if (element.IsString())
                {
                    actor->AddTag(element.GetString());
                }
            }
        }
    }

    // creates all the components
    if (doc.HasMember("components") && doc["components"].IsArray()) {
        for (auto& comp : doc["components"].GetArray()) {
            std::string type = comp["type"].GetString();
            Component* c = ComponentFactory::Instance().Create(type, actor);

            if (c) {
                if (comp.HasMember("properties")) {
                    // sets all the components properties from the prefab file
                    c->Deserialize(comp["properties"]);
                }
                actor->AddComponent(c);
                ZP_CORE_LOAD("Component " + type + " loaded and attached to " + actorName);
            }
            else
            { 
                ZP_CORE_ERROR("Component is invalid !");
            }
        }
    }

    actor->Start();

    ZP_LOAD("Prefab " + actorName + " loaded");
    return actor;
}
