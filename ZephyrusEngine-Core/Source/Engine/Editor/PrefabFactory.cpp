#include "PrefabFactory.h"
#include "rapidjson/document.h"
#include "Log.h"
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

    ZP_CORE_LOAD("Prefab " + actorName + " loaded");
    return actor;
}
