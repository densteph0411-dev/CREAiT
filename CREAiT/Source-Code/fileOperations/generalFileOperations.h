//--------------------------------------------------------
// $Rev: 399 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-10-24 17:10:54 -0700 (Fri, 24 Oct 2025) $ Date of last commit
// $HeadURL: svn://ipc1/requirementTool/Software/CREAiT/CREAiT-Qt-Master/Source-Code/fileOperations/generalFileOperations.h $ URL Location
// Copyright @2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------
#pragma once

#include "dataDictionary.h"
#include <string>

namespace fileOperations
{
    class GeneralFileOperations
    {
    public:
        // Parses the XML record file
        explicit GeneralFileOperations(const std::string& filePath);
        ~GeneralFileOperations();

        // Define directory as passed in parameter and context
        // (kept for backward compatibility with existing code paths)
        std::string filePath;
        std::string context;

        // define data and dictionary for the project information
        DataDictionary fieldInformationDictionary;

        // Reads the automation file
        void readFile(void);

        // Creates an automation file
        // FIX: return type must match the .cpp definition
        bool createFile(void);

        // Create from a DataDictionary (optionally only persistent fields)
        bool createFile(const DataDictionary& data, bool persistOnly = true);

        // Deletes the Automation File
        void deleteFile(void);

        // Gets automation record information
        std::string GetFieldValue(const std::string returnItem);

        // Gets automation record information
        std::string GetAttributeValue(const std::string FieldName,
                                      const std::string AttributeName);

        // Edit the automation file
        void editFile();

        // Set or Add an attribute if it doesn't exist
        void setAttribute(std::string ID,
                          std::string FieldName,
                          std::string attributeName,
                          std::string value);

        // Remove an attribute if it exists
        void removeAttribute(std::string ID,
                             std::string FieldName,
                             std::string attributeName);

    private:
        std::string filePath_;
    };
}
