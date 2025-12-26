#include "dataDictionary.h"

#include "dataDictionary.h"

#include "dpapi_crypto.h"  // add this include

void DataDictionary::createField(const std::string& fieldName,
                                 const std::string& context,
                                 const std::string& fieldValue,
                                 const std::string& persistence,
                                 const std::string& description) {
    dictionary[context][fieldName] = {fieldValue, persistence, description};
}

bool DataDictionary::removeField(const std::string& key, const std::string& context) {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        auto& fields = contextIt->second;
        if (fields.erase(key)) {
            if (fields.empty()) {
                dictionary.erase(contextIt);
            }
            return true;
        }
    }
    return false;
}

std::string DataDictionary::getFieldValue(const std::string& key, const std::string& context) const
{
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end())
    {
        auto fieldIt = contextIt->second.find(key);
        if (fieldIt != contextIt->second.end())
        {
            std::string value = fieldIt->second.value;

            // ✅ Decrypt only credentials fields
            if (context == "credentials" && (key == "userID" || key == "passWord"))
            {
                return dpapiDecryptString(value);
            }

            return value;
        }
    }
    return "";
}

std::string DataDictionary::getFieldPersistence(const std::string& key, const std::string& context) const {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        auto fieldIt = contextIt->second.find(key);
        if (fieldIt != contextIt->second.end()) {
            return fieldIt->second.persistence;
        }
    }
    return "";
}

std::string DataDictionary::getFieldDescription(const std::string& key, const std::string& context) const {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        auto fieldIt = contextIt->second.find(key);
        if (fieldIt != contextIt->second.end()) {
            return fieldIt->second.description;
        }
    }
    return "";
}

bool DataDictionary::setFieldValue(const std::string& key, const std::string& context, const std::string& fieldValue) {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        auto fieldIt = contextIt->second.find(key);
        if (fieldIt != contextIt->second.end()) {
            fieldIt->second.value = fieldValue;
            return true;
        }
    }
    return false;
}


// bool DataDictionary::setFieldValue(const std::string& key,
//                                    const std::string& context,
//                                    const std::string& fieldValue)
// {
//     auto contextIt = dictionary.find(context);
//     if (contextIt == dictionary.end()) return false;

//     auto fieldIt = contextIt->second.find(key);
//     if (fieldIt == contextIt->second.end()) return false;

//     std::string valueToStore = fieldValue;

//     if (context == "credentials" && (key == "userID" || key == "passWord"))
//     {
//         // prevent double-encryption
//         if (fieldValue.rfind("DPAPI:v1:", 0) != 0)
//         {
//             valueToStore = dpapiEncryptString(fieldValue);
//             if (valueToStore.empty() && !fieldValue.empty())
//                 return false;
//         }
//     }

//     fieldIt->second.value = valueToStore;
//     return true;
// }

void DataDictionary::clearDataDictionary() {
    dictionary.clear();
}

void DataDictionary::eraseDataTypeByContext(const std::string& context) {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        dictionary.erase(contextIt);
    }
}

void DataDictionary::clearDataTypeByContext(const std::string& context) {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        contextIt->second.clear(); // Clear the data without removing the key
    }
}

bool DataDictionary::keyExists(const std::string& key, const std::string& context) const {
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        return contextIt->second.find(key) != contextIt->second.end();
    }
    return false;
}

// New method to get all keys for a given context
std::vector<std::string> DataDictionary::getAllKeysByContext(const std::string& context) const {
    std::vector<std::string> keys;
    auto contextIt = dictionary.find(context);
    if (contextIt != dictionary.end()) {
        for (const auto& field : contextIt->second) {
            keys.push_back(field.first);
        }
    }
    return keys;
}

std::vector<std::string> DataDictionary::getAllContexts(void) const {
    std::vector<std::string> contexts;
    for (const auto& pair : dictionary) {
        contexts.push_back(pair.first);
    }
    return contexts;
}
