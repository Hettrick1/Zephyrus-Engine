#include "PrefabFactory.h"
#include "rapidjson/document.h"
#include "Log.h"
#include "ComponentFactory.h"
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
        ZP_CORE_ERROR("Parsing JSON failed !");
        return nullptr;
    }

    std::string actorName = doc.HasMember("name") ? doc["name"].GetString() : "UnnamedActor";

    auto actor = new EmptyActor();
    actor->SetName(actorName);

    if (doc.HasMember("components") && doc["components"].IsArray()) {
        for (auto& comp : doc["components"].GetArray()) {
            std::string type = comp["type"].GetString();
            Component* c = ComponentFactory::Instance().Create(type, actor);

            if (c) {
                if (comp.HasMember("properties")) {
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

    ZP_CORE_LOAD("Prefab " + actorName + " loaded");
    return actor;
}
