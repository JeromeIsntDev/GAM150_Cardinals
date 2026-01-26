#include <document.h>
#include "../header/GameSaver.hpp"
#include "../header/GameObject.hpp"

GameObject DeserializeGameObject(const rapidjson::Value& v) {
    GameObject obj;

    if (v.HasMember("posx") && v.HasMember("posy")) {
        obj.GetPosition().x = v["posx"].GetFloat();
        obj.GetPosition().y = v["posy"].GetFloat();
    }

    return obj;
}

void ReadJsonToObjects(const char* filePath)
{
	std::ifstream inputFileStream{ filePath };
    if (!inputFileStream.is_open()) {
        std::cerr << "Error: file cannot open.";
        return;
    }

    rapidjson::IStreamWrapper isw(inputFileStream);
 	rapidjson::Document doc;
	doc.ParseStream(isw);
    if (doc.HasParseError()) {
        std::cerr << "Error: parse error.";
        return;
    }

	inputFileStream.close();
  
    if (doc.HasMember("gameObjects")
        && doc["gameObjects"].IsArray()) {
        const auto& array = doc["gameObjects"];
        std::cout << "GameObjects: \n";
        for (const auto& v : array.GetArray()) {
            GameObject obj = DeserializeGameObject(v);
            std::cout << obj << "\n";
        }
        std::cout << std::endl;
    }
}

rapidjson::Value SerializeGameObject(
    const GameObject& go,
    rapidjson::Document::AllocatorType& allocator
) {
    rapidjson::Value v(rapidjson::kObjectType);

    v.AddMember("posx", go.GetPosition().x, allocator);
    v.AddMember("posy", go.GetPosition().y, allocator);

    return v; // moved, not copied
}


void SaveObjectsToJson(std::vector<GameObject> gameObjects)
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

    std::ofstream ofs("../../Saves/GameObjects.json");
    if (!ofs.is_open()) {
        std::cerr << "Error: cannot open output file";
        return;
    }

    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetIndent(' ', 2);
    doc.Accept(writer);
}
