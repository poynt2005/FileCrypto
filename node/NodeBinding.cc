#include "../FileEncryptorDecryptorAPI.h"
#include <Napi.h>
#include <string>

Napi::Value Node_CreateFileEncryptorDecryptor(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    bool isGenerate = false;
    unsigned int keyLength = 0;

    if (info.Length() >= 1)
    {
        if (!info[0].IsBoolean())
        {
            Napi::TypeError::New(
                env,
                "Wrong Arguments, position 0 excepted a boolean")
                .ThrowAsJavaScriptException();
            return env.Null();
        }
        isGenerate = info[0].As<Napi::Boolean>().Value();

        if (info.Length() >= 2)
        {
            if (!info[1].IsNumber())
            {
                Napi::TypeError::New(
                    env,
                    "Wrong Arguments, position 1 excepted a number")
                    .ThrowAsJavaScriptException();
                return env.Null();
            }

            auto tmpLength = info[1].As<Napi::Number>().Int32Value();

            if (tmpLength <= 0)
            {
                Napi::TypeError::New(
                    env,
                    "Wrong Arguments, number of position 1 must greater than 0")
                    .ThrowAsJavaScriptException();
                return env.Null();
            }

            keyLength = static_cast<unsigned int>(tmpLength);
        }
    }

    FileEncryptorDecryptorHandle handle;

    if (keyLength)
    {
        handle = CreateFileEncryptorDecryptorWithKeyLength(
            static_cast<int>(isGenerate),
            keyLength);
    }
    else
    {
        handle = CreateFileEncryptorDecryptor(static_cast<int>(isGenerate));
    }

    return Napi::String::New(env, std::to_string(handle));
}

Napi::Value Node_ReleaseFileEncryptorDecryptor(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, excepted a string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0 excepted a string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, ReleaseFileEncryptorDecryptor(handle));
}

Napi::Value Node_SaveKeyPair(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 3)
    {
        Napi::TypeError::New(
            env,
            "Wrong Arguments, excepted 3 strings")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() && !info[1].IsString() && !info[2].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0, 1 and 2 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto publicKeyPath = info[1].As<Napi::String>().Utf8Value();
    const auto privateKeyPath = info[2].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, SaveKeyPair(
                                       handle,
                                       publicKeyPath.data(),
                                       privateKeyPath.data()));
}

Napi::Value Node_SetPublicKeyFromFile(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(
            env,
            "Wrong Arguments, excepted 2 strings")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() && !info[1].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0 and 1 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto keyPath = info[1].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, SetPublicKeyFromFile(handle, keyPath.data()));
}

Napi::Value Node_SetPrivateKeyFromFile(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(
            env,
            "Wrong Arguments, excepted 2 strings")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() && !info[1].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0 and 1 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto keyPath = info[1].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, SetPrivateKeyFromFile(handle, keyPath.data()));
}

Napi::Value Node_EncryptFile(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 3)
    {
        Napi::TypeError::New(
            env,
            "Wrong Arguments, excepted 3 strings")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() && !info[1].IsString() && !info[2].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0, 1 and 2 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto srcPath = info[1].As<Napi::String>().Utf8Value();
    const auto dstPath = info[2].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, EncryptFile(
                                       handle,
                                       srcPath.data(),
                                       dstPath.data()));
}

Napi::Value Node_DecryptFile(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 3)
    {
        Napi::TypeError::New(
            env,
            "Wrong Arguments, excepted 3 strings")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() && !info[1].IsString() && !info[2].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0, 1 and 2 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto srcPath = info[1].As<Napi::String>().Utf8Value();
    const auto dstPath = info[2].As<Napi::String>().Utf8Value();

    return Napi::Boolean::New(env, DecryptFile(
                                       handle,
                                       srcPath.data(),
                                       dstPath.data()));
}

Napi::Value Node_HasPublicKey(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, excepted a string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, HasPublicKey(handle));
}

Napi::Value Node_HasPrivateKey(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, excepted a string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString())
    {
        Napi::TypeError::New(
            env,
            "Wrong Argument, position 0 excepted string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    auto handleStr = info[0].As<Napi::String>().Utf8Value();
    const auto handle = static_cast<FileEncryptorDecryptorHandle>(std::stoull(std::move(handleStr)));

    if (!CheckHandleExists(handle))
    {
        Napi::Error::New(
            env,
            "Handle is NOT exists")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, HasPrivateKey(handle));
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{

    exports.Set(
        Napi::String::New(env, "CreateFileEncryptorDecryptor"),
        Napi::Function::New(env, Node_CreateFileEncryptorDecryptor));

    exports.Set(
        Napi::String::New(env, "ReleaseFileEncryptorDecryptor"),
        Napi::Function::New(env, Node_ReleaseFileEncryptorDecryptor));

    exports.Set(
        Napi::String::New(env, "SaveKeyPair"),
        Napi::Function::New(env, Node_SaveKeyPair));

    exports.Set(
        Napi::String::New(env, "SetPublicKeyFromFile"),
        Napi::Function::New(env, Node_SetPublicKeyFromFile));

    exports.Set(
        Napi::String::New(env, "SetPrivateKeyFromFile"),
        Napi::Function::New(env, Node_SetPrivateKeyFromFile));

    exports.Set(
        Napi::String::New(env, "EncryptFile"),
        Napi::Function::New(env, Node_EncryptFile));

    exports.Set(
        Napi::String::New(env, "DecryptFile"),
        Napi::Function::New(env, Node_DecryptFile));

    exports.Set(
        Napi::String::New(env, "HasPrivateKey"),
        Napi::Function::New(env, Node_HasPrivateKey));

    exports.Set(
        Napi::String::New(env, "HasPublicKey"),
        Napi::Function::New(env, Node_HasPublicKey));

    return exports;
}

NODE_API_MODULE(FileCrypto, Initialize)