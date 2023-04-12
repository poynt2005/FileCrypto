#include <Windows.h>
#include <string>
#include <Napi.h>

Napi::Value Node_Downloader(const Napi::CallbackInfo &info)
{
    auto env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong Arguments, excepted 2 strings");
        return env.Null();
    }

    if (!info[0].IsString() && !info[1].IsString())
    {
        Napi::TypeError::New(env, "Wrong Argument, position 0 and 1 excepted string");
        return env.Null();
    }

    const auto url = info[0].As<Napi::String>().Utf8Value();
    const auto filename = info[1].As<Napi::String>().Utf8Value();

    HRESULT hResult = URLDownloadToFile(
        nullptr,
        url.data(),
        filename.data(),
        0,
        nullptr);

    return Napi::Boolean::New(env, hResult == S_OK);
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{

    exports.Set(
        Napi::String::New(env, "Downloader"),
        Napi::Function::New(env, Node_Downloader));

    return exports;
}

NODE_API_MODULE(FileDownloader, Initialize)