#include "FileEncryptorDecryptorAPI.h"
#include <stdio.h>

int main()
{
    FileEncryptorDecryptorHandle handle = CreateFileEncryptorDecryptorWithKeyLength(1, 512);
    SaveKeyPair(handle, "public.key", "private.key");
    ReleaseFileEncryptorDecryptor(handle);
    printf("End\n");
    return 0;
}