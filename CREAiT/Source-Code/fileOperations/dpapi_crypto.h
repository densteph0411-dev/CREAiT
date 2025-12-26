#pragma once

#include <string>

// Encrypts UTF-8 string -> returns "DPAPI:v1:<base64>"
// Empty input returns empty output.
// On failure returns empty string.
std::string dpapiEncryptString(const std::string& plainUtf8);

// Decrypts "DPAPI:v1:<base64>" -> returns original UTF-8 string
// If input does NOT start with prefix, it returns the input as-is.
// Empty input returns empty output.
// On failure returns empty string.
std::string dpapiDecryptString(const std::string& storedValue);
