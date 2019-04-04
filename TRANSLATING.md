# `kcm-colorful` translate tutorial
## STEP TO TRANSLATE
### 1. TRANSLATE THE PO FILE (HIGH PRIORITY)
The PO file stored the string which is in settings interface, for example: "Generate colors based on wallpaper".

If your language has NEVER translated, you should generate the po file of your language:

```
[translate@tutorial kcm-colorful]$ bash extract-messages.sh
[translate@tutorial kcm-colorful]$ cd po
[translate@tutorial po]$ msginit -l (your_language) -i kcm_colorful.pot
```

and then, open that PO file with the PO editor you prefer to use.

If your language has translated but not latest or incompleted, just run "extract-messages.sh"
and that tool will merge the po file in `po` directory.

```
[translate@tutorial kcm-colorful]$ bash extract-messages.sh
```

and then, open the PO file of your language in `po` directory.

### 2. TRANSLATE THE DESKTOP FILE (MEDIUM PRIORITY)
The desktop file stored the name and description of kcm-colorful in KDE System Settings.

First, open `kcm_colorful.desktop` in `src` directory with the text editor you prefer to use.

And then, you might see the following text:
```desktop
[Desktop Entry]
...omitted

Name=Colorful
Name[zh_CN]=多彩
Comment=Make your KDE Plasma colorful
Comment[zh_CN]=让您的KDE Plasma丰富多彩
X-KDE-Keywords=Color, Theme
X-KDE-Keywords[zh_CN]=Color, Theme,色彩,颜色,主题
```

Just add the localized translation like `zh_CN` does, for example:
```
Name=Colorful
Name[zh_CN]=多彩
Name[LANG_CODE]=(Translation)
Comment=Make your KDE Plasma colorful
Comment[zh_CN]=让您的KDE Plasma丰富多彩
Comment[LANG_CODE]=(Translation)
X-KDE-Keywords=Color, Theme
X-KDE-Keywords[zh_CN]=Color, Theme,色彩,颜色,主题
X-KDE-Keywords[LANG_CODE]=(Translation)
```

> **NOTE**: The keywords use "," as the delimiter.

### 3. Push and Create a Pull Request
And then, you just need to wait for approve :)
