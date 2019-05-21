# goPage

![goPage](https://bytebucket.org/zandoye/static/raw/41e5ed66d725d8201b5751a32e3581aae094a392/goPage/goPage_20180727_010713.png)

引得市，開卷助理 \*NIX 版。

[下載最新的 0.2.2 版](https://bitbucket.org/zandoye/gopage/get/0.2.2.tar.gz)

以下介紹，使用的術語採用大陸的習慣。

# goPage 原理介紹

[开卷助理——古文字检索新思路](https://zhuanlan.zhihu.com/p/30943761)

# 安裝

請依據自己的系統或者喜好在以下方式中任選一種安裝。

## Linux 下添加發行版源安裝或者下載發行版安裝包

感謝 openSUSE 的 obs 服務！

* [Debian, Ubuntu 使用者](https://software.opensuse.org/package/gopage)
* [openSUSE, Fedora, CentOS, RHEL 使用者](https://software.opensuse.org/package/goPage)

## Mac OS X 版本

下載 [goPage.dmg](http://machinelife.org/osc/goPage.dmg)

安裝使用示例：

1. [安裝](https://link.zhihu.com/?target=https%3A//bitbucket.org/zandoye/static/raw/tip/goPage/goPage_install.gif)
2. [使用](https://link.zhihu.com/?target=https%3A//bitbucket.org/zandoye/static/raw/tip/goPage/goPage_jump.gif)

## 從源碼安裝，刪除

依賴 qt-sdk 

進入 goPage 的 src 目录後：


### 編譯
    qmake
    make

編譯設定於後文介紹。

### 安裝
    make install

### 刪除
    make uninstall


# 使用

## 系統托盤

goPage 現在生活在系統托盤裏了，你可以點擊小圖標隱藏或顯示主介面。

![goPageIcon](https://bytebucket.org/zandoye/gopage/raw/2313f662d0f811e17fb62c985e51766a597f0dba/src/res/goPage.png)

## 編輯

1. 導入字典。在對話框中選擇字典文件後, goPage 爲此字典文件生成 rmp 條目與文件. goPage 並不管理字典文件。因此原字典文件仍需保留。

2. 導入 rmp. 在對話框中選擇 rmp 文件後, goPage 讀取其中條目，並爲此 rmp 文件生成副本並進行管理。

3. 刪除所選。在主介面選取字典名後，可按 編輯->刪除所選 來刪掉相應 rmp 條目。也可在選取完畢後，直接按鍵盤 del 鍵刪除所選。

以上導入操作，刪除所選操作均可多選，批量進行操作。

在主介面中，可進行 rmp 偏移設定以及字典文件路徑設定。

## 遷移

1. 從...遷移。設定源目錄以及目標目錄。將所有源目錄下的字典文件遷移到目標目錄，同時更新 rmp 條目。若取消「移動字典」勾選，則僅僅更新 rmp 條目而不進行字典文件移動。

2. 遷移所有。將所有字典文件遷移到目標目錄，同時更新 rmp 條目。若取消「移動字典」勾選，則僅僅更新 rmp 條目而不進行字典文件移動。


# 更多信息

## 設定

goPage 根據 freedesktop 標準，通過 xdg 工具組自動判斷文件類型，尋找默認閱讀器。當前支持 Gnome 桌面下的默認閱讀器 evince, KDE 桌面下的默認閱讀器 okular, 以及 pdf 閱讀器 xpdf, djvu 閱讀器 djview. 用戶可根據需要自己在 ~/.config/goPage/reader.cfg 配置文件中添加自定義閱讀器。

條目格式如下，其中程序參數中可用 %1 代表字典文件 %2 代表頁碼。

    閱讀器, 程序參數


## 安裝時的配置

### 目錄設定
安裝時可選定安裝目標目錄，由 PREFIX 變量設定。更可獨立設定配置文件目錄，由 SYSCONFDIR 變量設定。

設定方式爲如下
    qmake PREFIX=目標目錄 SYSCONFDIR=系統配置文件目錄

若省略 SYSCONFDIR 設定，則其默認值爲 PREFIX/etc 目錄。若省略 PREFIX 設定，則其默認值爲 /usr/local 目錄。

### 編譯工具設定

由 LUPDATE 和 LRELEASE 參數可設定國際化和本地化 lupdate lrelease 工具。若編譯過程因未找到兩個工具而出錯時(例如 Fedora 發行版), 則可在執行 qmake 時加上 LUPDATE=lupdate LRELEASE=lrelease 參數通過編譯。

當爲發行版打包時，建議設定爲
    qmake PREFIX=/usr SYSCONFDIR=/etc

## 導入機制

### 導入字典
    導入字典時，若已有相應 rmp 條目，則更新此 rmp 條目的字典路徑信息。

### 導入 rmp
    導入 rmp 時，若已有相應 rmp 條目，則忽視導入項目。


## 批量使用現成的 rmp 文件

引得市的 Q&A 頁面，已爲許多字典提供現成的 rmp 文件下載。其 rmp 條目的字典目錄爲 D:\work, 字典文件未添加後綴名。因此不能直接在 \*NIX 系統下直接使用。利用上述導入機制。我們可以如下方式利用現成的 rmp 文件。

1. 下載所需 rmp 文件。

2. 編輯->導入 rmp，選取所需 rmp 後導入。

3. 編輯->導入字典，選取所需字典文件並導入, goPage 即正確更新相應 rmp 條目的字典目錄。

# 釋出版本

[0.2.2](https://bitbucket.org/zandoye/gopage/get/0.2.2.tar.gz)

[0.2.1](https://bitbucket.org/zandoye/gopage/get/0.2.1.tar.gz)

[0.2.0](https://bitbucket.org/zandoye/gopage/get/0.2.0.tar.gz)

[0.1.1](https://bitbucket.org/zandoye/gopage/get/0.1.1.tar.gz)

[0.1.0](https://bitbucket.org/zandoye/gopage/get/0.1.0.tar.gz)


# 歡迎回覆

歡迎在 issues 頁面報告問題，提交疑問。
