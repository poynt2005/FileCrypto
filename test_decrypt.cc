#include "FileEncryptorDecryptorAPI.h"
#include <iostream>
#include <fstream>

int main()
{
    FileEncryptorDecryptorHandle handle = CreateFileEncryptorDecryptor(0);

    int isprivateKeySet = SetPrivateKeyFromFile(handle, "./private.key");
    std::cout << "isprivateKeySet: " << isprivateKeySet << "\n";

    int isFileDecrypted = DecryptFile(handle, "./file.enc.zip", "file.dec.zip");
    std::cout << "isFileDecrypted: " << isFileDecrypted << "\n";

    ReleaseFileEncryptorDecryptor(handle);

    std::cout << "End\n";
    return 0;
    return 0;
}