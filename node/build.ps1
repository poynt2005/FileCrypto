node-gyp configure; node-gyp build; echo F | xcopy /Y .\build\Release\FileCrypto.node .\FileCrypto.node; rm .\build -r -force