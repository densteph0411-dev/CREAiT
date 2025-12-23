//--------------------------------------------------------
// $Rev: 419 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-11-04 09:34:13 -0800 (Tue, 04 Nov 2025) $ Date of last commit
// $HeadURL: svn://ipc1/requirementTool/Software/CREAiT/CREAiT-Qt-Master/Source-Code/fileOperations/reviewfileoperations.cpp $ URL Location
// Copyright @2024-2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------

#include <filesystem>
#include <iostream>
#include <errno.h>
#include "reviewFileOperations.h"
#include "projectFileOperations.h"
#include "../3rdPartyLibraries/pugixml/src/pugixml.hpp"

using namespace pugi;
using namespace std;
using namespace fileOperations;

namespace fileOperations
{

reviewFileOperations::~reviewFileOperations() {
    // Destructor logic (can be empty)
}

void reviewFileOperations::readFile(std::string recordID)
{
    std::string fileName = reviewDirectory + "/" + recordID + ".xml";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());

    if (!result) {
        std::cerr << "readReviewFile - Error reading file: " << fileName
                  << "\nReason: " << result.description() << std::endl;
        return;
    }

    // Parse fields
    pugi::xml_node fieldsNode = doc.child("review").child("fields");
    for (pugi::xml_node field = fieldsNode.first_child(); field; field = field.next_sibling()) {
        std::string key = field.name();
        std::string value = field.text().as_string();

        if (fieldInformationDictionary.keyExists(key, "review")) {
            fieldInformationDictionary.setFieldValue(key, "review", value);
        } else {
            fieldInformationDictionary.createField(key, "review", value, "true", "");
        }
    }
}


std::string reviewFileOperations::createFile(const std::string reviewDirectory,
                                                           const std::string recordType,
                                                           const std::string newID)
{

    std::string newFileName = reviewDirectory + "/" + newID + ".xml";

    xml_document doc;
    xml_node recordNode = doc.append_child("review");
    recordNode.append_child("fields");

    // Set the ID in the local fieldInformationDictionary
    fieldInformationDictionary.setFieldValue("ID","review",newID);

    // Get the fields node
    xml_node fieldsNode = recordNode.child("fields");

    // Populate fields dynamically from fieldInformationDictionary
    std::vector<std::string> fieldKeys = fieldInformationDictionary.getAllKeysByContext("review");
    for (const auto &key : fieldKeys) {

        // First test if the data is persistent
        if(fieldInformationDictionary.getFieldPersistence(key,"review") == "true"){
            std::string value = fieldInformationDictionary.getFieldValue(key, "review");
            fieldsNode.append_child(key.c_str()).text().set(value.c_str());
        }
    }

    if (!doc.save_file(newFileName.c_str())) {
        std::cerr << "readReviewFile - Error saving file: " << newFileName
                  << "\nReason: " << std::endl;
        return "";
    }

    fieldInformationDictionary.setFieldValue("ID", "review", newID);
    fieldInformationDictionary.setFieldValue("fileName", "review", newFileName);

    return newID;
}

void reviewFileOperations::deleteFile()
{
    std::string fileName = fieldInformationDictionary.getFieldValue("fileName", "review");
    std::filesystem::remove(fileName);
}

std::string reviewFileOperations::getReviewInformation(const std::string& ID, const std::string& returnItem)
{
    std::string fileName = fieldInformationDictionary.getFieldValue("reviewDirectory", "project") + "/" + ID + ".xml";
    xml_document doc;
    if (!doc.load_file(fileName.c_str())) return "";

    xml_node node = doc.child("review").child("fields").child(returnItem.c_str());
    return node ? node.text().as_string() : "";
}

void reviewFileOperations::editReviewFile()
{
    std::string fileName = fieldInformationDictionary.getFieldValue("fileName", "review");
    xml_document doc;
    if (!doc.load_file(fileName.c_str())) return;

    xml_node fieldsNode = doc.child("review").child("fields");
    for (xml_node field = fieldsNode.first_child(); field; field = field.next_sibling()) {
        field.text().set(fieldInformationDictionary.getFieldValue(field.name(), "review").c_str());
    }

    doc.save_file(fileName.c_str());
}
}
