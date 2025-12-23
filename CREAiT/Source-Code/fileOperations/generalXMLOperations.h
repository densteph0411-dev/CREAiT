//--------------------------------------------------------
// $Rev: 498 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-12-14 21:56:42 -0700 (Sun, 14 Dec 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/fileOperations/generalXMLOperations.h $ URL Location
// Copyright Copyright @2024-2025 Systaero, Ltd. All rights reserved.
//--------------------------------------------------------

#pragma once

#include <filesystem>
#include <vector>
#include "../3rdPartyLibraries/pugixml/src/pugixml.hpp"

namespace fs = std::filesystem;

// Namespace for all file operations
namespace fileOperations
{

// defines fileparsing operation
class generalXMLOperations
{

public:

    // Define constructor by passing in the fileName
    generalXMLOperations();
    ~generalXMLOperations();

    // Sets values in xml files specifically under the <fields> node.
    // This function also adds a child to the node for description
    void setFieldValue(std::string elementName,
                       std::string textValue,
                       std::string description,
                       std::string fileName,
                       std::string projectName,
                       std::string context);

    // Opens the provided filename passed in as fileName, and sets the text of element name provided as elementName, to the value provided in textValue.
    // This file addresses possible exceptions in the pugixml library.
    void setElementTextInXMLFile(std::string elementName, std::string textValue, std::string fileName);

    // Returns the value of the attribute (attributeName) of the provided node (node)
    std::string getNodeAttribute(pugi::xml_node &node, std::string attributeName);

    // Sets the attribute (attributeName) of a node (node) to the value (attributeValue)
    // This function handles possible exceptions with pugixml. If there is no attribute with the provided name
    // then it returns the original node
    void setNodeAttribute(pugi::xml_node &node, std::string attributeName, std::string attributeValue);

    // Parses XML file and returns an array containing all chidren element names of the parent "fields".
    // It returns the array as NULL if they are not found or an error occurs.
    std::vector<std::string> parseFieldsInXMLFile(std::string fileName);

    // Opens the XML file at fileName, and creates a new child element under the fields" element named fieldName,
    // with the inner text set to fieldValue.
    // Returns true if successful.
    bool createFieldInXMLFile(std::string fieldName, std::string fieldValue, std::string fileName);

    // Removes the field called fieldName in the XML file fileName
    void removeFieldInXMLFile(std::string fieldName, std::string fileName, std::string context, std::string projectName);

    // Creates a new file at fileName which includes the following structure:
    // < "Element name of context as the root" >
    // </"Element name of context as the root" >
    bool createNewFile(std::string fileName, std::string context);

    // Get all xml filenames in a given directory and returns them in a vector
    std::vector<std::string> getXMLFilesInDirectory(const std::string& directoryPath);

    // Returns a vector of filenames found in the provided directory
    std::vector<std::string> getXmlFiles(const fs::path& directory);

    // Creates a new field in the files. If the context is record, it parses record file format,
    // if it's context is something else it parses the general project file formats
    void createField(std::string elementName,
                     std::string textValue,
                     std::string fileName,
                     std::string projectName);
};
}
