#include "pch.h"
#include "MusicAdapter.h"
#if __has_include("MusicAdapter.g.cpp")
#include "MusicAdapter.g.cpp"
#endif
#include "API/Hash.hpp"
#include "API/Random.hpp"

namespace winrt::EasyMusicPlayer::implementation
{
    // 计算Hash
    int MusicAdapter::MusicHash(hstring const& path)
    {
        return Hash(to_string(path));
    }
    // 添加Music
    void MusicAdapter::AddMusic(hstring const& path)
    {
        int hash = MusicHash(path);
        if (!map.find(hash)) {
            map.insert(hash, path);
            OutputDebugStringW((L"\n[Debug]添加Music,hash:" + to_hstring(hash) + L",path:" + path + L"\n").c_str());
        }
    }
    // 获取Music类
    EasyMusicPlayer::Music MusicAdapter::GetMusic(winrt::hstring const& path, winrt::hstring const& name, int32_t const& total)
    {
        return EasyMusicPlayer::Music(name, L"", path, total);
    }
    // 获取Music类
    EasyMusicPlayer::Music MusicAdapter::GetMusic(winrt::Windows::Data::Json::JsonObject const& json)
    {
        return EasyMusicPlayer::Music(json);
    }
    // 删除
    void MusicAdapter::RemoveMusic(hstring const& path)
    {
        int hash = MusicHash(path);
        if (map.find(hash)) {
            map.erase(hash);
            OutputDebugStringW((L"\n[Debug]移除Music,hash:" + to_hstring(hash) + L",path:" + path + L"\n").c_str());
        }
    }
    // 删除
    void MusicAdapter::RemoveMusic(int hash)
    {
        if (map.find(hash)) {
            map.erase(hash);
        }
    }
    // 获取当前文件夹
    Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> MusicAdapter::GetLocalFolder()
    {
        TCHAR buffer[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, buffer);
        co_return co_await winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(winrt::to_hstring(buffer));
    }
    // 获取全部
    Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music>> MusicAdapter::GetAll()
    {
        winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music> musics = winrt::single_threaded_observable_vector<EasyMusicPlayer::Music>();
        std::vector<hstring> vec;
        map.getall(vec);
        for (hstring p : vec) {
            Windows::Storage::StorageFile file = co_await Windows::Storage::StorageFile::GetFileFromPathAsync(p);
            if (file.FileType() == L".mp3") {
                auto dur = co_await file.Properties().GetMusicPropertiesAsync();
                int seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::duration(dur.Duration().count())).count());
                musics.Append(GetMusic(file.Path(), file.DisplayName(), seconds));
            }
            else {
                RemoveMusic(p);
            }
        }
        co_return musics;
    }
    // 获取随机
    Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music>> MusicAdapter::GetRandom()
    {
        winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music> musics = winrt::single_threaded_observable_vector<EasyMusicPlayer::Music>();

        Random random;

        std::vector<std::pair<int, int>> lib;
        for (int i = 0; i < map.size(); ++i)
        {
            lib.push_back(std::pair(random(), i));
        }
        std::sort(lib.begin(), lib.end(), [](std::pair<int, int> lhs, std::pair<int, int> rhs) {
            return lhs.second < rhs.second;
            });
        std::vector<hstring> vec;
        map.getall(vec);
        for (auto i : lib) {
            hstring p = vec[i.second];
            Windows::Storage::StorageFile file = co_await Windows::Storage::StorageFile::GetFileFromPathAsync(p);
            if (file.FileType() == L".mp3") {
                auto dur = co_await file.Properties().GetMusicPropertiesAsync();
                int seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::duration(dur.Duration().count())).count());
                musics.Append(GetMusic(file.Path(), file.DisplayName(), seconds));
            }
            else {
                RemoveMusic(p);
            }
        }

        co_return musics;
    }

    // 保存配置
    Windows::Foundation::IAsyncAction MusicAdapter::SaveConfig()
    {
        winrt::Windows::Data::Json::JsonArray musicArray;
        std::vector<hstring> vec;
        map.getall(vec);
        for (auto music : vec)
        {
            musicArray.Append(winrt::Windows::Data::Json::JsonValue::CreateStringValue(music));
        }
        m_config.Insert(L"musics", musicArray);
        winrt::Windows::Storage::StorageFolder localFolder = co_await GetLocalFolder();
        winrt::Windows::Storage::StorageFile jsonFile = co_await localFolder.CreateFileAsync(L"music_config.json", winrt::Windows::Storage::CreationCollisionOption::ReplaceExisting);
        hstring json = m_config.Stringify();
        co_await winrt::Windows::Storage::FileIO::WriteTextAsync(jsonFile, json);
        OutputDebugStringW(L"\n[Debug]报存配置文件\n");
    }
    // 载入配置
    Windows::Foundation::IAsyncAction MusicAdapter::LoadConfig()
    {
        winrt::Windows::Storage::StorageFolder localFolder = co_await GetLocalFolder();
        winrt::Windows::Storage::IStorageItem item = co_await localFolder.TryGetItemAsync(L"music_config.json");
        if (item != nullptr)
        {
            winrt::Windows::Storage::StorageFile jsonFile = item.as<winrt::Windows::Storage::StorageFile>();
            hstring json = co_await winrt::Windows::Storage::FileIO::ReadTextAsync(jsonFile);
            m_config = winrt::Windows::Data::Json::JsonObject::Parse(json);
            if (m_config.HasKey(L"musics"))
            {
                for (auto music : m_config.GetNamedArray(L"musics"))
                {
                    AddMusic(music.GetString());
                }
            }

            OutputDebugStringW(L"\n[Debug]读取配置文件\n");
        }
        else {

            OutputDebugStringW(L"\n[Debug]配置文件不存在,跳过\n");
        }

    }
}
