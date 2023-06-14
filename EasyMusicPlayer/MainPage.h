#pragma once

#include "MainPage.g.h"
#include "Music.h"
#include "MusicAdapter.h"
namespace winrt::EasyMusicPlayer::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        HWND GetHwnd();
        winrt::hstring TimeFormat(int sec);
        Windows::Foundation::IAsyncAction ReloadMusicList();
        Windows::Foundation::IAsyncAction ReloadPlayMusicList();
        int ReloadPlayMusicSelect();
        void SortSelect(bool up);
        void ImportFile(Windows::Storage::StorageFile& file);
        Windows::Foundation::IAsyncAction ImportFolder(Windows::Storage::StorageFolder& folder);
        void OnTick(Windows::Foundation::IInspectable const&, Windows::Foundation::IInspectable const&);
        Windows::Foundation::IAsyncAction ImportFile_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        Windows::Foundation::IAsyncAction ImportFolder_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void PlayModeButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        Windows::Foundation::IAsyncAction PlayMode_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music> MusicList() { return m_musics; }
        winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music> PlayMusicList() { return m_playMusics; }
        void MusicListView_ItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs const& e);
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    private:
        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        Windows::UI::Xaml::DispatcherTimer m_timer;
        winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music> m_musics = winrt::single_threaded_observable_vector<EasyMusicPlayer::Music>();
        winrt::Windows::Foundation::Collections::IObservableVector<EasyMusicPlayer::Music> m_playMusics = winrt::single_threaded_observable_vector<EasyMusicPlayer::Music>();
        EasyMusicPlayer::MusicAdapter m_adapter;
        hstring m_playMode = L"列表循环";
        Windows::Media::Playback::MediaPlaybackList m_playList;
    public:
        Windows::Foundation::IAsyncAction Remove_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        hstring PlayMode();
        void Pause();
        void Play();
        void PlayMode(hstring const& value);
        Windows::Foundation::IAsyncAction MusicListView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void MusicPlayListButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/);
        void RightButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void LeftButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void AZItem_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void ZAItem_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
        void FlyoutMusicListView_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
    };
}

namespace winrt::EasyMusicPlayer::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
