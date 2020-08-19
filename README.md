# KCM-Colorful
[English Version](https://github.com/IsoaSFlus/kcm-colorful/blob/master/README.en.md)

根据当前的壁纸改变KDE Plasma的颜色。

## Requirements
* Qt5
* KDE Frameworks 5
* cmake
* extra-cmake-modules
* GNU gettext utilities

## Dependencies
> There are some dependencies you'll need to install.  

### Ubuntu

```bash
sudo apt update && \
sudo apt install -y cmake make extra-cmake-modules gettext \
libqt5x11extras5-dev qt5-default qtquickcontrols2-5-dev \
git g++ libkf5kio-dev libkf5kcmutils-dev libkf5declarative-dev
```

## Build
```
git clone --recursive https://github.com/IsoaSFlus/kcm-colorful.git --depth=1
cd kcm-colorful
mkdir build
cd ./build
cmake ../
make
sudo make install
#Then set your plasma desktop theme to "Colorful" for best experience.
```

## Installation
### Archlinux (AUR)
```
yaourt -S kcm-colorful-git # or any other aur helper
```
Thanks for [@VOID001](https://github.com/VOID001)'s maintenance.

### Archlinux ([ArchlinuxCN](https://wiki.archlinux.org/index.php/Unofficial_user_repositories#archlinuxcn) repo)
```
sudo pacman -S kcm-colorful-git
```
Thanks for [@MarvelousBlack](https://github.com/MarvelousBlack)'s maintenance.

## Screenshots
![a](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/a.png)
![b](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/b.png)

## TODO
- [x] 实现命令行helper
- [x] 将颜色提取算法移植至C++并剔除Python依赖
- [x] 实现KCM集成至KDE设置
- [ ] 利用机器学习算法进一步优化主题色选择

## Postscript
除了KCM，本项目还提供了一个cli的程序，可以供用户配合脚本使用：

```
Usage: kcmcolorfulhelper [options]
Helper for kcm-colorful.
Project address: https://github.com/IsoaSFlus/kcm-colorful

Options:
  -h, --help               Displays this help.
  -p, --picture <file>     Picture to extract color.
  -c, --color <colorcode>  Set color manually, eg: #1234ef.
  -s, --set-as-wallpaper   Set picture specified by "-p" as wallpaper.
  -d, --debug              Show debug info.
  -n, --number <int>       Select the Nth color in candidate list. Default is
                           1.
  -o, --opacity <float>    Set the opacity of theme, from 0 to 1(1 is opaque).
```

**如果你想实现像我截图中一样的效果，那么还需要一些额外的设置。**

首先，打开KDE的系统设置->桌面行为->桌面特效，找到“模糊”项并使能（可根据个人喜好调整该项设置中的模糊程度以及噪点厚度），依据个人喜好开启或关闭“背景对比度”项。

之后，打开KDE的系统设置->工作空间主题->桌面主题，设置主题为“Colorful”（当然，前提是你安装了本项目）。

第三，安装[BreezeBlurred](https://github.com/alex47/BreezeBlurred)，这个项目在原版Breeze窗口装饰主题的基础上提供透明效果。在安装完成后打开KDE的系统设置->应用程序风格->窗口装饰，找到该主题“微风”（注意，你会发现你其中有复数个“微风”因为原版也叫“微风”）并打开其配置菜单，关闭绘制窗口背景渐变并调整透明度（推荐60%，因为这与桌面主题一致），如下图。
![](https://i.loli.net/2018/09/04/5b8e0ce1a79a7.png)

最后，如果你对我截图左侧的dock感兴趣，请安装[Latte-Dock](https://github.com/psifidotos/Latte-Dock)。
