#pragma once

#include <vector>
#include <records.h>

// dataRecord holds the record information.
class DocumentRecord {
public:

    // Updates the record values.
    void getRecord(std::string ID);

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    std::string create(const std::string documentName = "",
                       const std::string purpose = "",
                       const std::string description = "",
                       const std::string agentID = "");

    // Clear the record information stored in the static buffer.
    void clearDictionaryRecordData();

    // Delete the record.
    void remove(std::string ID);

    // Get information for the specified record.
    std::string getRecordField(const std::string ID,
                               const std::string field);

    // Get the user list
    std::vector<std::string> getListOfRecords(void);

    // Lock the record.
    bool lockRecord(std::string ID);

    // Release the record.
    bool releaseRecord(std::string ID);

private:
    std::string context = "document";
    std::string directory = "documentDirectory";
};
