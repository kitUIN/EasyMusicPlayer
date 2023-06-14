#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include <shobjidl_core.h>
#include <corewindow.h>
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include "API/MergeSort.hpp"
using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::EasyMusicPlayer::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
        m_timer.Interval(std::chrono::seconds{ 1 });
        auto registrationtoken = m_timer.Tick({ this, &MainPage::OnTick });
        m_timer.Start();
    }
    void MainPage::ImportFile(Windows::Storage::StorageFile& file)
    {
        if (file.FileType() == L".mp3") {
            m_adapter.AddMusic(file.Path());
        }
    }
    Windows::Foundation::IAsyncAction MainPage::ImportFolder(Windows::Storage::StorageFolder& folder)
    {
        auto files = co_await folder.GetFilesAsync();
        auto folders = co_await folder.GetFoldersAsync();
        for (auto file : files)
        {
            ImportFile(file);
        }
        for (auto f : folders)
        {
            co_await ImportFolder(f);
        }
    }

    // 刷新音乐列表
    Windows::Foundation::IAsyncAction MainPage::ReloadMusicList()
    {
        m_musics.Clear();
        for (auto music : co_await m_adapter.GetAll())
        {
            m_musics.Append(music);
        }

    }
    bool AscendCompare(std::pair<winrt::hstring, int> const& lhs, std::pair<winrt::hstring, int> const& rhs)
    {
        return lhs.first > rhs.first;
    };
    bool ReverseCompare(std::pair<winrt::hstring, int> const& lhs, std::pair<winrt::hstring, int> const& rhs)
    {
        return lhs.first < rhs.first;
    };
    // 排序 up:是否升序
    void MainPage::SortSelect(bool up)
    {
        // first:名称 second:索引
        std::vector<std::pair<winrt::hstring, int>> lib;
        for (uint32_t i = 0; i < m_musics.Size(); ++i)
        {
            lib.push_back(std::pair(m_musics.GetAt(i).Name(), i));
        }
        if (up)
        {
            //升序
            Sort(lib, ReverseCompare);
            ZA().Glyph(L"");
            AZ().Glyph(L"\uE8FB");
        }
        else {
            // 降序
            Sort(lib, AscendCompare);
            AZ().Glyph(L"");
            ZA().Glyph(L"\uE8FB");
        }

        Windows::Foundation::Collections::IVector<EasyMusicPlayer::Music> coll{ winrt::single_threaded_vector<EasyMusicPlayer::Music>()};
        for (auto v : lib) {
            coll.Append(m_musics.GetAt(v.second));
        }
        m_musics.Clear();
        for (auto c : coll) {
            m_musics.Append(c);
        }
    }
    // 刷新音乐播放列表
    Windows::Foundation::IAsyncAction MainPage::ReloadPlayMusicList()
    {
        m_playMusics.Clear();
        m_playList.AutoRepeatEnabled(true);
        m_playList.Items().Clear();
        if (PlayModeIcon().Symbol() == Windows::UI::Xaml::Controls::Symbol::RepeatAll) {
            //列表循环
            for (auto music : m_musics)
            {
                m_playMusics.Append(music);
                m_playList.Items().Append(Windows::Media::Playback::MediaPlaybackItem(winrt::Windows::Media::Core::MediaSource::CreateFromUri(Windows::Foundation::Uri(music.Path()))));
            }
        }
        else if (PlayModeIcon().Symbol() == Windows::UI::Xaml::Controls::Symbol::RepeatOne)
        {
            //单曲循环
            for (uint32_t k = 0; k < m_musics.Size(); k++) {
                if (MusicName().Text() == m_musics.GetAt(k).Name()) {
                    auto select = m_musics.GetAt(k);
                    m_playMusics.Append(select);
                    m_playList.Items().Append(Windows::Media::Playback::MediaPlaybackItem(winrt::Windows::Media::Core::MediaSource::CreateFromUri(Windows::Foundation::Uri(select.Path()))));
                    break;
                }
            }
        }
        else {
            //随机播放
            for (auto music : co_await m_adapter.GetRandom())
            {
                m_playMusics.Append(music);
                m_playList.Items().Append(Windows::Media::Playback::MediaPlaybackItem(winrt::Windows::Media::Core::MediaSource::CreateFromUri(Windows::Foundation::Uri(music.Path()))));
            }
        }
        bool flag = true;
        for (uint32_t k = 0; k < m_playMusics.Size(); k++) {
            if (MusicName().Text() == m_playMusics.GetAt(k).Name()) {
                OutputDebugStringW((L"\n[Debug]播放列表起始:" + to_hstring(k) + L"\n").c_str());
                m_playList.StartingItem(m_playList.Items().GetAt(k));
                flag = false;
                break;
            }
        }
        if (flag && m_playList.Items().Size() > 0) {
            m_playList.StartingItem(m_playList.Items().GetAt(0));
            MusicName().Text(m_playMusics.GetAt(0).Name());
        }
        MusicPlayer().Source(m_playList);

    }
    int MainPage::ReloadPlayMusicSelect()
    {
        uint32_t j;
        for (j = 0; j < MusicListView().Items().Size(); j++) {
            if (MusicListView().Items().GetAt(j).as<EasyMusicPlayer::Music>().Name() == MusicName().Text()) {
                MusicListView().SelectedIndex(j);
                break;
            }
        }
        for (j = 0; j < FlyoutMusicListView().Items().Size(); j++) {
            if (FlyoutMusicListView().Items().GetAt(j).as<EasyMusicPlayer::Music>().Name() == MusicName().Text()) {
                FlyoutMusicListView().SelectedIndex(j);
                return static_cast<int>(j);
            }
        }
        return -1;
    }
    // 获取窗体句柄
    HWND MainPage::GetHwnd() {
        winrt::com_ptr<ICoreWindowInterop> interop {};
        winrt::check_hresult(winrt::get_unknown(Windows::UI::Core::CoreWindow::GetForCurrentThread())->QueryInterface(interop.put()));
        HWND hwnd{};
        winrt::check_hresult(interop->get_WindowHandle(&hwnd));
        return hwnd;
    }
    // int -> string hh:ss
    winrt::hstring MainPage::TimeFormat(int sec) {
        std::stringstream mm;
        mm << std::setw(2) << std::setfill('0') << sec / 60 << ":" << std::setw(2) << std::setfill('0') << sec % 60;
        return winrt::to_hstring(mm.str());
    }
    //每秒响应
    void MainPage::OnTick(Windows::Foundation::IInspectable const& /* sender */, Windows::Foundation::IInspectable const& /* e */)
    {
        Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [this]()
            {
                auto position = MusicPlayer().MediaPlayer().PlaybackSession().Position().count();
                int seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::duration(position)).count());
                MinTime().Text(TimeFormat(seconds));
                TimeBar().Value(seconds);
                int max_seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::duration(MusicPlayer().MediaPlayer().PlaybackSession().NaturalDuration().count())).count());
                if (max_seconds != 0) {
                    TimeBar().Maximum(max_seconds);
                }
                MaxTime().Text(TimeFormat(max_seconds));
            });
    }
}


/**
 * @brief 选择文件
 * @param
 * @param
 * @return
*/
Windows::Foundation::IAsyncAction winrt::EasyMusicPlayer::implementation::MainPage::ImportFile_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    Windows::Storage::Pickers::FileOpenPicker picker;
    picker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::MusicLibrary);
    picker.FileTypeFilter().Append(L".mp3");
    picker.ViewMode(Windows::Storage::Pickers::PickerViewMode::List);
    picker.as<IInitializeWithWindow>()->Initialize(GetHwnd());

    auto files = co_await picker.PickMultipleFilesAsync();
    if (files != nullptr)
    {
        for (auto file : files) {
            ImportFile(file);
        }
    }
    co_await ReloadMusicList();
    co_await m_adapter.SaveConfig();
    co_await ReloadPlayMusicList();
    SortSelect(true);
}
/**
 * @brief 选择文件夹
 * @param
 * @param
 * @return
*/
Windows::Foundation::IAsyncAction winrt::EasyMusicPlayer::implementation::MainPage::ImportFolder_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    Windows::Storage::Pickers::FolderPicker picker;
    picker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::MusicLibrary);
    picker.FileTypeFilter().Append(L"*");
    picker.ViewMode(Windows::Storage::Pickers::PickerViewMode::List);
    picker.as<IInitializeWithWindow>()->Initialize(GetHwnd());

    Windows::Storage::StorageFolder folder = co_await picker.PickSingleFolderAsync();
    if (folder != nullptr)
    {
        co_await ImportFolder(folder);
    }
    co_await ReloadMusicList();
    co_await m_adapter.SaveConfig();
    co_await ReloadPlayMusicList();
    SortSelect(true);
}

void winrt::EasyMusicPlayer::implementation::MainPage::Play()
{
    MusicPlayer().MediaPlayer().Play();
    PlayModeButtonIcon().Glyph(L"\uE769");
    TimeBar().ShowPaused(false);
    PlayingModeToolTip().Content(winrt::box_value(L"暂停"));
}
void winrt::EasyMusicPlayer::implementation::MainPage::Pause()
{
    MusicPlayer().MediaPlayer().Pause();
    PlayModeButtonIcon().Glyph(L"\uE768");
    TimeBar().ShowPaused(true);
    PlayingModeToolTip().Content(winrt::box_value(L"开始播放"));
}
/**
 * @brief 播放/停止 音乐
 * @param
 * @param
 * @return
*/
void winrt::EasyMusicPlayer::implementation::MainPage::PlayModeButton_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    if (PlayModeButtonIcon().Glyph() == L"\uE768")
    {
        Play();
    }
    else
    {
        Pause();
    }
}

//切换音乐循环模式
Windows::Foundation::IAsyncAction winrt::EasyMusicPlayer::implementation::MainPage::PlayMode_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    if (PlayModeIcon().Symbol() == Windows::UI::Xaml::Controls::Symbol::RepeatAll) {
        //单曲循环
        PlayModeIcon().Symbol(Windows::UI::Xaml::Controls::Symbol::RepeatOne);
        PlayMode(L"单曲循环");
    }
    else if (PlayModeIcon().Symbol() == Windows::UI::Xaml::Controls::Symbol::RepeatOne)
    {
        //随机播放
        PlayModeIcon().Symbol(Windows::UI::Xaml::Controls::Symbol::Shuffle);
        PlayMode(L"随机播放");
    }
    else {
        //列表循环
        PlayModeIcon().Symbol(Windows::UI::Xaml::Controls::Symbol::RepeatAll);
        PlayMode(L"列表循环");
    }
    co_await ReloadPlayMusicList();
    OutputDebugStringW((L"\n[Debug]选择播放模式:" + PlayMode() + L"\n").c_str());
    Play();
}
//在列表里点击
void winrt::EasyMusicPlayer::implementation::MainPage::MusicListView_ItemClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs const& e)
{

    auto music = e.ClickedItem().as<EasyMusicPlayer::Music>();
    auto list = sender.as<Windows::UI::Xaml::Controls::ListView>();
    MusicName().Text(music.Name());
    int i = ReloadPlayMusicSelect();
    if (i != -1) {
        m_playList.MoveTo(i);
        OutputDebugStringW((L"\n[Debug]播放:" + to_hstring(i) + L"\n").c_str());
    }
    Play();
}



// 删除
Windows::Foundation::IAsyncAction winrt::EasyMusicPlayer::implementation::MainPage::Remove_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    auto music = sender.as<winrt::Windows::UI::Xaml::Controls::HyperlinkButton>().Tag().as<EasyMusicPlayer::Music>();
    uint32_t index = 0;
    if (m_musics.IndexOf(music, index)) {
        m_musics.RemoveAt(index);
        m_playMusics.RemoveAt(index);
        m_adapter.RemoveMusic(music.Path());
        co_await m_adapter.SaveConfig();
    }
}



// 初始化
Windows::Foundation::IAsyncAction winrt::EasyMusicPlayer::implementation::MainPage::MusicListView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

    m_playList.CurrentItemChanged([this](Windows::Media::Playback::MediaPlaybackList const&, Windows::Media::Playback::CurrentMediaPlaybackItemChangedEventArgs  const&) {
        Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [this]()
            {
                OutputDebugStringW(L"\n[Debug]CurrentItemChanged响应\n");
                ReloadPlayMusicSelect();

            });

        });
    MusicPlayer().MediaPlayer().MediaEnded([this](Windows::Media::Playback::MediaPlayer, winrt::Windows::Foundation::IInspectable) {
        Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [this]()
            {
                OutputDebugStringW(L"\n[Debug]MediaEnded响应\n");
                uint32_t i;
                if (m_playMusics.IndexOf(MusicName().Tag().as<EasyMusicPlayer::Music>(), i)) {
                    auto music = m_playMusics.GetAt((i + 1) % m_playMusics.Size());
                    MusicName().Tag(box_value(music));
                    FlyoutMusicListView().SelectedItem(music);
                    MusicListView().SelectedItem(music);
                    MusicName().Text(music.Name());
                }
            });
        });
    co_await m_adapter.LoadConfig();
    co_await ReloadMusicList();
    SortSelect(true);
    co_await ReloadPlayMusicList();
}



// 点击播放列表
void winrt::EasyMusicPlayer::implementation::MainPage::MusicPlayListButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const&/* e*/)
{
    winrt::Windows::UI::Xaml::Controls::Primitives::FlyoutBase::ShowAttachedFlyout(sender.as<Windows::UI::Xaml::FrameworkElement>());
}

// 下一首
void winrt::EasyMusicPlayer::implementation::MainPage::RightButton_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    for (uint32_t i = 0; i < m_playMusics.Size(); i++) {
        if (MusicName().Text() == m_playMusics.GetAt(i).Name()) {
            auto music = m_playMusics.GetAt((i + 1) % m_playMusics.Size());
            MusicName().Text(music.Name());
            m_playList.MoveNext();
            Play();
            OutputDebugStringW((L"\n[Debug]➡下一首" + music.Name() + L"\n").c_str());
            break;
        }
    }
}

// 上一首
void winrt::EasyMusicPlayer::implementation::MainPage::LeftButton_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    for (uint32_t i = 0; i < m_playMusics.Size(); i++) {
        if (MusicName().Text() == m_playMusics.GetAt(i).Name()) {
            auto music = m_playMusics.GetAt((i - 1) % m_playMusics.Size());
            MusicName().Text(music.Name());
            m_playList.MovePrevious();
            Play();
            OutputDebugStringW((L"\n[Debug]⬅上一首" + music.Name() + L"\n").c_str());
            break;
        }
    }
}

// A-Z排序
void winrt::EasyMusicPlayer::implementation::MainPage::AZItem_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    SortSelect(true);
}

// Z-A排序
void winrt::EasyMusicPlayer::implementation::MainPage::ZAItem_Click(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    SortSelect(false);
}

winrt::event_token winrt::EasyMusicPlayer::implementation::MainPage::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
{
    return m_propertyChanged.add(handler);
}

void winrt::EasyMusicPlayer::implementation::MainPage::PropertyChanged(winrt::event_token const& token) noexcept
{
    m_propertyChanged.remove(token);
}
hstring winrt::EasyMusicPlayer::implementation::MainPage::PlayMode()
{
    return m_playMode;
}
void winrt::EasyMusicPlayer::implementation::MainPage::PlayMode(hstring const& value)
{
    m_playMode = value;
    m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"PlayMode" });
}


void winrt::EasyMusicPlayer::implementation::MainPage::FlyoutMusicListView_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
{
    sender.as<Windows::UI::Xaml::Controls::ListView>().ScrollIntoView(e.AddedItems().First());
}
