# kcm-colorful
根据当前的壁纸改变KDE Plasma的颜色。

# Requirements
* Qt5
* KDE Frameworks 5
* Python3
* [color-thief-py](https://github.com/fengsp/color-thief-py)
* cmake
* extra-cmake-modules

# Build
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

# Screenshots
![a](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/a.png)
![b](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/b.png)

# Postscript
本项目还处于早期阶段，目前只实现了一个cli的程序用于根据输入的图片改变kde的颜色配置，用法如下：
```
kcmcolorfulhelper <picture-file>
```
接下来的目标主要是在主题色选择上，让得到主题色比较符合人类的审美。

基本功能成熟之后会做成KCM集成到kde的系统设置中（希望我不要中途弃坑233333）。
