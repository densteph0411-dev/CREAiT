//--------------------------------------------------------
// $Rev: 498 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-12-14 21:56:42 -0700 (Sun, 14 Dec 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/fileOperations/projectfileOperations.cpp $ URL Location
// Copyright 2023-2025
//--------------------------------------------------------

#include "generalXMLOperations.h"
#include "projectFileOperations.h"
#include <filesystem>
#include <iostream>
#include <string>

// Add namespace for Stream
using namespace std::filesystem;

namespace fs = std::filesystem;

bool fileOperations::parseProjectFile::load()
{

    // Define the return variable
    bool returnVal = false;

    // Create the new general XML file object
    fileOperations::generalXMLOperations generalXMLOperations;

    // Check if a project.xml file has been created
    if(!std::filesystem::exists(this->programDirectory + "/project.xml")){
        std::cerr << "parseProjectFile::load-Error: project not created at the location "
                  << this->programDirectory << std::endl;
        return false;
    }

    // Get all of the .xml project files in the directory
    const fs::path dirPath = this->programDirectory.c_str();
    std::vector<std::string> xmls = generalXMLOperations.getXmlFiles(dirPath);

    // Define the xml document object
    pugi::xml_document doc;

    // If the activeProjectName is empty, get the currently selected one, otherwise
    // set it to the value of ActiveProjectName
    if(activeProjectName == ""){
        activeProjectName = this->getActiveProjectName();
    }
    else
    {
        this->setActiveProject(activeProjectName,this->programDirectory + "/credentials.xml");
    }

    // Loop through each of the files
    for (const auto& file : xmls) {
        // std::cout << "Found XML file: " << file << std::endl;

        // Try loading the doc
        if (!doc.load_file(file.c_str()))
        {
            std::cerr << "Error: Could not load project file " << file << std::endl;
            return false;
        }

        // select the <root node> node
        pugi::xml_node projectsNode = doc.first_child();
        std::string context = projectsNode.name();
        if (!projectsNode)
        {
            std::cerr << "Error: 'projects' element not found in XML." << std::endl;
            return false;
        }

        // Iterate through the projects to create the project list
        // If the context is project, the project file is being read. No reason to parse for every file
        if(context == "project"){
            for (pugi::xml_node node = projectsNode.child("projectInfo"); node; node = node.next_sibling("projectInfo"))
            {
                // Add the project name to the list
                projectList.push_back(std::string(node.attribute("name").value()));
            }
        }

        // Get the active project from the credentials file
        pugi::xml_node activeProjectInfoNode = parseProjectFile::getProjectInfoNode(doc,this->activeProjectName);

        // Load fields into fieldInformationDictionary
        pugi::xml_node fieldsNode = activeProjectInfoNode.child("fields");
        for (pugi::xml_node field = fieldsNode.first_child(); field; field = field.next_sibling())
        {

            // Successfull in finding data to load
            returnVal = true;

            fieldInformationDictionary.createField(
                field.name(), context, field.child_value(), "true", "");
        }
    }

    return returnVal;
}

bool fileOperations::parseProjectFile::addProjectToProjectFile(DataDictionary projectDictionary) {

    // Create the return value variable. It's set true on initialization and set false if not succesfull
    bool returnVal = false;
    bool fileNotUpdatedFlag = false;

    pugi::xml_document doc;

    // Iterate through each context (project file) in the dictionary and add the project to each one at a time
    std::vector<std::string> contexts = projectDictionary.getAllContexts();
    // Stores the temporary fileName
    std::string tempFileName;

    // Check that the context is available
    if(contexts.empty()){
        std::cerr << "parseProjectFile-addProjectToProjectFile: No contexts exist in the dictionary" << std::endl;
        return returnVal;
    }

    // Iterate through each of the contexts and add the project and field information
    for (const auto &context : contexts) {

        // Build file string
        tempFileName = this->programDirectory + "/" + context + ".xml";
        pugi::xml_parse_result result = doc.load_file(tempFileName.c_str());

        if(result){
            // Add the project to the project file

            // Find the root node <project>
            pugi::xml_node projectsNode = doc.child(context);
            if (!projectsNode) {
                // If the node named after context does not exist (which shouldn't happen if we created it), create it
                projectsNode = doc.append_child(context);
            }

            // Create the new <projectInfo> node
            pugi::xml_node projectInfoNode = projectsNode.append_child("projectInfo");

            // Get the project name from fieldInformationDictionary
            std::string projectName = projectDictionary.getFieldValue("projectName", "project");
            if (projectName.empty()) {
                return false; // Project name is required
            }

            // Set the attributes for projectInfo
            projectInfoNode.append_attribute("name") = projectName.c_str();

            // Only set the active attribute in the credentials file, this is specific to each user
            // and different users will be using different projects
            if(context == "credentials"){
                projectInfoNode.append_attribute("active") = "true";
            }

            // Create the <fields> node
            pugi::xml_node fieldsNode = projectInfoNode.append_child("fields");

            // Populate fields dynamically from fieldInformationDictionary
            std::vector<std::string> fieldKeys = projectDictionary.getAllKeysByContext(context);

            // Loop through the keys to create fields
            for (const auto &key : fieldKeys) {

                // First check if the keys are persistent. If not don't create them as they are not to
                // be stored
                if(projectDictionary.getFieldPersistence(key,context) == "true"){
                    std::string value = projectDictionary.getFieldValue(key, context);
                    fieldsNode.append_child(key.c_str()).text().set(value.c_str());
                }
            }
            // Save the updated XML back to the file
            doc.save_file(tempFileName.c_str());
        }else{

            std::cerr << "parseProjectFile-addProjectToProjectFile: Could not add project to project file: "
                         "" << tempFileName << std::endl;

            // Set the flag that a file was not updated
            fileNotUpdatedFlag = true;
        }
    }

    if(fileNotUpdatedFlag){
        returnVal = false;
    }
    else
    {
        returnVal = true;
    }

    return returnVal;
}


std::string fileOperations::parseProjectFile::getNextID(std::string recordTypeName) {

    // If it's a project, only one ID needed.
    if(recordTypeName == "project"){
        return "PROJ";
    }

    // Load the XML document

    // Set the temporary currentID variable
    int currentID = 0;
    std::string nextID = "";

    pugi::xml_document doc;

    // Load the project file name
    std::string projectFileName = this->programDirectory + "/recordType.xml";
    if (!doc.load_file(projectFileName.c_str())) {
        return ""; // Return "" if file cannot be loaded
    }

    // Get the project node for the active project
    pugi::xml_node recordTypesNode = parseProjectFile::getProjectInfoNode(doc,this->activeProjectName);

    //// Navigate to <recordTypes>
    //pugi::xml_node recordTypesNode = doc.child("recordType")
    //                                     .child("projectInfo")
    //                                     .child("fields");

    if (!recordTypesNode) {
        return ""; // Return empty if <recordTypes> node is not found
    }

    pugi::xml_node fieldsNode = recordTypesNode.child("fields");

    // Iterate over <fields> elements
    for (pugi::xml_node recordTypeNode : fieldsNode.children()) {
        std::string innerText = recordTypeNode.text().as_string();
        if (innerText == recordTypeName) {
            // Retrieve the currentID attribute
            pugi::xml_attribute currentIDAttr = recordTypeNode.attribute("currentID");
            if (!currentIDAttr) {

                // No currentID exists, create it and add it
                nextID = std::string(recordTypeNode.name()) + "_1";
                recordTypeNode.append_attribute("currentID") = "2";
            }
            else
            {

                // Convert currentID to an integer, increment it, and update the XML
                currentID = currentIDAttr.as_int();
                nextID = std::string(recordTypeNode.name()) + "_" + std::to_string(currentID); // Store current ID as return value

                // Increment and update the XML attribute
                currentIDAttr.set_value(currentID + 1);
            }

            // Save the updated XML file
            if (!doc.save_file(projectFileName.c_str())) {
                return ""; // If saving fails, return empty
            }

            return nextID; // Return the previous currentID before incrementing
        }
    }

    return ""; // Return "" if the record type is not found
}

void fileOperations::parseProjectFile::revertID(std::string recordTypeName) {
    // Load the XML document
    pugi::xml_document doc;

    std::string projectFileName = this->programDirectory + "/project.xml";
    pugi::xml_parse_result result = doc.load_file(projectFileName.c_str());

    if (!result) {
        std::cerr << "Error: Could not load project file." << std::endl;
        return;
    }

    // Navigate to <recordTypes>
    pugi::xml_node recordTypesNode = doc.child("projects")
                                         .child("projectInfo")
                                         .child("recordTypes");

    if (!recordTypesNode) {
        std::cerr << "Error: <recordTypes> node not found in project file." << std::endl;
        return;
    }

    // Iterate over <recordType> elements
    for (pugi::xml_node recordTypeNode : recordTypesNode.children("recordType")) {
        std::string innerText = recordTypeNode.text().as_string();
        if (innerText == recordTypeName) {
            // Retrieve the currentID attribute
            pugi::xml_attribute currentIDAttr = recordTypeNode.attribute("currentID");
            if (!currentIDAttr) {
                std::cerr << "Error: currentID attribute not found for record type: "
                          << recordTypeName << std::endl;
                return;
            }

            // Convert currentID to an integer, decrement it if it's greater than 1
            int currentID = currentIDAttr.as_int();
            if (currentID > 1) {
                currentIDAttr.set_value(currentID - 1);
            } else {
                std::cerr << "Warning: currentID is already at the minimum value (1), cannot decrement."
                          << std::endl;
                return;
            }

            // Save the updated XML file
            if (!doc.save_file(projectFileName.c_str())) {
                std::cerr << "Error: Failed to save updated project file." << std::endl;
                return;
            }

            std::cout << "Successfully decremented currentID for " << recordTypeName
                      << " to " << currentID - 1 << std::endl;
            return;
        }
    }

    std::cerr << "Error: Record type " << recordTypeName << " not found in project file." << std::endl;
}

bool fileOperations::parseProjectFile::setUpProjectDirAndFile(std::string projectDirectory,
                                                              std::vector<std::string> recordList,
                                                              std::vector<std::string> contexts) {

    this->projectDirectory = projectDirectory;

    // Create required directories or make sure it exists
    if(!std::filesystem::create_directories(programDirectory) and
        (!std::filesystem::exists(programDirectory))){

        std::cerr << "parseProjectFile-setUpProjectDirAndFile: "
                     "Could not create programDirectory. Abort project setup" << std::endl;
        return false;
    }

    // Create required directories
    if(!std::filesystem::create_directories(projectDirectory) and
        (!std::filesystem::exists(projectDirectory))){
        std::cerr << "parseProjectFile-setUpProjectDirAndFile: "
                     "Could not create projectDirectory. Abort project setup" << std::endl;
        return false;
    }

    // define the variable used for holding the record directory
    std::string recordDirectory;

    for (auto& record : recordList) {

        recordDirectory = projectDirectory + "/" + record;

        // Create record directory
        if(!std::filesystem::create_directories(recordDirectory) and
            (!std::filesystem::exists(recordDirectory))){
            std::cerr << "parseProjectFile-setUpProjectDirAndFile: "
                         "Could not create directory " + recordDirectory + ". Abort project setup" << std::endl;
            return false;
        }
    }

    // Final check that directory environment is ready
    for (auto& record : recordList) {

        recordDirectory = projectDirectory + "/" + record;

        // Create record directory
        if(!std::filesystem::exists(recordDirectory)){
            std::cerr << "parseProjectFile-setUpProjectDirAndFile: "
                         "Directory was not created: " + recordDirectory + ". Abort project setup" << std::endl;
            return false;
        }
    }

    // Create the general XML File operations object
    fileOperations::generalXMLOperations generalXMLOperations;

    std::string tempFilePath;
    // Set up the project files
    for (auto& value : contexts) {

        // Set up the file path
        tempFilePath = programDirectory + "/" + value + ".xml";

        // Test that the file exists
        if(!std::filesystem::exists(tempFilePath)){

            // Create the new file
            generalXMLOperations.createNewFile(tempFilePath,value);

            if(!std::filesystem::exists(tempFilePath)){
                std::cerr << "parseProjectFile-setUpProjectDirAndFile: Could not create file " +
                                 tempFilePath + ". Abort project setup" << std::endl;

                return false;
            }
        }
    }

    // Send back the check
    return true;
}

std::string fileOperations::parseProjectFile::getActiveProjectName()
{
    pugi::xml_document doc;

    std::string credentialsFileName = programDirectory + "/credentials.xml";

    if (!doc.load_file(credentialsFileName.c_str()))
    {
        std::cerr << "Error: Could not load credentials file " << credentialsFileName << std::endl;
        return "";
    }

    pugi::xml_node credentialsNode = doc.child("credentials");
    for (pugi::xml_node projectInfoNode = credentialsNode.child("projectInfo");
         projectInfoNode; projectInfoNode = projectInfoNode.next_sibling("projectInfo"))
    {
        if (std::string(projectInfoNode.attribute("active").value()) == "true")
        {
            return projectInfoNode.attribute("name").value();
        }
    }

    std::cerr << "Error: No active project found in credentials file." << std::endl;
    return "";
}

void fileOperations::parseProjectFile::setActiveProject(std::string activeProject, std::string credentialsFileName) {
    // Load the XML file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(credentialsFileName.c_str());

    if (!result) {
        std::cerr << "Error: Failed to load credentials file '" << credentialsFileName << "': "
                  << result.description() << std::endl;
        return;
    }

    // Get the root <credentials> node
    pugi::xml_node credentialsNode = doc.child("credentials");
    if (!credentialsNode) {
        // If root doesn't exist, create it
        credentialsNode = doc.append_child("credentials");
    }

    bool projectFound = false;

    // Deactivate all projects and look for the one to activate
    for (pugi::xml_node projectInfo : credentialsNode.children("projectInfo")) {
        std::string projectName = projectInfo.attribute("name").as_string();

        if (projectName == activeProject) {
            projectInfo.attribute("active").set_value("true");
            projectFound = true;
        } else {
            if (!projectInfo.attribute("active")) {
                projectInfo.append_attribute("active");
            }
            projectInfo.attribute("active").set_value("false");
        }
    }

    // If the project wasn't found, create it
    if (!projectFound) {
        std::cerr << "Info: Project '" << activeProject << "' not found. Creating new <projectInfo> node." << std::endl;

        pugi::xml_node newProjectNode = credentialsNode.append_child("projectInfo");
        newProjectNode.append_attribute("name") = activeProject.c_str();
        newProjectNode.append_attribute("active") = "true";

        // Optionally add empty <fields> node if desired:
        newProjectNode.append_child("fields");
    }

    // Save updated file
    if (!doc.save_file(credentialsFileName.c_str())) {
        std::cerr << "Error: Failed to save updated credentials file: " << credentialsFileName << std::endl;
    }
}

void fileOperations::parseProjectFile::updateProject(std::string projectName) {
    std::string projectFileName = programDirectory + "/project.xml";

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(projectFileName.c_str());
    if (!result) {
        std::cerr << "Error: Could not load project file '" << projectName << "': "
                  << result.description() << std::endl;
        return;
    }

    pugi::xml_node projectsNode = doc.child("project");
    if (!projectsNode) {
        std::cerr << "Error: root node not found in project file.\n";
        return;
    }

    bool updated = false;

    for (pugi::xml_node projectInfo : projectsNode.children("projectInfo")) {
        const char* name = projectInfo.attribute("name").value();
        if (projectName == name) {
            pugi::xml_node fieldsNode = projectInfo.child("fields");
            if (!fieldsNode) {
                std::cerr << "Warning: <fields> not found in project " << projectName << "\n";
                continue;
            }

            // Example updates — you can add/remove as needed:
            fieldsNode.child("type").text() = fieldInformationDictionary.getFieldValue("type", "project");
            fieldsNode.child("IDPrefix").text() = fieldInformationDictionary.getFieldValue("IDPrefix", "project");
            fieldsNode.child("credentialsFileName").text() = fieldInformationDictionary.getFieldValue("fileName", "credentials");
            fieldsNode.child("description").text() = fieldInformationDictionary.getFieldValue("description", "project");

            updated = true;
            break; // stop after updating the matched project
        }
    }

    if (updated) {
        if (!doc.save_file(projectFileName.c_str(), "  ")) {
            std::cerr << "Error: Could not save updated project file.\n";
        } else {
            std::cout << "Project '" << projectName << "' updated successfully.\n";
        }
    } else {
        std::cerr << "Project '" << projectName << "' not found in XML.\n";
    }
}

void fileOperations::parseProjectFile::getProjectList(void) {
    projectList.clear(); // Clear any existing entries

    pugi::xml_document doc;

    std::string tempFileName = programDirectory + "/project.xml";
    pugi::xml_parse_result result = doc.load_file(tempFileName.c_str());

    if (!result) {
        std::cerr << "Error: Could not load project file '" << tempFileName << "': "
                  << result.description() << std::endl;
        return;
    }

    pugi::xml_node projectsNode = doc.child("project");
    if (!projectsNode) {
        std::cerr << "Error: root node not found in project file.\n";
        return;
    }

    for (pugi::xml_node projectInfo : projectsNode.children("projectInfo")) {
        const char* name = projectInfo.attribute("name").value();
        if (name && *name) {
            projectList.push_back(std::string(name));
        }
    }
}

bool fileOperations::parseProjectFile::createProjectFile(std::string fileName) {

    // Create the generalXML Operation object to create the base file
    fileOperations::generalXMLOperations generalXMLOperations;

    return generalXMLOperations.createNewFile(fileName,"project");
}

pugi::xml_node fileOperations::parseProjectFile::getProjectInfoNode(pugi::xml_document& doc, const std::string& projectName) {

    std::string xpath = "//projectInfo[@name='" + projectName + "']";
    return doc.select_node(xpath.c_str()).node();
}


std::vector<std::string> fileOperations::parseProjectFile::getRecordList() {
    std::vector<std::string> recordList;

    if(programDirectory.empty() or activeProjectName.empty()){
        return recordList;
    }

    std::string projectDirectory = this->programDirectory + "/" + this->activeProjectName + "/record";
    std::filesystem::path directoryPath(projectDirectory);

    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
        return recordList; // Return empty if path doesn't exist or isn't a directory
    }

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::filesystem::path filePath = entry.path();

            // Check for .xml extension (case-sensitive)
            if (filePath.extension() == ".xml") {
                recordList.push_back(filePath.stem().string()); // stem removes extension
            }
        }
    }

    return recordList;
}

std::vector<std::string> fileOperations::parseProjectFile::getAutomationsList() {
    std::vector<std::string> recordList;

    if(programDirectory.empty() or activeProjectName.empty()){
        return recordList;
    }

    std::string projectDirectory = this->programDirectory + "/" + this->activeProjectName + "/automation";
    std::filesystem::path directoryPath(projectDirectory);

    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
        return recordList; // Return empty if path doesn't exist or isn't a directory
    }

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::filesystem::path filePath = entry.path();

            // Check for .xml extension (case-sensitive)
            if (filePath.extension() == ".xml") {
                recordList.push_back(filePath.stem().string()); // stem removes extension
            }
        }
    }

    return recordList;
}
