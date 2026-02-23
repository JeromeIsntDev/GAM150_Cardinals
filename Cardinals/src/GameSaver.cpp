#include <document.h>
#include "../header/GameSaver.hpp"

GameObject DeserializeGameObject(const rapidjson::Value& v) {
    GameObject obj;

    if (   v.HasMember("id")
        && v.HasMember("posx") && v.HasMember("posy") 
        && v.HasMember("rot")
        && v.HasMember("scalex") && v.HasMember("scaley")
        )
    {
        obj.SetID(v["id"].GetUint64());
        obj.UpdatePosition(AEVec2{ v["posx"].GetFloat(), v["posy"].GetFloat() });
        obj.UpdateRotation(v["rot"].GetFloat());
        obj.UpdateScale(AEVec2{ v["scalex"].GetFloat(), v["scaley"].GetFloat() });
    }

    return obj;
}

std::vector<GameObject> ReadJsonToObjects(const char* filePath)
{
    std::vector<GameObject> gameObjectArray{};
	std::ifstream inputFileStream{ filePath };
    if (!inputFileStream.is_open()) {
        std::cerr << "Error: file cannot open.";
        return gameObjectArray;
    }

    rapidjson::IStreamWrapper isw(inputFileStream);
 	rapidjson::Document doc;
	doc.ParseStream(isw);
    if (doc.HasParseError()) {
        std::cerr << "Error: parse error.";
        return gameObjectArray;
    }

	inputFileStream.close();
  
    if (doc.HasMember("gameObjects")
        && doc["gameObjects"].IsArray()) {
        const auto& array = doc["gameObjects"];
        for (const auto& v : array.GetArray()) {
            GameObject obj = DeserializeGameObject(v);
            gameObjectArray.push_back(obj);
        }
    }
    return gameObjectArray;
}

rapidjson::Value SerializeGameObject(
    const GameObject* obj,
    rapidjson::Document::AllocatorType& allocator
) {
    rapidjson::Value v(rapidjson::kObjectType);

    v.AddMember("id",     obj->GetID(), allocator);
    v.AddMember("posx",   obj->GetPosition().x, allocator);
    v.AddMember("posy",   obj->GetPosition().y, allocator);
    v.AddMember("rot",    obj->GetRotation(), allocator);
    v.AddMember("scalex", obj->GetScale().x, allocator);
    v.AddMember("scaley", obj->GetScale().y, allocator);

    return v; // moved, not copied
}


void SaveObjectsToJson(std::vector<GameObject*> gameObjects, std::string filepath)
{
    rapidjson::Document doc;
    
    doc.SetObject();

    auto& allocator = doc.GetAllocator();

    rapidjson::Value objectArray(rapidjson::kArrayType);

    for (const auto& obj : gameObjects) {
        objectArray.PushBack(
            SerializeGameObject(obj, allocator),
            allocator
        );
    }

    doc.AddMember("gameObjects", objectArray, allocator);

    std::ofstream ofs(filepath);
    if (!ofs.is_open()) {
        std::cerr << "Error: cannot open output file";
        return;
    }

    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetIndent(' ', 2);
    doc.Accept(writer);
}
