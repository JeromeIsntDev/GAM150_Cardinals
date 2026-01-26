#include <document.h>
#include <istreamwrapper.h>
#include <ostreamwrapper.h>
#include <writer.h>
#include <prettywriter.h>
#include <vector>

#include "../header/GameSaver.hpp"
#include "../header/GameObject.hpp"


void JsonRead(const char* filePath)
{
	std::ifstream inputFileStream{ "../../Saves/test.json" };
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

    if (doc.HasMember("name") && doc["name"].IsString()) {
        std::string name = doc["name"].GetString();
        std::cout << "Name: " << name << std::endl;
    }

    // Get the "age" member
    if (doc.HasMember("age") && doc["age"].IsInt()) {
        int age = doc["age"].GetInt();
        std::cout << "Age: " << age << std::endl;
    }

    // Get the "city" member
    if (doc.HasMember("city") && doc["city"].IsString()) {
        std::string city = doc["city"].GetString();
        std::cout << "City: " << city << std::endl;
    }

    // Get the "skills" array
    if (doc.HasMember("skills")
        && doc["skills"].IsArray()) {
        const rapidjson::Value& skills = doc["skills"];
        std::cout << "Skills: ";
        for (rapidjson::SizeType i = 0; i < skills.Size();
            i++) {
            if (skills[i].IsString()) {
                std::string skill = skills[i].GetString();
                std::cout << skill << " ";
            }
        }
        std::cout << std::endl;
    }

    doc.AddMember("job", "Exorcist", doc.GetAllocator());
    doc.AddMember("gender", "Male", doc.GetAllocator());

    std::ofstream ofs("../../Saves/output.json");

    if (!ofs.is_open()) {
        std::cerr << "Error: cannot open output file";
        return;
    }

    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetIndent(' ', 2);
    doc.Accept(writer);
}

rapidjson::Value SerializeGameObject(
    const GameObject& go,
    rapidjson::Document::AllocatorType& allocator
) {
    rapidjson::Value v(rapidjson::kObjectType);

    v.AddMember("x", go.GetPosition().x, allocator);
    v.AddMember("y", go.GetPosition().y, allocator);

    return v; // moved, not copied
}


void JsonPrint(std::vector<GameObject> gameObjects)
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
