#include "SceneFactory.h"
#include "PrefabFactory.h"
#include "ComponentFactory.h"
#include "JSONUtils.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "SceneManager.h"

bool SceneFactory::PopulateSceneFromFile(const std::string& pFilePath)
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

    SceneManager::ActiveScene->SetFilePath(pFilePath);

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
                    auto ids = actorPrefab->GetComponentsIds();
                    for (auto component : *actorComponents)
                    {
                        std::string id;
                        if (auto componentId = Serialization::Json::ReadString(*component, "componentId"))
                        {
                            id = *componentId;
                        }
                        if (auto componentProperties = Serialization::Json::ReadObject(*component, "properties"))
                        {
                            auto c = actorPrefab->GetComponentWithId(id);
                            // if found = component is still there
                            if (c)
                            {
                                c->Deserialize(*componentProperties);
                                ids.erase(std::remove(ids.begin(), ids.end(), id), ids.end());
                            }
                            // if the id is not found (component has been added)
                            else 
                            {
                                PrefabFactory::CreateAndAttachComponent(*component, actorPrefab);
                            }
                        }
                    }
                    // if there is still an id in the prefab but not in the scene (component has been deleted)
                    if (!ids.empty())
                    {
                        for (auto id : ids)
                        {
                            auto c = actorPrefab->GetComponentWithId(id);
                            if (c)
                            {
                                c->OnEnd();
                                actorPrefab->RemoveComponent(c);
                                delete c;
                            }
                        }
                    }
                }

                if (prefabName == "PlayerStart")
                {
                    SceneManager::ActiveScene->SetPlayerStart(actorPrefab);
                }
                SceneManager::ActiveScene->AddActor(actorPrefab);
            }
        }
    }

    return true;
}
