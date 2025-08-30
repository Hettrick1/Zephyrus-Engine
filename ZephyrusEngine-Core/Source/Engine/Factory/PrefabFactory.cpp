#include "PrefabFactory.h"
#include "rapidjson/document.h"
#include "Log.h"
#include "ComponentFactory.h"
#include "JSONUtils.h"
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
    if (auto state = Serialization::Json::ReadString(doc, "state"))
    {
        std::string stateStr = *state;
        actor->SetActive(StringToActorState(stateStr));
    }

    // sets the actor transform
    if (auto transform = Serialization::Json::ReadObject(doc, "transform"))
    {
        if (auto pos = Serialization::Json::ReadVector3D(*transform, "position"))
        {
            actor->SetPosition(*pos);
        }

        if (auto size = Serialization::Json::ReadVector3D(*transform, "size"))
        {
            actor->SetSize(*size);
        }

        if (auto rot = Serialization::Json::ReadVector3D(*transform, "rotation"))
        {
            actor->SetRotation(Quaternion(*rot));
        }
    }

    if (auto arr = Serialization::Json::ReadArrayString(doc, "tags"))
    {
        for (auto& element : *arr)
        {
            actor->AddTag(element);
        }
    }

    // creates all the components
    if (auto arr = Serialization::Json::ReadArrayObject(doc, "components"))
    {
        for (auto& component : *arr)
        {
            CreateAndAttachComponent(*component, actor);
        }
    }

    ZP_LOAD("Prefab " + actorName + " loaded");
    return actor;
}

Component* PrefabFactory::CreateAndAttachComponent(const rapidjson::Value& componentJson, EmptyActor* actor)
{
    std::string type = *Serialization::Json::ReadString(componentJson, "type");
    Component* c = ComponentFactory::Instance().Create(type, actor);

    if (!c) {
        ZP_CORE_ERROR("Component " + type + " is invalid !");
        return nullptr;
    }
    if (auto properties = Serialization::Json::ReadObject(componentJson, "properties")) {
        // sets all the components properties from the prefab file
        c->Deserialize(*properties);
    }

    actor->AddComponent(c);
    ZP_CORE_LOAD("Component " + type + " loaded and attached to " + actor->GetName());
    return c;
}
