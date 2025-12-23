//--------------------------------------------------------
// $Rev: 333 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-09-06 14:51:33 -0700 (Sat, 06 Sep 2025) $ Date of last commit
// $HeadURL: svn://ipc1/requirementTool/Software/CREAiT/CREAiT-Qt-Master/Source-Code/fileOperations/reviewfileoperations.h $ URL Location
// Copyright @2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------
#pragma once

#include "Source-Code/dataObjects/dataDictionary.h"
#include <string>

namespace fileOperations
{
class reviewFileOperations
{
private:

public:

    // Parses the XML record file
    ~reviewFileOperations();

    std::string reviewDirectory;
    // define data and arrays for the project information
    DataDictionary fieldInformationDictionary = *new DataDictionary();

    // Define constructor by passing in the projectFileLocation
    reviewFileOperations(std::string reviewDirectory) :
        reviewDirectory(reviewDirectory){
    }

    // Reads the automation file
    void readFile(std::string recordID);

    // Creates an automation file
    std::string createFile(const std::string reviewDirectory,
                                     const std::string recordType,
                                     const std::string newID);

    // Deletes the Automation File
    void deleteFile();

    // Gets automation record information
    std::string getReviewInformation(const std::string& recordID, const std::string& returnItem);

    // Edit the review file
    void editReviewFile();
};
}
