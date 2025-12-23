//--------------------------------------------------------
// $Rev: 333 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-09-06 15:51:33 -0600 (Sat, 06 Sep 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-Master/Source-Code/fileOperations/recordFileOperations.h $ URL Location
// Copyright @2024-2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------
#pragma once

#include "Source-Code/dataObjects/dataDictionary.h"
#include <string>

namespace fileOperations
{
    class parseRecordFile
    {
    private:
        std::string keyValue;
        std::string type;

    public:

        // Parses the XML record file
        ~parseRecordFile();

        std::string recordDirectory;
        static std::vector<std::string> linkName;
        static std::vector<std::string> linkDirection;
        static std::vector<std::string> linkType;
        static std::vector<std::string> linkTitle;

        static std::vector<std::string> versionNumber;
        static std::vector<std::string> versionDescription;
        static std::vector<std::string> versionDate;

        // Define constructor by passing in the projectFileLocation
        parseRecordFile(std::string recordDirectory) : recordDirectory(recordDirectory){

        }

        // define data and arrays for the project information
        DataDictionary fieldInformationDictionary = *new DataDictionary();

        void readRecordFile(std::string recordID);
        std::string createRecordFile(const std::string recordDirectory, const std::string recordType, const std::string newID);
        void deleteRecordFile();
        std::string getRecordInformation(const std::string& recordID, const std::string& returnItem);
        void linkManipulate(std::string linkID, bool addLink, bool directionUp, std::string linkType);
        void updateVersion(std::string description, std::string dateTime);
        void editRecordFile();
    };
}
