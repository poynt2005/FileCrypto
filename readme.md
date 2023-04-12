# FileCrypto

## 只能運行於 win32 x64 平台

這是一個使用 cryptopp 做的測試，目的是要用該函式庫使用 RSA keypair 的形式加密以及解密檔案  
使用方法:

1. 先運行 set.py
2. cd 到 node 目錄，運行  
   a. npm run test-gen: 生成 RSA keypair with 1024 key length  
   b. npm run test-enc: 下載檔案，使用 public.key 加密之  
   b. npm run test-dec: 使用 private.key 解密檔案
3. 運行 clear.py 清除

## issue

1. 加密很快，解密很久

## 未來要做的

1. 使用 thread 加速流程
