#include "SceneFactory.h"
#include "PrefabFactory.h"
#include "ComponentFactory.h"
#include "JSONUtils.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "SceneManager.h"

bool SceneFactory::CreateSceneFromFile(const std::string& pFilePath)
{
    if (SceneManager::ActiveScene == nullptr)
    {
        ZP_CORE_ERROR("SceneManager no active scene created by the manager!");
        return false;
    }

    std::ifstream file(pFilePath);

    if (!file.is_open())
    {
        ZP_CORE_ERROR("Impossible to open the Scene : " + pFilePath);
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());

    if (doc.HasParseError()) {
        ZP_CORE_ERROR(pFilePath + " Parsing JSON failed !");
        return false;
    }

    if (auto actorArray = Serialization::Json::ReadArrayObject(doc, "actors"))
    {
        for (auto& actor : *actorArray)
        {
            if (auto prefabName = Serialization::Json::ReadString(*actor, "prefabName"))
            {
                auto actorPrefab = PrefabFactory::CreateActorFromPrefab(*prefabName);
                actorPrefab->Deserialize(*actor);
                if (auto actorComponents = Serialization::Json::ReadArrayObject(*actor, "components"))
                {
                    for (auto component : *actorComponents)
                    {
                        std::string id;
                        if (auto componentId = Serialization::Json::ReadString(*component, "componentId"))
                        {
                            id = *componentId;
                        }
                        if (auto componentProperties = Serialization::Json::ReadObject(*component, "properties"))
                        {
                            actorPrefab->GetComponentWithId(id)->Deserialize(*componentProperties);
                        }
                    }
                }
            }
        }
    }

    return true;
}
