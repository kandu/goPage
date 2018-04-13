# goPage

引得市，開卷助理 *NIX 版。

# 使用說明

[开卷助理——古文字检索新思路](https://zhuanlan.zhihu.com/p/30943761)

# 與 Windows 版本的不同處

1. goPage *NIX 根據 freedesktop 標準，通過 xdg 工具組自動尋找默認 pdf 閱讀器。當前支持 Gnome 桌面下的默認閱讀器 evince, KDE 桌面下的默認閱讀器 okular, 以及通用閱讀器 xpdf。用戶可根據需要自己在 pdfViewer.cfg 配置文件中添加自定義閱讀器。

2. rmp 文件內部設定文件名時，應遵照 \*INX 習慣，不省略後綴名(.pdf)

# 編譯及安裝

依賴 qt4-sdk 

進入 goPage 源碼目錄

    qmake
    make

即可在 src 目錄下得到 goPage 程序。

![演示](https://bytebucket.org/zandoye/static/raw/ff7891d5ce569b25042dc397e6fe9e80efe953dd/goPage/goPage.gif)