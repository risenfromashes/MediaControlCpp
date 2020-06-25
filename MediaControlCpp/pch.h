#pragma once
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <rapidjson/writer.h>
#include <uwebsockets/App.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/base.h>
#include <thread>

namespace win {
using namespace winrt;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Media::Control;
using namespace winrt::Windows::Foundation;
}; // namespace win