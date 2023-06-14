# EasyMusicPlayer 简易音乐播放器
本人数据结构课设,使用的是 `C++/WinRT` + `Xaml Islands`(使用[Mile.Xaml](https://github.com/ProjectMile/Mile.Xaml))
## 使用前提
您需要满足以下前置知识:
- C++ 17 语法, Win32相关操作
- [C++/WinRT UWP 相关开发知识](https://learn.microsoft.com/zh-cn/windows/uwp/cpp-and-winrt-apis/)
  - Xaml 语法,控件`MediaPlayerElement` `ListView`等,模板使用,事件等操作
  - 集合 
  - x:Bind绑定
  - 并发和异步操作
  - ...
- 简单的数据结构知识
- 善于思考的大脑
- 勤劳的手
## 简要说明
- `EasyMusicPlayer` > `API`目录下是使用的数据结构
  - 归并排序 (`MergeSort.hpp`)
  - 平方取中法 (`Random.hpp`)
  - 双进制hash (`Hash.hpp`,`HashMap.hpp`)
  - HashMap (`HashMap.hpp`,`Pair.hpp`,`Tree.hpp`,`Splay.hpp`)
- `MainPage.xaml`为主界面
- `Music`类为界面显示绑定实例
- `MusicAdapter`类用于HashMap与Music操作
- `music_config.json`是缓存文件,用来实现音乐列表持续化
- `MusicTestFolder`是用于测试的音乐文件
## 鸣谢(排名不分先后)
Co-Worker: (负责数据结构部分)  
Project:[Mile.Xaml](https://github.com/ProjectMile/Mile.Xaml).
