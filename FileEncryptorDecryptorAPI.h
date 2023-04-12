#ifndef __FILE_ENCRYPTOR_DECRYPTOR_API_H__
#define __FILE_ENCRYPTOR_DECRYPTOR_API_H__

#ifdef BUILDFILEENCDECAPI
#define EXPORTFILEENCDECAPI __declspec(dllexport)
#else
#define EXPORTFILEENCDECAPI __declspec(dllimport)
#endif

typedef unsigned long long FileEncryptorDecryptorHandle;

#ifdef __cplusplus
extern "C"
{
#endif
    EXPORTFILEENCDECAPI FileEncryptorDecryptorHandle CreateFileEncryptorDecryptor(const int);
    EXPORTFILEENCDECAPI FileEncryptorDecryptorHandle CreateFileEncryptorDecryptorWithKeyLength(const int, const unsigned int);
    EXPORTFILEENCDECAPI int CheckHandleExists(const FileEncryptorDecryptorHandle);
    EXPORTFILEENCDECAPI int ReleaseFileEncryptorDecryptor(const FileEncryptorDecryptorHandle);
    EXPORTFILEENCDECAPI int SaveKeyPair(const FileEncryptorDecryptorHandle, const char *, const char *);
    EXPORTFILEENCDECAPI void SetPublicKey(const FileEncryptorDecryptorHandle, const char *, const int);
    EXPORTFILEENCDECAPI int SetPublicKeyFromFile(const FileEncryptorDecryptorHandle, const char *);
    EXPORTFILEENCDECAPI void SetPrivateKey(const FileEncryptorDecryptorHandle, const char *, const int);
    EXPORTFILEENCDECAPI int SetPrivateKeyFromFile(const FileEncryptorDecryptorHandle, const char *);
    EXPORTFILEENCDECAPI unsigned int GetKeyLength(const FileEncryptorDecryptorHandle);
    EXPORTFILEENCDECAPI void SetKeyLength(const FileEncryptorDecryptorHandle, const unsigned int);
    EXPORTFILEENCDECAPI int HasPublicKey(const FileEncryptorDecryptorHandle);
    EXPORTFILEENCDECAPI int HasPrivateKey(const FileEncryptorDecryptorHandle);
    EXPORTFILEENCDECAPI int EncryptFile(const FileEncryptorDecryptorHandle, const char *, const char *);
    EXPORTFILEENCDECAPI int DecryptFile(const FileEncryptorDecryptorHandle, const char *, const char *);
#ifdef __cplusplus
}
#endif

#endif