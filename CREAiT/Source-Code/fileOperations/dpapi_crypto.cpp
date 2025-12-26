#include "dpapi_crypto.h"

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "Crypt32.lib")
#endif

#include <iostream>
#include <cstring>

#include <QByteArray>

static const char* kEncPrefix = "DPAPI:v1:";

// Optional: "extra entropy" binds encryption to your app (not a secret)
static DATA_BLOB makeEntropyBlob()
{
    static const char kEntropy[] = "CREAiT-DPAPI-Entropy-v1";
    DATA_BLOB blob{};
    blob.pbData = (BYTE*)kEntropy;
    blob.cbData = (DWORD)strlen(kEntropy);
    return blob;
}

static bool startsWith(const std::string& s, const char* pfx)
{
    size_t n = std::strlen(pfx);
    return s.size() >= n && s.compare(0, n, pfx) == 0;
}

static std::string lastErrorString()
{
#ifdef _WIN32
    DWORD err = GetLastError();
    return "GetLastError=" + std::to_string(err);
#else
    return "Not on Windows";
#endif
}

std::string dpapiEncryptString(const std::string& plainUtf8)
{
#ifdef _WIN32
    if (plainUtf8.empty())
        return {};

    QByteArray in = QByteArray::fromStdString(plainUtf8);

    DATA_BLOB inBlob{};
    inBlob.pbData = reinterpret_cast<BYTE*>(in.data());
    inBlob.cbData = static_cast<DWORD>(in.size());

    DATA_BLOB outBlob{};
    DATA_BLOB entropy = makeEntropyBlob();

    BOOL ok = CryptProtectData(
        &inBlob,
        L"CREAiT credential field",
        &entropy,
        nullptr,
        nullptr,
        CRYPTPROTECT_UI_FORBIDDEN,
        &outBlob
        );

    if (!ok)
    {
        std::cerr << "DPAPI encrypt failed: " << lastErrorString() << std::endl;
        return {};
    }

    QByteArray encrypted(reinterpret_cast<const char*>(outBlob.pbData),
                         static_cast<int>(outBlob.cbData));

    LocalFree(outBlob.pbData);

    std::string b64 = encrypted.toBase64().toStdString();
    return std::string(kEncPrefix) + b64;
#else
    (void)plainUtf8;
    return {};
#endif
}

std::string dpapiDecryptString(const std::string& storedValue)
{
#ifdef _WIN32
    if (storedValue.empty())
        return {};

    // Not encrypted? Return as-is.
    if (!startsWith(storedValue, kEncPrefix))
        return storedValue;

    std::string b64 = storedValue.substr(std::strlen(kEncPrefix));
    QByteArray encrypted = QByteArray::fromBase64(QByteArray::fromStdString(b64));
    if (encrypted.isEmpty())
        return {};

    DATA_BLOB inBlob{};
    inBlob.pbData = reinterpret_cast<BYTE*>(encrypted.data());
    inBlob.cbData = static_cast<DWORD>(encrypted.size());

    DATA_BLOB outBlob{};
    DATA_BLOB entropy = makeEntropyBlob();

    BOOL ok = CryptUnprotectData(
        &inBlob,
        nullptr,
        &entropy,
        nullptr,
        nullptr,
        CRYPTPROTECT_UI_FORBIDDEN,
        &outBlob
        );

    if (!ok)
    {
        std::cerr << "DPAPI decrypt failed: " << lastErrorString() << std::endl;
        return {};
    }

    QByteArray plain(reinterpret_cast<const char*>(outBlob.pbData),
                     static_cast<int>(outBlob.cbData));

    LocalFree(outBlob.pbData);

    return plain.toStdString();
#else
    (void)storedValue;
    return {};
#endif
}
