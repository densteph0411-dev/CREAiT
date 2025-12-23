#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class DataDictionary {
public:
    DataDictionary() = default;
    ~DataDictionary() = default;

    void createField(const std::string& fieldName,
                     const std::string& context,
                     const std::string& fieldValue,
                     const std::string& persistence,
                     const std::string& description);

    bool removeField(const std::string& key, const std::string& context);

    std::string getFieldValue(const std::string& key, const std::string& context) const;

    std::string getFieldPersistence(const std::string& key, const std::string& context) const;

    std::string getFieldDescription(const std::string& key, const std::string& context) const;

    bool setFieldValue(const std::string& key, const std::string& context, const std::string& fieldValue);

    void clearDataDictionary();

    void eraseDataTypeByContext(const std::string& context);

    void clearDataTypeByContext(const std::string& context);

    bool keyExists(const std::string& key, const std::string& context) const;

    // New method to fetch all keys for a given context
    std::vector<std::string> getAllKeysByContext(const std::string& context) const;

    // New method to fetch all keys in this class for a given context and return as a vector
    std::vector<std::string> getAllContexts(void) const;

private:
    struct FieldData {
        std::string value;
        std::string persistence;
        std::string description;
    };

    std::unordered_map<std::string, std::unordered_map<std::string, FieldData>> dictionary;
};
