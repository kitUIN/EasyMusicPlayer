#pragma once
#include "App.xaml.g.h"

namespace winrt::EasyMusicPlayer::implementation
{
    class App : public AppT<App>
    {
    public:
        App();
        void Close();
    };
}
