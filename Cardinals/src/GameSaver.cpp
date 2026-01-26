#include "../header/GameSaver.hpp"
#include "document.h"
#include "filereadstream.h"
#include "istreamwrapper.h"

void JsonRead(const char* filePath)
{
	std::ifstream inputFileStream{ filePath };
    rapidjson::IStreamWrapper isw(inputFileStream);

	rapidjson::Document doc;
	doc.ParseStream(isw);

	inputFileStream.close();

    //if (doc.HasMember("name") && doc["name"].IsString()) {
    //    std::string name = doc["name"].GetString();
    //    std::cout << "Name: " << name << std::endl;
    //}

    //// Get the "age" member
    //if (doc.HasMember("age") && doc["age"].IsInt()) {
    //    int age = doc["age"].GetInt();
    //    std::cout << "Age: " << age << std::endl;
    //}

    //// Get the "city" member
    //if (doc.HasMember("city") && doc["city"].IsString()) {
    //    std::string city = doc["city"].GetString();
    //    std::cout << "City: " << city << std::endl;
    //}

    //// Get the "skills" array
    //if (doc.HasMember("skills")
    //    && doc["skills"].IsArray()) {
    //    const rapidjson::Value& skills = doc["skills"];
    //    std::cout << "Skills: ";
    //    for (rapidjson::SizeType i = 0; i < skills.Size();
    //        i++) {
    //        if (skills[i].IsString()) {
    //            std::string skill = skills[i].GetString();
    //            std::cout << skill << " ";
    //        }
    //    }
    //    std::cout << std::endl;
    //}
}

//void JsonPrint()
//{
//
//}
