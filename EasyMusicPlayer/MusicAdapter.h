#pragma once

#include "MusicAdapter.g.h"
#include "Music.h"
#include "API/HashMap.hpp"

namespace winrt::EasyMusicPlayer::implementation
{
    struct MusicAdapter : MusicAdapterT<MusicAdapter>
    {
        MusicAdapter() = default;
        int MusicHash(winrt::hstring const& path);
        void AddMusic(winrt::hstring const& path);
        EasyMusicPlayer::Music GetMusic(winrt::hstring const& path, winrt::hstring const& name, int32_t const& total);
        EasyMusicPlayer::Music GetMusic(winrt::Windows::Data::Json::JsonObject const& json);
        void RemoveMusic(hstring const& path);
        void RemoveMusic(int hash);
        Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> GetLocalFolder();
        Windows::Foundation::IAsyncAction SaveConfig();
        Windows::Foundation::IAsyncAction LoadConfig();
        Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music>> GetAll();
        Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music>> GetRandom();

    private:
        HashMap<int, winrt::hstring> map;
        winrt::Windows::Data::Json::JsonObject m_config;
    };
}

namespace winrt::EasyMusicPlayer::factory_implementation
{
    struct MusicAdapter : MusicAdapterT<MusicAdapter, implementation::MusicAdapter>
    {
    };
}
