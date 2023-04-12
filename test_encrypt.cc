#include "FileEncryptorDecryptorAPI.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

int main()
{
    FileEncryptorDecryptorHandle handle = CreateFileEncryptorDecryptor(0);

    int ispublicKeySet = SetPublicKeyFromFile(handle, "./public.key");
    std::cout << "ispublicKeySet: " << ispublicKeySet << "\n";

    int isFileEncrypted = EncryptFile(handle, "./file.zip", "file.enc.zip");
    std::cout << "isFileEncrypted: " << isFileEncrypted << "\n";

    ReleaseFileEncryptorDecryptor(handle);

    std::cout << "End\n";
    return 0;
}