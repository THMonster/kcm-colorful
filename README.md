# KCM-Colorful
根据当前的壁纸改变KDE Plasma的颜色。

## Requirements
* Qt5
* KDE Frameworks 5
* cmake
* extra-cmake-modules

## Build
```
git clone --recursive https://github.com/IsoaSFlus/kcm-colorful.git
cd kcm-colorful
mkdir build
cd ./build
cmake ../
make
sudo make install
#Then set your plasma desktop theme to "Colorful" for best experience.
```

## INSTALLATION
### Archlinux (AUR)
```
yaourt -S kcm-colorful-git # or any other aur helper
```
Thanks for [@VOID001](https://github.com/VOID001)'s work.

## Screenshots
![a](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/a.png)
![b](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/b.png)

## TODO
- [x] 实现命令行helper
- [x] 将颜色提取算法移植至C++并剔除Python依赖
- [ ] 实现KCM集成至KDE设置
- [ ] 利用机器学习算法进一步优化主题色选择

## Postscript
本项目还处于早期阶段，目前只实现了一个cli的程序用于根据输入的图片改变kde的颜色配置，用法如下：
```
kcmcolorfulhelper <picture-file>
```
