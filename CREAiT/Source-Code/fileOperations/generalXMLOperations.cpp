//--------------------------------------------------------
// $Rev: 498 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-12-14 21:56:42 -0700 (Sun, 14 Dec 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/fileOperations/generalXMLOperations.cpp $ URL Location
// Copyright @2024-2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------
#include "generalXMLOperations.h"
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

namespace fileOperations
{

generalXMLOperations::generalXMLOperations() {
    // Constructor logic (can be empty)
}

generalXMLOperations::~generalXMLOperations() {
    // Destructor logic (can be empty)
}

void generalXMLOperations::setFieldValue(std::string elementName,
                                         std::string textValue,
                                         std::string description,
                                         std::string fileName,
                                         std::string projectName,
                                         std::string context)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    if (!result)
    {
        std::cerr << "setElementTextInXMLFile-Error: Could not load XML file " << fileName << std::endl;
        return;
    }

    // XPath for project file
    std::string xPath = "";
    if((context == "project") ||
        (context == "credentials") ||
        (context == "recordType") ||
        (context == "linkType")){

        xPath = "//projectInfo[@name='" + projectName + "']/fields" + elementName;
    }
    else
    {

        xPath = "//fields" + elementName;
    }

    pugi::xpath_node fieldNode = doc.select_node(xPath.c_str());

    if (fieldNode)
    {
        // Set the field inner text to textValue
        fieldNode.node().text().set(textValue.c_str());
        // Set the child description text to description
        fieldNode.node().child("description").text().set(description.c_str());
    }
    else
    {
        std::cerr << "setElementTextInXMLFile-Error: Element " << elementName << " not found in XML." << std::endl;
    }

    // Release the document
    doc.save_file(fileName.c_str());
}

void generalXMLOperations::setElementTextInXMLFile(std::string elementName, std::string textValue, std::string fileName)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    if (!result)
    {
        std::cerr << "setElementTextInXMLFile-Error: Could not load XML file " << fileName << std::endl;
        return;
    }

    pugi::xml_node node = doc.child(elementName.c_str());
    if (node)
    {
        node.text().set(textValue.c_str());
        doc.save_file(fileName.c_str());
    }
    else
    {
        std::cerr << "setElementTextInXMLFile-Error: Element " << elementName << " not found in XML." << std::endl;
    }
}

std::string generalXMLOperations::getNodeAttribute(pugi::xml_node &node, std::string attributeName)
{
    pugi::xml_attribute attr = node.attribute(attributeName.c_str());
    return attr ? attr.value() : "";
}

void generalXMLOperations::setNodeAttribute(pugi::xml_node &node, std::string attributeName, std::string attributeValue)
{
    pugi::xml_attribute attr = node.attribute(attributeName.c_str());
    if (attr)
    {
        attr.set_value(attributeValue.c_str());
    }
    else
    {
        node.append_attribute(attributeName.c_str()) = attributeValue.c_str();
    }
}

std::vector<std::string> generalXMLOperations::parseFieldsInXMLFile(std::string fileName)
{
    std::vector<std::string> fieldNames;
    pugi::xml_document doc;
    if (!doc.load_file(fileName.c_str()))
    {
        std::cerr << "parseFieldsInXMLFile-Error: Could not load XML file " << fileName << std::endl;
        return fieldNames;
    }

    pugi::xml_node fieldsNode = doc.child("fields");
    if (!fieldsNode)
    {
        std::cerr << "parseFieldsInXMLFile-Error: 'fields' element not found in XML." << std::endl;
        return fieldNames;
    }

    for (pugi::xml_node field = fieldsNode.first_child(); field; field = field.next_sibling())
    {
        fieldNames.push_back(field.name());
    }
    return fieldNames;
}

bool generalXMLOperations::createFieldInXMLFile(std::string fieldName,
                                                std::string fieldValue,
                                                std::string fileName)
{
    pugi::xml_document doc;
    pugi::xml_parse_result ok = doc.load_file(fileName.c_str());
    if (!ok)
    {
        std::cerr << "createFieldInXMLFile-Error: Could not load XML file "
                  << fileName << " (" << ok.description() << ")\n";
        return false;
    }

    pugi::xml_node fieldsNode = doc.child("fields");
    if (!fieldsNode)
        fieldsNode = doc.append_child("fields");

    // Find existing node by name; if not found, create it
    pugi::xml_node fieldNode = fieldsNode.child(fieldName.c_str());
    if (!fieldNode)
        fieldNode = fieldsNode.append_child(fieldName.c_str());

    // Set/replace the text content
    fieldNode.text().set(fieldValue.c_str());

    if (!doc.save_file(fileName.c_str()))
    {
        std::cerr << "createFieldInXMLFile-Error: Could not save XML file "
                  << fileName << std::endl;
        return false;
    }
    return true;
}


void generalXMLOperations::removeFieldInXMLFile(std::string fieldName, std::string fileName, std::string context, std::string projectName)
{
    pugi::xml_document doc;
    if (!doc.load_file(fileName.c_str()))
    {
        std::cerr << "removeFieldInXMLFile-Error: Could not load XML file " << fileName << std::endl;
        return;
    }

    // XPath for project file
    std::string xPath = "";
    if((context == "project") ||
        (context == "credentials") ||
        (context == "recordType") ||
        (context == "linkType")){

        xPath = "//projectInfo[@name='" + projectName + "']/fields";
    }
    else
    {

        xPath = "//fields";
    }

    try
    {
        pugi::xpath_node fieldsNode = doc.select_node(xPath.c_str());

        // Find the path
        if (fieldsNode)
        {
            pugi::xml_node fields = fieldsNode.node();
            pugi::xml_node child = fields.child(fieldName.c_str());

            if (child)
            {
                fields.remove_child(child);
            }
            else
            {
                std::cerr << "removeFieldInXMLFile-Error: Field element " << fieldName
                          << " not found under " << xPath << std::endl;
            }
        }
        else
        {
            std::cerr << "removeFieldInXMLFile-Error: XPath " << xPath
                      << " did not match any <fields> node." << std::endl;
        }
    }
    catch (const pugi::xpath_exception& e)
    {
        std::cerr << "XPath error: " << e.what()
        << " in expression: " << xPath << std::endl;
        return;
    }

    // Release the document
    doc.save_file(fileName.c_str());
}

bool generalXMLOperations::createNewFile(std::string fileName, std::string context)
{
    std::ofstream file(fileName);
    if (file.is_open())
    {
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<" << context << ">\n";
        file << "</" << context << ">\n";
        file.close();
    }
    else
    {
        std::cerr << "createNewFile-Error: Could not create file " << fileName << std::endl;
        return false;
    }

    // Add the fields node
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    if (!result)
    {
        std::cerr << "createNewFile-Error: Could not load XML file " << fileName << std::endl;
        return false;
    }

    // Protect against invalid path exception.
    pugi::xml_node node = doc.root().first_child();
    if (node) {
        node.append_child("fields");
    } else {
        std::cerr << "createNewFile - parent node didn't match any node.\n";
        return false;
    }

    // Release the document
    doc.save_file(fileName.c_str());

    return true;
}

// Implemented because it was declared in the header.
std::vector<std::string> generalXMLOperations::getXMLFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> xmlFiles;
    fs::path directory(directoryPath);

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Invalid directory: " << directory << std::endl;
        return xmlFiles;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            xmlFiles.push_back(entry.path().string());
        }
    }

    return xmlFiles;
}

std::vector<std::string> generalXMLOperations::getXmlFiles(const fs::path& directory) {
    std::vector<std::string> xmlFiles;

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Invalid directory: " << directory << '\n';
        return xmlFiles;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            xmlFiles.push_back(entry.path().string());   // or .u8string() if you prefer UTF-8 on Windows
        }
    }

    return xmlFiles;
}

void generalXMLOperations::createField(std::string elementName,
                                       std::string textValue,
                                       std::string fileName,
                                       std::string projectName)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    if (!result)
    {
        std::cerr << "setElementTextInXMLFile-Error: Could not load XML file " << fileName << std::endl;
        return;
    }

    // XPath for project file
    std::string xPath = "";
    if(projectName == ""){

        xPath = "//fields";
    }
    else
    {
        xPath = "/projectInfo[@name='" + projectName + "']//fields";
    }

    // Protect against invalid path exception.
    try {
        pugi::xpath_node xpathResult = doc.select_node(xPath.c_str());
        if (xpathResult) {
            pugi::xml_node node = xpathResult.node();
            node.append_child(elementName.c_str()).text().set(textValue.c_str());
        } else {
            std::cerr << "XPath didn't match any node.\n";
        }

        // Release the document
        doc.save_file(fileName.c_str());

    } catch (const pugi::xpath_exception& e) {
        std::cerr << "XPath error: " << e.what() << "\n";
        return;
    }
}
}
