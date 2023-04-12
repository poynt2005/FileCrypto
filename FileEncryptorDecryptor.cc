#include "FileEncryptorDecryptor.h"

#include <cryptlib.h>
#include <rsa.h>
#include <osrng.h>
#include <files.h>
#include <hex.h>
#include <base64.h>
#include <filters.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <type_traits>
#include <exception>

namespace FileCrypto
{

    FileEncryptorDecryptor::FileEncryptorDecryptor()
        : FileEncryptorDecryptor(false) {}

    FileEncryptorDecryptor::FileEncryptorDecryptor(const bool isGen)
        : FileEncryptorDecryptor(isGen, 3072)
    {
    }

    FileEncryptorDecryptor::FileEncryptorDecryptor(const bool isGen, const unsigned int keyLength)
        : isGenerateKeyPair(isGen), KEY_LENGTH(keyLength)
    {
        if (isGenerateKeyPair)
        {
            GenerateKeyPair();
        }

        MAXIMUM_ENC_LENGTH = (KEY_LENGTH / 8) - 2 * 20 - 2;
        MAXIMUM_DEC_LENGTH = (KEY_LENGTH / 8);
    }

    FileEncryptorDecryptor::~FileEncryptorDecryptor() {}

    void FileEncryptorDecryptor::GenerateKeyPair()
    {
        /** Create a Random digit generator*/
        CryptoPP::AutoSeededRandomPool prng;

        /** Generate RSA private*/
        CryptoPP::RSA::PrivateKey privateKey;
        privateKey.GenerateRandomWithKeySize(prng, KEY_LENGTH);

        /** Generate public key by private */
        CryptoPP::RSA::PublicKey publicKey(privateKey);

        /** Use StringSink to store the key*/
        CryptoPP::StringSink privateKeySink(privateKeyStringBuf);
        CryptoPP::StringSink publicKeySink(publicKeyStringBuf);

        privateKey.Save(privateKeySink);
        publicKey.Save(publicKeySink);
    }

    bool FileEncryptorDecryptor::EncryptFile(const std::string &srcFile, const std::string &dstFile)
    {
        CryptoPP::StringSource publicKeySrc(publicKeyStringBuf, true);
        CryptoPP::RSA::PublicKey publicKey;

        try
        {
            publicKey.Load(publicKeySrc);
        }
        catch (CryptoPP::Exception &ex)
        {
            return false;
        }

        if (!std::filesystem::exists(srcFile))
        {
            return false;
        }

        std::ifstream inFile(
            std::filesystem::absolute(srcFile).string(),
            std::ios::binary);

        std::ofstream outFile(
            std::filesystem::absolute(dstFile).string(),
            std::ios::binary);

        return processFile<CryptoPP::RSA::PublicKey, CryptoPP::RSAES_OAEP_SHA_Encryptor>(
            publicKey,
            inFile,
            outFile);
    };

    bool FileEncryptorDecryptor::DecryptFile(const std::string &srcFile, const std::string &dstFile)
    {
        CryptoPP::StringSource privateKeySrc(privateKeyStringBuf, true);
        CryptoPP::RSA::PrivateKey privateKey;

        try
        {
            privateKey.Load(privateKeySrc);
        }
        catch (CryptoPP::Exception &ex)
        {
            return false;
        }

        if (!std::filesystem::exists(srcFile))
        {
            return false;
        }

        CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
        CryptoPP::AutoSeededRandomPool prng;

        std::ifstream inFile(
            std::filesystem::absolute(srcFile).string(),
            std::ios::binary);

        std::ofstream outFile(
            std::filesystem::absolute(dstFile).string(),
            std::ios::binary);

        return processFile<CryptoPP::RSA::PrivateKey, CryptoPP::RSAES_OAEP_SHA_Decryptor>(
            privateKey,
            inFile,
            outFile);
    };

    void FileEncryptorDecryptor::GetPrivateKey(std::string &key)
    {
        key = privateKeyStringBuf;
    }

    void FileEncryptorDecryptor::SetPublicKey(const std::string &inputkeyBuff)
    {
        publicKeyStringBuf = inputkeyBuff;
    }

    void FileEncryptorDecryptor::SetKeyLength(const unsigned int length)
    {
        if (length % 8 != 0)
        {
            throw std::exception("Key length must divided by 8");
        }

        if (isGenerateKeyPair)
        {
            GenerateKeyPair();
        }

        KEY_LENGTH = length;
        MAXIMUM_ENC_LENGTH = (KEY_LENGTH / 8) - 2 * 20 - 2;
        MAXIMUM_DEC_LENGTH = (KEY_LENGTH / 8);
    }

    void FileEncryptorDecryptor::GetPublicKey(std::string &key)
    {
        key = publicKeyStringBuf;
    }

    void FileEncryptorDecryptor::SetPrivateKey(const std::string &inputkeyBuff)
    {
        privateKeyStringBuf = inputkeyBuff;
    }

    bool FileEncryptorDecryptor::GetIsGenerateKeyPair()
    {
        return isGenerateKeyPair;
    }

    void FileEncryptorDecryptor::SaveStringBufferToFile(const std::string &filename, const std::string &keyStr)
    {
        auto absPath = std::filesystem::absolute(filename);
        std::ofstream outFile(absPath.string(), std::ios::binary);
        outFile.write(&(keyStr.data()[0]), keyStr.length());
        outFile.close();
    }

    const unsigned int FileEncryptorDecryptor::GetKeyLength() const
    {
        return KEY_LENGTH;
    }

    template <typename KeyType, typename CryptoType>
    bool FileEncryptorDecryptor::processFile(const KeyType &key, std::ifstream &inFile, std::ofstream &outFile)
    {
        if constexpr (
            !(std::is_same_v<CryptoType, CryptoPP::RSAES_OAEP_SHA_Encryptor> && std::is_same_v<KeyType, CryptoPP::RSA::PublicKey>)&&!(std::is_same_v<CryptoType, CryptoPP::RSAES_OAEP_SHA_Decryptor> && std::is_same_v<KeyType, CryptoPP::RSA::PrivateKey>))
        {
            return false;
        }

        CryptoPP::AutoSeededRandomPool prng;

        auto cryptoer = CryptoType(key);

        inFile.seekg(0, std::ios::end);
        const size_t fileSize = inFile.tellg();
        inFile.seekg(std::ios::beg);

        size_t outFileSize = 0;

        int MAXIMUM_CHUNK_LENGTH;

        if constexpr (std::is_same_v<CryptoType, CryptoPP::RSAES_OAEP_SHA_Encryptor>)
        {
            MAXIMUM_CHUNK_LENGTH = MAXIMUM_ENC_LENGTH;
        }
        else if constexpr (std::is_same_v<CryptoType, CryptoPP::RSAES_OAEP_SHA_Decryptor>)
        {
            MAXIMUM_CHUNK_LENGTH = MAXIMUM_DEC_LENGTH;
        }

        for (size_t pos = 0; pos < fileSize; pos += MAXIMUM_CHUNK_LENGTH)
        {

            inFile.seekg(pos, std::ios::beg);
            outFile.seekp(outFileSize, std::ios::beg);
            size_t readLength = ((pos + MAXIMUM_CHUNK_LENGTH) >= fileSize) ? (fileSize - pos) : MAXIMUM_CHUNK_LENGTH;

            std::string chunkData(readLength, '\0');
            inFile.read(chunkData.data(), readLength);

            try
            {
                std::string processedData;

                if constexpr (std::is_same_v<CryptoType, CryptoPP::RSAES_OAEP_SHA_Encryptor>)
                {
                    CryptoPP::StringSource(
                        chunkData,
                        true,
                        new CryptoPP::PK_EncryptorFilter(
                            prng,
                            cryptoer,
                            new CryptoPP::StringSink(processedData)));
                }
                else if constexpr (std::is_same_v<CryptoType, CryptoPP::RSAES_OAEP_SHA_Decryptor>)
                {
                    CryptoPP::StringSource(
                        chunkData,
                        true,
                        new CryptoPP::PK_DecryptorFilter(
                            prng,
                            cryptoer,
                            new CryptoPP::StringSink(processedData)));
                }

                outFileSize += processedData.length();
                outFile.write(processedData.data(), processedData.length());
            }
            catch (CryptoPP::Exception &ex)
            {
                return false;
            }
        }

        inFile.close();
        outFile.close();

        return true;
    }
}