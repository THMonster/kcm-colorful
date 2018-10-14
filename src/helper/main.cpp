#include <QCoreApplication>
#include "kcmcolorfulhelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Helper for kcm-colorful.\nProject address: https://github.com/IsoaSFlus/kcm-colorful");
    parser.addHelpOption();
//    parser.addVersionOption();

    QCommandLineOption pictureOption(QStringList() << "p" << "picture", "Picture to extract color.", "file", "IsoaSFlus-NOPIC");
    parser.addOption(pictureOption);
    QCommandLineOption colorOption(QStringList() << "c" << "color", "Set color manually, eg: #1234ef.", "colorcode", "#EFEFEF");
    parser.addOption(colorOption);
    QCommandLineOption paletteOption(QStringList() << "n" << "palette-number", "Set the number of colors of palette. Valid number is between 1 to 16, default is 8.", "int", "8");
    parser.addOption(paletteOption);

    parser.process(a);

    KcmColorfulHelper kch(parser.value(pictureOption), parser.value(colorOption), parser.value(paletteOption));
    kch.run();

//    return a.exec();
    return 0;
}
