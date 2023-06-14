#include "pch.h"
#include "Music.h"
#if __has_include("Music.g.cpp")
#include "Music.g.cpp"
#endif

namespace winrt::EasyMusicPlayer::implementation
{
    Music::Music(winrt::hstring const& name, winrt::hstring const& icon, winrt::hstring const& path, int32_t const& total)
    {
        this->name = name;
        this->icon = icon;
        this->path = path;
        this->total = total;
    }
    Music::Music(winrt::Windows::Data::Json::JsonObject const& json)
    {
        if (json.HasKey(L"name"))
            Name(json.GetNamedString(L"name"));
        if (json.HasKey(L"icon"))
            Icon(json.GetNamedString(L"icon"));
        if (json.HasKey(L"path"))
            Path(json.GetNamedString(L"path"));
        if (json.HasKey(L"total"))
            Total(static_cast<int>(json.GetNamedNumber(L"total")));
    }
    void Music::Path(winrt::hstring const& value)
    {
        this->path = value;
    }
    void Music::Icon(winrt::hstring const& value)
    {
        this->icon = value;
    }
    void Music::Name(winrt::hstring const& value)
    {
        this->name = value;
    }
    void Music::Total(int32_t const& value)
    {
        this->total = value;
    }
    winrt::hstring Music::Time()
    {
        return TimeFormat(this->total);
    }
    winrt::hstring Music::TimeFormat(int32_t const& sec)
    {
        std::stringstream mm;
        mm << std::setw(2) << std::setfill('0') << sec / 60 << ":" << std::setw(2) << std::setfill('0') << sec % 60;
        return winrt::to_hstring(mm.str());
    }
    Windows::Data::Json::JsonObject Music::ToJson()
    {
        Windows::Data::Json::JsonObject object;
        object.Insert(L"name", Windows::Data::Json::JsonValue::CreateStringValue(this->name));
        object.Insert(L"icon", Windows::Data::Json::JsonValue::CreateStringValue(this->icon));
        object.Insert(L"path", Windows::Data::Json::JsonValue::CreateStringValue(this->path));
        object.Insert(L"total", Windows::Data::Json::JsonValue::CreateNumberValue(this->total));
        return object;
    }
}
