#include "pch.h"
#include "SceneFactory.h"
#include "PrefabFactory.h"
#include "JSONUtils.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "SceneManager.h"

namespace Zephyrus::Factory {
    SceneFactory::SceneFactory(ISceneContext* pSceneContext)
        : mSceneContext{pSceneContext}
    {
    }
    bool SceneFactory::PopulateSceneFromFile(Scene* pSceneRef, const std::string& pFilePath)
    {
        if (pSceneRef == nullptr)
        {
            ZP_CORE_ERROR("SceneManager no active scene created by the manager!");
            return false;
        }

        Serialization::Json::JsonReader reader;
        if (!reader.LoadDocument(pFilePath))
        {
            ZP_CORE_ERROR("Impossible to open or parse the Scene : " + pFilePath);
            return false;
        }

        pSceneRef->SetFilePath(pFilePath);

        if (reader.BeginObjectArray("actors"))
        {
            while (reader.NextObjectElement())
            {
                auto prefabName = reader.ReadString("prefabName");
                if (!prefabName) continue;

                auto actorPrefab = mSceneContext->GetPrefabFactory()->InitPrefab(mSceneContext->GetActiveScene(), *prefabName);
                actorPrefab->Deserialize(reader);

                if (reader.BeginObjectArray("components"))
                {
                    auto ids = actorPrefab->GetComponentsIds();
                    while (reader.NextObjectElement())
                    {
                        std::string id;
                        if (auto componentId = reader.ReadString("componentId"))
                        {
                            id = *componentId;
                        }
                        
                        auto c = actorPrefab->GetComponentWithId(id);
                        if (c)
                        {
                            if (auto parentId = reader.ReadString("parentID"))
                            {
                                mSceneContext->GetPrefabFactory()->AddParentToAttach(c, *parentId);
                            }

                            if (auto name = reader.ReadString("componentName"))
                            {
                                c->SetName(*name);
                            }
                        
                            if (reader.BeginObject("properties"))
                            {
                                // if found = component is still there
                                c->Deserialize(reader);
                                ids.erase(std::remove(ids.begin(), ids.end(), id), ids.end());
                                reader.EndObject();
                            }
                        }
                        else
                        {
                            // if the id is not found (component has been added)
                            c = mSceneContext->GetPrefabFactory()->CreateAndAttachComponent(reader, actorPrefab);
                        }
                    }

                    reader.EndObjectArray();

                    // if there is still an id in the prefab but not in the scene (component has been deleted)
                    if (!ids.empty())
                    {
                        for (auto leftoverId : ids)
                        {
                            if (auto c = actorPrefab->GetComponentWithId(leftoverId))
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
                    pSceneRef->SetPlayerStart(actorPrefab);
                }
                mSceneContext->GetPrefabFactory()->AttachComponentToParent(actorPrefab);
            }
        }

        return true;
    }
}