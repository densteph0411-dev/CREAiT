//--------------------------------------------------------
// $Rev: 429 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-11-07 12:53:44 -0700 (Fri, 07 Nov 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/fileOperations/recordFileOperations.cpp $ URL Location
// Copyright @2024-2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------

#include <filesystem>
#include <iostream>
#include "recordFileOperations.h"
#include "projectFileOperations.h"
#include "../3rdPartyLibraries/pugixml/src/pugixml.hpp"

using namespace pugi;
using namespace std;
using namespace fileOperations;

namespace fileOperations
{

    std::vector<std::string> parseRecordFile::linkName;
    std::vector<std::string> parseRecordFile::linkDirection;
    std::vector<std::string> parseRecordFile::linkType;
    std::vector<std::string> parseRecordFile::linkTitle;

    std::vector<std::string> parseRecordFile::versionNumber;
    std::vector<std::string> parseRecordFile::versionDescription;
    std::vector<std::string> parseRecordFile::versionDate;

    parseRecordFile::~parseRecordFile() {
        // Destructor logic (can be empty)
    }

    void parseRecordFile::readRecordFile(std::string recordID)
    {
        std::string fileName = recordDirectory + "/" + recordID + ".xml";

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fileName.c_str());

        if (!result) {
            std::cerr << "readRecordFile - Error reading file: " << fileName
                      << "\nReason: " << result.description() << std::endl;
            return;
        }

        // Clear previous link arrays before loading new data
        linkName.clear();
        linkDirection.clear();
        linkType.clear();
        linkTitle.clear();
        // Clear previous versions before loading t
        versionNumber.clear();
        versionDescription.clear();
        versionDate.clear();

        // Parse fields
        pugi::xml_node fieldsNode = doc.child("record").child("fields");
        for (pugi::xml_node field = fieldsNode.first_child(); field; field = field.next_sibling()) {
            std::string key = field.name();
            std::string value = field.text().as_string();

            if (fieldInformationDictionary.keyExists(key, "record")) {
                fieldInformationDictionary.setFieldValue(key, "record", value);
            } else {
                fieldInformationDictionary.createField(key, "record", value, "true", "");
            }
        }

        // Parse links
        pugi::xml_node linksNode = doc.child("record").child("links");
        for (pugi::xml_node link = linksNode.first_child(); link; link = link.next_sibling()) {
            std::string name = link.text().as_string();
            std::string direction = link.attribute("direction").as_string();
            std::string type = link.attribute("type").as_string();

            linkName.push_back(name);
            linkDirection.push_back(direction);
            linkType.push_back(type);

            // Get the title information
            linkTitle.push_back(parseRecordFile::getRecordInformation(name,"title"));
        }

        // Parse versions
        pugi::xml_node versionsNode = doc.child("record").child("versions");
        for (pugi::xml_node version = versionsNode.first_child(); version; version = version.next_sibling()) {
            std::string verNumber = version.attribute("versionNumber").as_string();
            std::string verDescription = version.text().as_string();
            std::string verDate = version.attribute("versionDate").as_string();

            versionNumber.push_back(verNumber);
            versionDescription.push_back(verDescription);
            versionDate.push_back(verDate);
        }
    }


    std::string parseRecordFile::createRecordFile(const std::string recordDirectory,
                                                  const std::string recordType,
                                                  const std::string newID)
    {

        std::string newFileName = recordDirectory + "/" + newID + ".xml";

        xml_document doc;
        xml_node recordNode = doc.append_child("record");
        recordNode.append_child("fields");
        recordNode.append_child("links");
        recordNode.append_child("versions");

        // Set the ID in the local fieldInformationDictionary
        fieldInformationDictionary.setFieldValue("ID","record",newID);

        // Get the fields node
        xml_node fieldsNode = recordNode.child("fields");

        // Populate fields dynamically from fieldInformationDictionary
        std::vector<std::string> fieldKeys = fieldInformationDictionary.getAllKeysByContext("record");
        for (const auto &key : fieldKeys) {

            // First test if the data is persistent
            if(fieldInformationDictionary.getFieldPersistence(key,"record") == "true"){
                std::string value = fieldInformationDictionary.getFieldValue(key, "record");
                fieldsNode.append_child(key.c_str()).text().set(value.c_str());
            }
        }

        if (!doc.save_file(newFileName.c_str())) {
            cerr << "createXMLRecordFile-Error creating file: " << newFileName << endl;
            return "";
        }

        // Set the newly loaded ID and fileName
        fieldInformationDictionary.setFieldValue("ID", "record", newID);
        fieldInformationDictionary.setFieldValue("fileName", "record", newFileName);

        // Add the first version
        parseRecordFile::updateVersion("Initial Version",fieldInformationDictionary.getFieldValue("lastModifiedOn","record"));

        return newID;
    }

    void parseRecordFile::deleteRecordFile()
    {
        std::string fileName = fieldInformationDictionary.getFieldValue("fileName", "record");
        std::filesystem::remove(fileName);
    }

    std::string parseRecordFile::getRecordInformation(const std::string& ID, const std::string& returnItem)
    {
        std::string fileName = fieldInformationDictionary.getFieldValue("recordDirectory", "project") + "/" + ID + ".xml";
        xml_document doc;
        if (!doc.load_file(fileName.c_str())) return "";

        xml_node node = doc.child("record").child("fields").child(returnItem.c_str());
        return node ? node.text().as_string() : "";
    }

    void parseRecordFile::linkManipulate(std::string linkID, bool addLink, bool directionUp, std::string linkType)
    {
        std::string fileName = fieldInformationDictionary.getFieldValue("fileName", "record");
        xml_document doc;
        if (!doc.load_file(fileName.c_str())) return;

        xml_node linksNode = doc.child("record").child("links");
        if (!linksNode) return;

        if (addLink) {
            xml_node linkNode = linksNode.append_child("link");
            linkNode.append_attribute("direction") = directionUp ? "Up" : "Down";
            linkNode.append_attribute("type") = linkType;
            linkNode.text().set(linkID.c_str());
        } else {
            for (xml_node linkNode = linksNode.first_child(); linkNode; linkNode = linkNode.next_sibling()) {
                if (std::string(linkNode.text().as_string()) == linkID) {
                    linksNode.remove_child(linkNode);
                    break;
                }
            }
        }

        // Save the file
        doc.save_file(fileName.c_str());

        // Add the version information. Done after filesave as the updateVersion also saves the file.
        if(addLink){
            // Add the version record for newly added links
            parseRecordFile::updateVersion("Added Link to " + linkID,fieldInformationDictionary.getFieldValue("lastModifiedOn","record"));
        }
        else
        {
            // Add the version record for newly added links
            parseRecordFile::updateVersion("Removed Link to " + linkID,fieldInformationDictionary.getFieldValue("lastModifiedOn","record"));
        }

    }

    void parseRecordFile::updateVersion(std::string description, std::string dateTime)
    {
        std::string fileName = fieldInformationDictionary.getFieldValue("fileName", "record");
        xml_document doc;
        if (!doc.load_file(fileName.c_str())) return;

        xml_node versionNodes = doc.child("record").child("versions");
        if (!versionNodes) return;

        int newVersionID = 0;
        int currNewVersionID = 0;

        // Get the latest version number
        for (xml_node versionNode = versionNodes.first_child(); versionNode; versionNode = versionNode.next_sibling()) {

            // Set the version to the last read version
            currNewVersionID = std::stoi(versionNode.attribute("versionNumber").value());

            // if this version is greater than the last read version, record it
            if(currNewVersionID > newVersionID){
                newVersionID = currNewVersionID;
            }
        }

        // Test if a version was found. If not set it to 1
        if(newVersionID == 0){
            newVersionID = 1;
        }
        else
        {
            newVersionID++;
        }

        // Create the new item
        xml_node versionNode = versionNodes.append_child("version");
        versionNode.append_attribute("versionNumber") = newVersionID;
        versionNode.append_attribute("versionDate") = dateTime;
        versionNode.text().set(description.c_str());

        doc.save_file(fileName.c_str());
    }

    void parseRecordFile::editRecordFile()
    {
        std::string fileName = fieldInformationDictionary.getFieldValue("fileName", "record");
        xml_document doc;
        if (!doc.load_file(fileName.c_str())) return;

        xml_node fieldsNode = doc.child("record").child("fields");
        for (xml_node field = fieldsNode.first_child(); field; field = field.next_sibling()) {
            field.text().set(fieldInformationDictionary.getFieldValue(field.name(), "record").c_str());
        }

        doc.save_file(fileName.c_str());
    }
}
