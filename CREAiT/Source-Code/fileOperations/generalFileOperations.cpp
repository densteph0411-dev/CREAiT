//--------------------------------------------------------
// $Rev: 397 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-10-22 17:43:16 -0700 (Wed, 22 Oct 2025) $ Date of last commit
// $HeadURL: svn://ipc1/requirementTool/Software/CREAiT/CREAiT-Qt-Master/Source-Code/fileOperations/generalFileOperations.cpp $ URL Location
// Copyright @2024-2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------

#include <filesystem>
#include <iostream>
#include <cerrno>

#include "generalFileOperations.h"
#include "generalXMLOperations.h"
#include <pugixml.hpp>

using namespace pugi;
using namespace std;
using namespace fileOperations;

namespace fileOperations
{

GeneralFileOperations::GeneralFileOperations(const std::string& filePath)
    : filePath_(filePath)
{
    // keep public alias in sync for any legacy call sites
    this->filePath = filePath;
}

GeneralFileOperations::~GeneralFileOperations() {
    // Destructor logic
}

void GeneralFileOperations::readFile(void)
{
    // Create the file objects
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(this->filePath_.c_str());

    if (!result) {
        std::cerr << "GeneralFileOperations::readFile - Error reading file: " << this->filePath_
                  << "\nReason: " << result.description() << std::endl;
        return;
    }

    std::string testVal = "";

    // Parse fields
    pugi::xml_node fieldsNode = doc.child("fields");
    for (pugi::xml_node field = fieldsNode.first_child(); field; field = field.next_sibling()) {
        std::string key = field.name();
        std::string value = field.text().as_string();

        fieldInformationDictionary.createField(key, "basic", value, "true", "");
        testVal = fieldInformationDictionary.getFieldValue(key,"basic");
    }
}

bool GeneralFileOperations::createFile() {
    // fallback: empty skeleton
    DataDictionary empty;
    return createFile(empty, /*persistOnly=*/true);
}

static pugi::xml_node ensure_child(pugi::xml_node parent, const char* name) {
    if (auto n = parent.child(name)) return n;
    return parent.append_child(name);
}

bool GeneralFileOperations::createFile(const DataDictionary& data, bool persistOnly) {
    pugi::xml_document doc;

    // Root: <fields>
    pugi::xml_node fields = ensure_child(doc, "fields");

    // Upsert all keys from all contexts directly under <fields>
    std::vector<std::string> contexts = data.getAllContexts();
    for (const auto& ctx : contexts) {
        for (const auto& key : data.getAllKeysByContext(ctx)) {
            if (persistOnly && data.getFieldPersistence(key, ctx) != "true") {
                continue;
            }

            // Find or create the element directly under <fields>
            pugi::xml_node n = fields.child(key.c_str());
            if (!n) n = fields.append_child(key.c_str());

            // Upsert text
            const std::string value = data.getFieldValue(key, ctx);
            if (!value.empty()) {
                n.text().set(value.c_str());
            } else {
                // leave empty tag if no value provided (e.g., <title/>)
                // except for "deleted" which we default to "false" below
            }

            // Optional metadata as attributes (keep if you still want them)
            const std::string p = data.getFieldPersistence(key, ctx);
            const std::string d = data.getFieldDescription(key, ctx);
            if (!p.empty()) n.attribute("persistence").set_value(p.c_str());
            if (!d.empty()) n.attribute("description").set_value(d.c_str());
        }
    }

    // Ensure a default <deleted>false</deleted> if user/data didn't provide it
    pugi::xml_node deletedNode = fields.child("deleted");
    if (!deletedNode) {
        deletedNode = fields.append_child("deleted");
    }
    if (!deletedNode.text()) {
        deletedNode.text().set("false");
    } else if (std::string(deletedNode.text().as_string()).empty()) {
        deletedNode.text().set("false");
    }

    if (!doc.save_file(filePath_.c_str())) {
        std::cerr << "createFile-Error: Could not save " << filePath_ << "\n";
        return false;
    }
    return true;
}

void GeneralFileOperations::deleteFile(void)
{
    std::filesystem::remove(filePath_);
}

std::string GeneralFileOperations::GetFieldValue(const std::string returnItem)
{
    xml_document doc;
    if (!doc.load_file(filePath_.c_str())) return "";

    xml_node node = doc.child("fields").child(returnItem.c_str());
    return node ? node.text().as_string() : "";
}

// Gets automation record information
std::string GeneralFileOperations::GetAttributeValue(const std::string FieldName,
                                                     const std::string AttributeName){

    // Define return value
    std::string returnVal;

    // Open the document and test it was opened.
    xml_document doc;
    if (!doc.load_file(filePath_.c_str())) return "";

    // Test if the field is not present. If it does log an error and return
    xml_node fieldsNode = doc.child("fields").child(FieldName.c_str());
    if(fieldsNode.empty()){
        std::cerr << "GeneralFileOperations::GetAttributeValue - field " + FieldName + " does not exist.";
        return "";
    }

    // Fetch the attribute
    pugi::xml_attribute attribute = fieldsNode.attribute(AttributeName.c_str());
    if(attribute.empty()){
        std::cerr << "GeneralFileOperations::GetAttributeValue - attribute " + AttributeName + " does not exist.";
        return "";
    }

    return attribute.value();
}

void GeneralFileOperations::editFile()
{
    xml_document doc;
    if (!doc.load_file(this->filePath_.c_str())) return;

    xml_node fieldsNode = doc.child("fields");
    for (xml_node field = fieldsNode.first_child(); field; field = field.next_sibling()) {
        field.text().set(fieldInformationDictionary.getFieldValue(field.name(), "basic").c_str());
    }

    doc.save_file(this->filePath_.c_str());
}


// Set or Add an attribute if it doesn't exist
void GeneralFileOperations::setAttribute(std::string /*ID*/,
                                         std::string FieldName,
                                         std::string attributeName,
                                         std::string value){

    xml_document doc;
    if (!doc.load_file(this->filePath_.c_str())) return;

    xml_node fieldsNode = doc.child("fields").child(FieldName.c_str());
    if(fieldsNode.empty()){
        std::cerr << "GeneralFileOperations::setAttribute - field " + FieldName + " does not exist.";
        return;
    }

    // Upsert attribute
    pugi::xml_attribute attribute = fieldsNode.attribute(attributeName.c_str());
    if(attribute.empty()){
        attribute = fieldsNode.append_attribute(attributeName.c_str());
        if(attribute.empty()){
            std::cerr << "GeneralFileOperations::setAttribute - attribute " + attributeName + " could not be created.";
            return;
        }
    }
    attribute.set_value(value.c_str());

    doc.save_file(this->filePath_.c_str());
}

// Remove an attribute if it exists
void GeneralFileOperations::removeAttribute(std::string /*ID*/,
                                            std::string FieldName,
                                            std::string attributeName)
{
    xml_document doc;
    if (!doc.load_file(this->filePath_.c_str())) return;

    xml_node fieldsNode = doc.child("fields").child(FieldName.c_str());
    if(fieldsNode.empty()){
        std::cerr << "GeneralFileOperations::removeAttribute - field " + FieldName + " does not exist.";
        return;
    }

    pugi::xml_attribute attribute = fieldsNode.attribute(attributeName.c_str());
    if(attribute) {
        fieldsNode.remove_attribute(attribute);
        doc.save_file(this->filePath_.c_str());
    } else {
        std::cerr << "GeneralFileOperations::removeAttribute - attribute " + attributeName + " does not exist.";
    }
}

} // namespace fileOperations
