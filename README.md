# goPage

引得市，開卷助理 *NIX 版。

# 使用說明

[开卷助理——古文字检索新思路](https://zhuanlan.zhihu.com/p/30943761)

# 與 Windows 版本的不同處

1. goPage *NIX 根據 freedesktop 標準，通過 xdg 工具組自動尋找默認 pdf 閱讀器。當前支持 Gnome 桌面下的默認閱讀器 evince, KDE 桌面下的默認閱讀器 okular, 以及通用閱讀器 xpdf。用戶可根據需要自己在 pdfViewer.cfg 配置文件中添加自定義閱讀器。

2. rmp 文件內部設定文件名時，應遵照 \*INX 習慣，不省略後綴名(.pdf)

# 編譯及使用

依賴 qt4-sdk 

進入 goPage 源碼目錄

    qmake
    make

即可在 src 目錄下得到 goPage 程序。

編譯產生的 goPage程序可放置於任意位置使用，唯一需求文件為 pdfViewer.cfg 文件，需和 goPage 放置於同一目錄下。
其餘文件，如 pdf 辭典文件, rmp 重定向文件則可按需放置於 goPage 同一目錄下使用。

![演示](https://bytebucket.org/zandoye/static/raw/ff7891d5ce569b25042dc397e6fe9e80efe953dd/goPage/goPage.gif)