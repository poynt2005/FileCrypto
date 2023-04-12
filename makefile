cxx = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe"
cflags = /I./cryptocpp/incl /I"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\include" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\shared" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\winrt"  
ldflags = /LIBPATH:./cryptocpp/lib /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64"  /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\ucrt\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.18362.0\um\x64" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\lib\x64"
libs = cryptlib.lib


dll:
	$(cxx) $(cflags) /MD /std:c++17 /c FileEncryptorDecryptor.cc /Fo:FileEncryptorDecryptor.obj
	$(cxx) $(cflags)  /MD /std:c++17 /DBUILDFILEENCDECAPI /c FileEncryptorDecryptorAPI.cc /Fo:FileEncryptorDecryptorAPI.obj
	$(cxx)  /LD FileEncryptorDecryptor.obj FileEncryptorDecryptorAPI.obj /link $(ldflags) $(libs) /OUT:FileEncryptorDecryptorAPI.dll /IMPLIB:FileEncryptorDecryptorAPI.lib

test:
	gcc -c  test_generate_key.c -o test_generate_key.o
	g++ -c -std=c++17 test_decrypt.cc -o test_decrypt.o
	g++ -c -std=c++17 test_encrypt.cc -o test_encrypt.o
	gcc -L./ test_generate_key.o -o test_generate_key.exe -lFileEncryptorDecryptorAPI
	g++ -L./ test_decrypt.o -o test_decrypt.exe -lFileEncryptorDecryptorAPI
	g++ -L./ test_encrypt.o -o test_encrypt.exe -lFileEncryptorDecryptorAPI