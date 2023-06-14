#pragma once

#include "Music.g.h"

namespace winrt::EasyMusicPlayer::implementation
{
    struct Music : MusicT<Music>
    {
        Music(winrt::hstring const& name, winrt::hstring const& icon, winrt::hstring const& path, int32_t const& total);
        Music(winrt::Windows::Data::Json::JsonObject const& json);
        winrt::hstring Path() { return path; }
        void Path(winrt::hstring const& value);
        winrt::hstring Icon() { return icon; }
        void Icon(winrt::hstring const& value);
        winrt::hstring Name() { return name; }
        void Name(winrt::hstring const& value);
        int32_t Total() { return total; }
        void Total(int32_t const& value);
        friend void swap(Music& a, Music& b) {
            using std::swap;
            swap(a.path, b.path);
            swap(a.name, b.name);
            swap(a.icon, b.icon);
            swap(a.total, b.total);
        }
    private:
        winrt::hstring path;
        winrt::hstring icon;
        winrt::hstring name;
        int32_t total;
    public:
        winrt::hstring TimeFormat(int32_t const& sec);
        winrt::hstring Time();
        Windows::Data::Json::JsonObject ToJson();
    };
}

namespace winrt::EasyMusicPlayer::factory_implementation
{
    struct Music : MusicT<Music, implementation::Music>
    {
    };
}
