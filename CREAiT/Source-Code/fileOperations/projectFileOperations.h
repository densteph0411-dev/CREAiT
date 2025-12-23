//--------------------------------------------------------
// $Rev: 429 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-11-07 12:53:44 -0700 (Fri, 07 Nov 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/fileOperations/projectFileOperations.h $ URL Location
// Copyright Copyright @2023, 2024 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------
#pragma once

#include "../3rdPartyLibraries/pugixml/src/pugixml.hpp"
#include "../dataObjects/dataDictionary.h"
#include <string>
#include <vector>

// Namespace for all file operations
namespace fileOperations
{

    // defines fileparsing operation
    class parseProjectFile
    {

    public:

        std::string programDirectory;

        // Define constructor by passing in the projectFileLocation
        parseProjectFile(std::string programDirectory) : programDirectory(programDirectory){

            // Load the project
            this->load();
        }

        // define data and arrays for the project information
        DataDictionary fieldInformationDictionary = *new DataDictionary();
        std::vector<std::string> projectList;

        // Store the project Name
        std::string activeProjectName;
        std::string projectDirectory;

        // loads the information from the project file as provided in projectFileName for each file located withing the
        // project directory. It loads the file with the file name = context + ".xml" and loads it into the
        // fieldInformationDictionary
        //
        // The file has the following structure:
        //  <project>
        //      <projectInfo name="Project Name">
        //          <fields></fields>
        //      </projectInfo>
        //  </project>
        //
        // Returns true of successfull in loading projects. Otherwise it returns false.
        bool load(void);

        // Add each of the record types from the project file
        // under <recordTypes> to the recordTypesDictionary
        void addTypeToTreeViewArray(std::string recordType);

        // Adds a project to the project file using the information passed into the dictionary with the context "project"
        // It creates a brand new project structure as defined below as a child of the <projects> node
        // <projectInfo name="">
        //     <fields></fields>
        // </projectInfo>
        //
        // The newly created project is set to active by creating and setting the <projectInfo> attribute active="true"
        // The recordTypes, and linkTypes are populated from the recordTypesDictionary and linkTypesDictionary dictionary
        bool addProjectToProjectFile(DataDictionary projectDictionary);

        // This function opens the project file, finds the provided recordType, and returns the next available ID of the provided record type from the project file.
        // It does this by reading projectFileName, and returning the currentID attribute of the element <recordType>
        // with inner text=recordTypeName. If it does not find the value it returns NULL.
        std::string getNextID(std::string recordTypeName);

        // This function reverts the ID of the current record type in the project file.
        // It does this by reading projectFileName, and subtracting the currentID attribute of the element <recordType>
        // by one and setting it to the new value.
        void revertID(std::string recordTypeName);

        // This function checks if the project directory and files exist at the specified location projectDirectoryLocation.
        // If not it creates them. The directory structure is the following:
        // programName (Directory)
        //       contextFiles (XML File)
        //     +-projectName (Directory)
        //         +-records (Directory)
        //         +-images (Directory)
        // It returns true if successfull in creating the project directories and files, and false if not.
        bool setUpProjectDirAndFile(
            std::string projectDirectory,
            std::vector<std::string> contextFiles,
            std::vector<std::string> projectDirectories);

        // This function parses the credentials file and iterates over all <projectInfo> children of element
        // <credentials> and returns the name attribute of element <projectInfo> that has attribute active="true"
        std::string getActiveProjectName(void);

        // Tests of the active projectInfo structure has been reached
        bool readToActiveProject(pugi::xml_document currentReader);

        // Sets the project element attribute active=true with attribute name=activeProject
        // and sets all other project element attribute active=false for all other project
        // structure present in the file
        void setActiveProject(std::string activeProject, std::string credentialsFileName);

        // Update the project with the provided project name
        void updateProject(std::string projectName);

        // populates the project list in the projectInfo structure
        void getProjectList(void);

        // Create a new project file
        bool createProjectFile(std::string fileName);

        // Returns the project node for the provided doc and project name
        pugi::xml_node getProjectInfoNode(pugi::xml_document& doc, const std::string& projectName);

        // Returns a list of records
        std::vector<std::string> getRecordList();

        // Returns a list of automations
        std::vector<std::string> getAutomationsList();
    };
}
