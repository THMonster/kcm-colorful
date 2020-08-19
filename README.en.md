# KCM-Colorful
Change your KDE Plasma's color based on your wallpaper.

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
Thanks for [@VOID001](https://github.com/VOID001)'s work.

### Archlinux ([ArchlinuxCN](https://wiki.archlinux.org/index.php/Unofficial_user_repositories#archlinuxcn) repo)
```
sudo pacman -S kcm-colorful-git
```
Thanks for [@MarvelousBlack](https://github.com/MarvelousBlack)'s maintenance.

## Screenshots
![a](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/a.png)
![b](https://raw.githubusercontent.com/IsoaSFlus/kcm-colorful/master/screenshots/b.png)

## TODO
- [x] Implement CLI helper
- [x] Port color extraction code to C++
- [x] Implement KCM
- [ ] Improve color selection by ML

## Postscript
Besides KCM, this project also provides a CLI program that could be used by your script:
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

**If you want to make your KDE look like the screenshots above, just follow these steps.**

First，open KDE's systemsettings->Desktop Behavior->Desktop Effects. Find "blur" and enable it(You can set the Blur strength and Noise strength to preferable value). 

Then，open KDE's systemsettings->Workspace Theme->Desktop Theme, set your desktop theme to “Colorful”.

Third，install [BreezeBlurred](https://github.com/alex47/BreezeBlurred)，this project provide a blurred version of Breeze window decoration. After installation, open KDE's systemsettings->Application Style->Window Decorations. Find the item named "BreezeBlur" and click "Configure BleezeBlur". Disable "Draw window backbround gradient" and set the Opacity(I'll recommend 60%, because it is the same as desktop theme's).

Finally，if your are curious about the dock on the Screenshots，just check out [Latte-Dock](https://github.com/psifidotos/Latte-Dock).
