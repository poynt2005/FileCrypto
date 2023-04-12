#ifndef __FILE_ENCRYPTOR_DECRYPTOR_H__
#define __FILE_ENCRYPTOR_DECRYPTOR_H__

#include <string>
#include <fstream>

namespace FileCrypto
{
    class FileEncryptorDecryptor
    {
    public:
        FileEncryptorDecryptor();
        FileEncryptorDecryptor(const bool);
        FileEncryptorDecryptor(const bool, const unsigned int);
        ~FileEncryptorDecryptor();

        void GetPublicKey(std::string &);
        void GetPrivateKey(std::string &);

        void SetPublicKey(const std::string &);
        void SetPrivateKey(const std::string &);

        const unsigned int GetKeyLength() const;
        void SetKeyLength(const unsigned int);

        bool GetIsGenerateKeyPair();

        bool EncryptFile(const std::string &, const std::string &);
        bool DecryptFile(const std::string &, const std::string &);

        static void SaveStringBufferToFile(const std::string &, const std::string &);

    private:
        void GenerateKeyPair();
        bool isGenerateKeyPair;

        template <typename KeyType, typename CryptoType>
        bool processFile(const KeyType &, std::ifstream &, std::ofstream &);

        std::string privateKeyStringBuf;
        std::string publicKeyStringBuf;

        unsigned int KEY_LENGTH;
        unsigned int MAXIMUM_ENC_LENGTH;
        unsigned int MAXIMUM_DEC_LENGTH;
    };
}
#endif