#include "FileEncryptorDecryptor.h"
#include "FileEncryptorDecryptorAPI.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <bitset>

std::unordered_map<FileEncryptorDecryptorHandle, std::unique_ptr<FileCrypto::FileEncryptorDecryptor>> instanceMapping;

FileEncryptorDecryptorHandle CreateFileEncryptorDecryptor(const int isGenerate)
{
    auto instance = std::make_unique<FileCrypto::FileEncryptorDecryptor>(static_cast<bool>(isGenerate));
    FileEncryptorDecryptorHandle handle = reinterpret_cast<FileEncryptorDecryptorHandle>(instance.get());
    instanceMapping.insert(std::pair<FileEncryptorDecryptorHandle, std::unique_ptr<FileCrypto::FileEncryptorDecryptor>>(
        handle,
        std::move(instance)));
    return handle;
}

FileEncryptorDecryptorHandle CreateFileEncryptorDecryptorWithKeyLength(const int isGenerate, const unsigned int keyLength)
{
    auto instance = std::make_unique<FileCrypto::FileEncryptorDecryptor>(static_cast<bool>(isGenerate), keyLength);
    FileEncryptorDecryptorHandle handle = reinterpret_cast<FileEncryptorDecryptorHandle>(instance.get());
    instanceMapping.insert(std::pair<FileEncryptorDecryptorHandle, std::unique_ptr<FileCrypto::FileEncryptorDecryptor>>(
        handle,
        std::move(instance)));
    return handle;
}

int CheckHandleExists(const FileEncryptorDecryptorHandle handle)
{
    return instanceMapping.count(handle) == 1;
}

int ReleaseFileEncryptorDecryptor(const FileEncryptorDecryptorHandle handle)
{
    if (!instanceMapping.count(handle))
    {
        return 0;
    }
    instanceMapping.erase(handle);
    return 1;
}

int SaveKeyPair(const FileEncryptorDecryptorHandle handle, const char *publicKeyName, const char *privateKeyName)
{
    if (instanceMapping[handle]->GetIsGenerateKeyPair())
    {
        std::string privateKeyBuf;
        instanceMapping[handle]->GetPrivateKey(privateKeyBuf);

        std::string publicKeyBuf;
        instanceMapping[handle]->GetPublicKey(publicKeyBuf);

        const std::string binKeyLength = std::bitset<32>(instanceMapping[handle]->GetKeyLength()).to_string();

        FileCrypto::FileEncryptorDecryptor::SaveStringBufferToFile(
            publicKeyName,
            binKeyLength + publicKeyBuf);

        FileCrypto::FileEncryptorDecryptor::SaveStringBufferToFile(
            privateKeyName,
            binKeyLength + privateKeyBuf);

        return 1;
    }
    return 0;
}

void SetPublicKey(const FileEncryptorDecryptorHandle handle, const char *key, const int length)
{
    const std::string keyStr(key, length);
    instanceMapping[handle]->SetPublicKey(keyStr);
}

int SetPublicKeyFromFile(const FileEncryptorDecryptorHandle handle, const char *filename)
{
    if (!std::filesystem::exists(filename))
    {
        return 0;
    }

    std::ifstream inFile(filename, std::ios::binary);

    inFile.seekg(0, std::ios::end);
    const size_t fileSize = static_cast<size_t>(inFile.tellg()) - 32;
    inFile.seekg(0, std::ios::beg);

    std::string keyLengthBuf(32, '\0');
    inFile.read(keyLengthBuf.data(), 32);

    unsigned int keyLength = std::bitset<32>(keyLengthBuf).to_ulong();

    inFile.seekg(32, std::ios::beg);
    std::string keyBuf(fileSize, '\0');
    inFile.read(keyBuf.data(), fileSize);
    inFile.close();

    instanceMapping[handle]->SetPublicKey(keyBuf);
    instanceMapping[handle]->SetKeyLength(keyLength);
}

void SetPrivateKey(const FileEncryptorDecryptorHandle handle, const char *key, const int length)
{
    const std::string keyStr(key, length);
    instanceMapping[handle]->SetPrivateKey(keyStr);
}

int SetPrivateKeyFromFile(const FileEncryptorDecryptorHandle handle, const char *filename)
{
    if (!std::filesystem::exists(filename))
    {
        return 0;
    }

    std::ifstream inFile(filename, std::ios::binary);

    inFile.seekg(0, std::ios::end);
    const size_t fileSize = static_cast<size_t>(inFile.tellg()) - 32;
    inFile.seekg(0, std::ios::beg);

    std::string keyLengthBuf(32, '\0');
    inFile.read(keyLengthBuf.data(), 32);

    unsigned int keyLength = std::bitset<32>(keyLengthBuf).to_ulong();

    inFile.seekg(32, std::ios::beg);
    std::string keyBuf(fileSize, '\0');
    inFile.read(keyBuf.data(), fileSize);
    inFile.close();

    instanceMapping[handle]->SetPrivateKey(keyBuf);
    instanceMapping[handle]->SetKeyLength(keyLength);
}

unsigned int GetKeyLength(const FileEncryptorDecryptorHandle handle)
{
    return instanceMapping[handle]->GetKeyLength();
}

void SetKeyLength(const FileEncryptorDecryptorHandle handle, const unsigned int length)
{
    instanceMapping[handle]->SetKeyLength(length);
}

int HasPublicKey(const FileEncryptorDecryptorHandle handle)
{
    std::string key;

    instanceMapping[handle]->GetPublicKey(key);

    if (!key.empty())
    {
        return 1;
    }

    return 0;
}
int HasPrivateKey(const FileEncryptorDecryptorHandle handle)
{
    std::string key;

    instanceMapping[handle]->GetPrivateKey(key);

    if (!key.empty())
    {
        return 1;
    }

    return 0;
}

int EncryptFile(const FileEncryptorDecryptorHandle handle, const char *srcFile, const char *dstFile)
{
    return static_cast<bool>(instanceMapping[handle]->EncryptFile(srcFile, dstFile));
}

int DecryptFile(const FileEncryptorDecryptorHandle handle, const char *srcFile, const char *dstFile)
{
    return static_cast<bool>(instanceMapping[handle]->DecryptFile(srcFile, dstFile));
}
