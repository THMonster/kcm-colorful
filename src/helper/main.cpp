#include <QCoreApplication>
#include "kcmcolorfulhelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Helper for kcm-colorful.\nProject address: https://github.com/IsoaSFlus/kcm-colorful");
    parser.addHelpOption();
//    parser.addVersionOption();

    QCommandLineOption pictureOption(QStringList() << "p" << "picture", "Picture to extract color.", "file", "kcmcolorful-NOPIC");
    parser.addOption(pictureOption);
    QCommandLineOption colorOption(QStringList() << "c" << "color", "Set color manually, eg: #1234ef.", "colorcode", "kcmcolorful-NOCOLOR");
    parser.addOption(colorOption);
    QCommandLineOption paletteOption("palette-number", "Set the number of colors of palette in the first color extraction. Valid number is between 1 to 16, default is 8.", "int", "8");
    paletteOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser.addOption(paletteOption);
    QCommandLineOption setWPOption(QStringList() << "s" << "set-as-wallpaper", "Set picture specified by \"-p\" as wallpaper.");
    parser.addOption(setWPOption);
    QCommandLineOption debugOption(QStringList() << "d" << "debug", "Show debug info.");
    parser.addOption(debugOption);
    QCommandLineOption kcmOption("kcm");
    kcmOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser.addOption(kcmOption);
    QCommandLineOption colorNumOption(QStringList() << "n" << "number", "Select the Nth color in candidate list. Default is 1.", "int", "1");
    parser.addOption(colorNumOption);
    QCommandLineOption opacityOption(QStringList() << "o" << "opacity", "Set the opacity of theme, from 0 to 1(1 is opaque).", "float", "notset");
    parser.addOption(opacityOption);


    parser.process(a);

    if (QCoreApplication::arguments().size() <= 1) {
        qDebug().noquote() << "Error: You should at least specify one argument.";
        parser.showHelp();
    }

    QStringList args;
    args << parser.value(pictureOption);
    args << parser.value(colorOption);
    args << parser.value(paletteOption);
    args << (parser.isSet(setWPOption) ? "true" : " false");
    args << (parser.isSet(debugOption) ? "true" : " false");
    args << (parser.isSet(kcmOption) ? "true" : " false");
    args << parser.value(colorNumOption);
    args << parser.value(opacityOption);
    KcmColorfulHelper kch(args);
    kch.run();

//    return a.exec();
    return 0;
}
