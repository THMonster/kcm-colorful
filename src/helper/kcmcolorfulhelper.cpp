#include "kcmcolorfulhelper.h"
#include "colordata.h"
#include "mmcq.h"
#include <iostream>
#include <climits>
#include <cmath>
#include <QtCore>
#include <QTextStream>
#include <QSettings>
#include <QUuid>
#include <QDebug>
#include <QtGlobal>
#include <QDir>
#include <QThread>

KcmColorfulHelper::KcmColorfulHelper(QStringList args, QObject *parent) : QObject(parent)
{
    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
    mConfig = KSharedConfig::openConfig(QStringLiteral("kdeglobals"));

    if (args[4] == "true") {
        debug_flag = true;
    }
    if (args[5] == "true") {
        kcm_flag = true;
    }
    bool ok = false;
    paletteNum = args[2].toInt(&ok);
    if (ok == true) {
        if (paletteNum < 1 || paletteNum > 16) {
            paletteNum = 8;
        }
    } else {
        paletteNum = 8;
    }
    ok = false;
    selectNum = args[6].toInt(&ok);
    if (ok != true) {
        selectNum = 1;
    }
    if (args[0] == QString("kcmcolorful-NOPIC")) {
        if (args[1] != QString("kcmcolorful-NOCOLOR")) {
            c = new QColor(args[1]);
            run_type = run_type | 0x02u;
        } else {
            run_type = run_type & (~0x02u);
        }
    } else {
        wallpaperFilePath = args[0];
        mmcq = new MMCQ(wallpaperFilePath, debug_flag, kcm_flag);
        if (args[3] == "true") {
            setWallpaper(args[0]);
        }
        run_type = run_type | 0x02u;
    }

    ok = false;
    theme_opacity = args[7].toDouble(&ok);
    run_type = run_type | 0x01u;
    if (ok != true) {
        theme_opacity = 0.6;
        run_type = run_type & (~0x01u);
    }

//    genCSName();
//    getPrevCSName();
//    readDefaultCS();
}

KcmColorfulHelper::~KcmColorfulHelper()
{
    delete c;
    delete mmcq;
}

void KcmColorfulHelper::run()
{
    // for theme color
    if ((run_type & 0x02u) != 0) {
        if (mmcq != nullptr) {
            palette = mmcq->get_palette(paletteNum);
            palette_16 = mmcq->get_palette(16);
            calcColor();
        }
        int i = 1;
        if (kcm_flag == true) {
            QStringList colors;
            while (i <= 4) {
                colors << color_refine((pt_and_color.cend() - i).value()).name();
                if (pt_and_color.cend() - i == pt_and_color.cbegin()) {
                    break;
                }
                i++;
            }
            for (auto color : colors) {
                std::cout << color.toStdString() << ",";
            }
            std::cout << std::endl;
        } else {
            if (!c) {
                while (i <= 4) {
                    auto it = pt_and_color.cend() - i;
                    QColor color = it.value();
                    color = color_refine(color);
                    if (selectNum == i) {
                        c = new QColor(color);
                    }
                    qDebug().noquote() << QString("Candidate: %4, %1, %2, %3 \033[48;2;%1;%2;%3m     \033[0m").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()), QString::number(it.key()));
                    if (it == pt_and_color.cbegin()) {
                        c = new QColor(color);
                        break;
                    }
                    i++;
                }
                qDebug().noquote() << QString("Choose: %1, %2, %3 \033[48;2;%1;%2;%3m     \033[0m").arg(QString::number(c->red()), QString::number(c->green()), QString::number(c->blue()));
            }
            readTemplateCS();
    //        changeColorScheme(tConfig);
            changeColorSchemeB();
            save();
            mConfig->markAsClean();
            tConfig->markAsClean();
        }
    }
    // for theme opacity
    if ((run_type & 0x01u) != 0) {
        changeThemeOpacity();
    }
}

void KcmColorfulHelper::getPrevCSName()
{
    KSharedConfigPtr cfg = KSharedConfig::openConfig(QStringLiteral("kdeglobals"));
    KConfigGroup groupOut(cfg, "General");
    prevColorSchemeName = groupOut.readEntry("ColorScheme");
}

void KcmColorfulHelper::readTemplateCS()
{
    QString tName;
    if (isDarkTheme()) {
        tName = "ColorfulDark";
    } else {
        tName = "ColorfulLight";
    }

    QString schemeFile;

    schemeFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "color-schemes/" + tName + ".colors");
    QFile file(schemeFile);

    if (!file.exists()) {
        schemeFile = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                    + "/color-schemes/" + "BreezeIso.colors";
    }
    debug_flag ? qDebug().noquote() << schemeFile : qDebug();
    tConfig = KSharedConfig::openConfig(schemeFile);
}

void KcmColorfulHelper::changeColorScheme()
{
    KConfigGroup group(mConfig, "Colors:View");
//    qDebug() << group.readEntry("ForegroundNormal");

    group.writeEntry("ForegroundNormal", addJitter(group.readEntry("ForegroundNormal")));
    group.writeEntry("DecorationHover", *c);


//    KConfigGroup groupWMTheme(config, "WM");
    KConfigGroup groupWMOut(mConfig, "WM");

    QStringList colorItemListWM;
    colorItemListWM << "activeBackground"
                    << "activeForeground"
                    << "inactiveBackground"
                    << "inactiveForeground";
//                    << "activeBlend"
//                    << "inactiveBlend";

    QVector<QColor> defaultWMColors;
    defaultWMColors << *c
                    << QColor(239,240,241)
                    << *c
                    << QColor(189,195,199)
/*                    << QColor(255,255,255)
                    << QColor(75,71,67)*/;

    int i = 0;
    for (const QString &coloritem : colorItemListWM)
    {
            groupWMOut.writeEntry(coloritem, defaultWMColors.value(i));
            ++i;
    }
}

void KcmColorfulHelper::changeColorSchemeB()
{
    KConfigGroup group(mConfig, "Colors:View");
    group.writeEntry("ForegroundLink", *c);
    group.writeEntry("DecorationHover", *c);
    KConfigGroup groupWM(mConfig, "WM");
    groupWM.writeEntry("activeBackground", *c);
    groupWM.writeEntry("inactiveBackground", *c);
}

void KcmColorfulHelper::changeColorScheme(KSharedConfigPtr config)
{
//    // store colorscheme name in global settings
//    mConfig = KSharedConfig::openConfig(QStringLiteral("kdeglobals"));
//    KConfigGroup groupOut(mConfig, "General");
//    qDebug() << groupOut.readEntry("ColorScheme");
//    groupOut.writeEntry("ColorScheme", colorSchemeName);

    QStringList colorItemList;
    colorItemList << "BackgroundNormal"
                  << "BackgroundAlternate"
                  << "ForegroundNormal"
                  << "ForegroundInactive"
                  << "ForegroundActive"
                  << "ForegroundLink"
                  << "ForegroundVisited"
                  << "ForegroundNegative"
                  << "ForegroundNeutral"
                  << "ForegroundPositive"
                  << "DecorationFocus"
                  << "DecorationHover";

    QStringList colorSetGroupList;
    colorSetGroupList << "Colors:View"
                      << "Colors:Window"
                      << "Colors:Button"
                      << "Colors:Selection"
                      << "Colors:Tooltip"
                      << "Colors:Complementary";

    QList <KColorScheme> colorSchemes;

    colorSchemes.append(KColorScheme(QPalette::Active, KColorScheme::View, config));
    colorSchemes.append(KColorScheme(QPalette::Active, KColorScheme::Window, config));
    colorSchemes.append(KColorScheme(QPalette::Active, KColorScheme::Button, config));
    colorSchemes.append(KColorScheme(QPalette::Active, KColorScheme::Selection, config));
    colorSchemes.append(KColorScheme(QPalette::Active, KColorScheme::Tooltip, config));
    colorSchemes.append(KColorScheme(QPalette::Active, KColorScheme::Complementary, config));

    for (int i = 0; i < colorSchemes.length(); ++i)
    {
        KConfigGroup group(mConfig, colorSetGroupList.value(i));
        group.writeEntry("BackgroundNormal", addJitter(colorSchemes[i].background(KColorScheme::NormalBackground).color()));
        group.writeEntry("BackgroundAlternate", colorSchemes[i].background(KColorScheme::AlternateBackground).color());
        group.writeEntry("ForegroundNormal", addJitter(colorSchemes[i].foreground(KColorScheme::NormalText).color()));
        group.writeEntry("ForegroundInactive", colorSchemes[i].foreground(KColorScheme::InactiveText).color());
        group.writeEntry("ForegroundActive", colorSchemes[i].foreground(KColorScheme::ActiveText).color());
        group.writeEntry("ForegroundLink", colorSchemes[i].foreground(KColorScheme::LinkText).color());
        group.writeEntry("ForegroundVisited", colorSchemes[i].foreground(KColorScheme::VisitedText).color());
        group.writeEntry("ForegroundNegative", colorSchemes[i].foreground(KColorScheme::NegativeText).color());
        group.writeEntry("ForegroundNeutral", colorSchemes[i].foreground(KColorScheme::NeutralText).color());
        group.writeEntry("ForegroundPositive", colorSchemes[i].foreground(KColorScheme::PositiveText).color());
        group.writeEntry("DecorationFocus", colorSchemes[i].decoration(KColorScheme::FocusColor).color());
        if (i == 0) {
           group.writeEntry("DecorationHover", *c);
        } else {
            group.writeEntry("DecorationHover", colorSchemes[i].decoration(KColorScheme::HoverColor).color());
        }
    }

    KConfigGroup groupWMTheme(config, "WM");
    KConfigGroup groupWMOut(mConfig, "WM");

    QStringList colorItemListWM;
    colorItemListWM << "activeBackground"
                    << "activeForeground"
                    << "inactiveBackground"
                    << "inactiveForeground"
                    << "activeBlend"
                    << "inactiveBlend";

    QVector<QColor> defaultWMColors;
    defaultWMColors << *c
                    << colorSchemes[1].foreground(KColorScheme::NormalText).color()
                    << *c
                    << colorSchemes[1].foreground(KColorScheme::NormalText).color()
                    << QColor(255,255,255)
                    << QColor(75,71,67);

    int i = 0;
    for (const QString &coloritem : colorItemListWM)
    {
        groupWMOut.writeEntry(coloritem, defaultWMColors.value(i));
        ++i;
    }

    QStringList groupNameList;
    groupNameList << "ColorEffects:Inactive" << "ColorEffects:Disabled";

    QStringList effectList;
    effectList << "Enable"
               << "ChangeSelectionColor"
               << "IntensityEffect"
               << "IntensityAmount"
               << "ColorEffect"
               << "ColorAmount"
               << "Color"
               << "ContrastEffect"
               << "ContrastAmount";

    for (const QString &groupName : groupNameList)
    {

        KConfigGroup groupEffectOut(mConfig, groupName);
        KConfigGroup groupEffectTheme(config, groupName);

        for (const QString &effect : effectList) {
            groupEffectOut.writeEntry(effect, groupEffectTheme.readEntry(effect));
        }
    }
}

void KcmColorfulHelper::save()
{
    mConfig->sync();

//    KConfig cfg(QStringLiteral("kcmdisplayrc"), KConfig::NoGlobals);
//    KConfigGroup displayGroup(&cfg, "X11");

//    displayGroup.writeEntry("exportKDEColors", true);

//    cfg.sync();

//    QSettings* settings = new QSettings(QLatin1String("Trolltech"));
//    KSharedConfigPtr kglobalcfg = KSharedConfig::openConfig( "kdeglobals" );
//    KConfigGroup kglobals(kglobalcfg, "KDE");
//    QPalette newPal = KColorScheme::createApplicationPalette(kglobalcfg);
//    applyQtColors(kglobalcfg, *settings, newPal);
//    delete settings;
//    runRdb(KRdbExportQtColors | KRdbExportGtkTheme |  KRdbExportColors);


    QDBusMessage message = QDBusMessage::createSignal(QStringLiteral("/KGlobalSettings"),
                                                      QStringLiteral("org.kde.KGlobalSettings"),
                                                      QStringLiteral("notifyChange"));
    message.setArguments({
                             0, //previous KGlobalSettings::PaletteChanged. This is now private API in khintsettings
                             0  //unused in palette changed but needed for the DBus signature
                         });
    QDBusConnection::sessionBus().send(message);

    QDBusMessage message_2 = QDBusMessage::createSignal(QStringLiteral("/KWin"), QStringLiteral("org.kde.KWin"), QStringLiteral("reloadConfig"));
    QDBusConnection::sessionBus().send(message_2);

    QThread::sleep(1);
    QDBusConnection::sessionBus().send(message_2);
}

void KcmColorfulHelper::genCSName()
{
    colorSchemeName = "" + QUuid::createUuid().toString(QUuid::WithoutBraces).mid(0, 6);
}

void KcmColorfulHelper::saveCSFile()
{

    QString newpath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
            + "/color-schemes/";
    QProcess::execute(QString("bash -c \"rm '%1Colorful-*'\"").arg(newpath));
    QDir dir;
    dir.mkpath(newpath);
    newpath += colorSchemeName + ".colors";


    KConfig *config = mConfig->copyTo(newpath);
//    mConfig->markAsClean();
//    m_config->reparseConfiguration();
    KConfigGroup groupA(config, "General");
    groupA.writeEntry("Name", colorSchemeName);

    KConfigGroup group(config, "Colors:View");
    group.writeEntry("DecorationHover", *c);


//    KConfigGroup groupWMTheme(config, "WM");
    KConfigGroup groupWMOut(config, "WM");

    QStringList colorItemListWM;
    colorItemListWM << "activeBackground"
                    << "activeForeground"
                    << "inactiveBackground"
                    << "inactiveForeground";
//                    << "activeBlend"
//                    << "inactiveBlend";

    QVector<QColor> defaultWMColors;
    defaultWMColors << *c
                    << QColor(239,240,241)
                    << *c
                    << QColor(189,195,199)
/*                    << QColor(255,255,255)
                    << QColor(75,71,67)*/;

    int i = 0;
    for (const QString &coloritem : colorItemListWM)
    {
            groupWMOut.writeEntry(coloritem, defaultWMColors.value(i));
            ++i;
    }


    // sync it and delete pointer
    config->sync();
//    changeColorScheme(config);
    delete config;
    KSharedConfigPtr rConfig = KSharedConfig::openConfig(newpath);
    changeColorScheme(rConfig);

}

QColor KcmColorfulHelper::addJitter(QColor color)
{
    return QColor(color.red() + (qrand()%5 - 2), color.green() + (qrand()%5 - 2), color.blue() + (qrand()%5 - 2));
}

bool KcmColorfulHelper::isDarkTheme()
{
    if (c->red() < 66 && c->green() < 66 && c->blue() < 80) {
        return true;
    } else {
        return false;
    }
}

void KcmColorfulHelper::calcColor()
{
    double pt = 0;
    double pt_val = 0;
    double pt_sat = 0;
    double pt_hue = 0;
    double hue = 0;
    double sat = 0;
    double val = 0;
    double A = 0;
    int i = 0;
    int j = 0;

    double weight_of_order[8] = {1, 1, 1, 0.95, 0.9, 0.8, 0.7, 0.6};
    QList<QColor> colors;

//    QList<QColor>::iterator it;
    i = 0;
    for (auto it = palette.begin(); it != palette.end() && (it - palette.begin()) < 8; ++it) {
        colors << *it;
        i++;
    }
    for (auto it = palette_16.begin(); it != palette_16.end() && (it - palette_16.begin()) < 8; ++it) {
        colors << *it;
    }

    j = 0;
    for (auto color : colors) {
        pt = 0;
        pt_val = 0;
        pt_sat = 0;
        pt_hue = 0;
        val = color.value();
        sat = color.saturation();
        hue = color.hue();

        A = 220.0 + (0.137 * (255.0 - sat));
        if (val < A) {
            pt_val = 80.0 - ((A - val) / A * 80.0);
        } else {
            pt_val = 80.0 + ((val - A) / (255.0 - A) * 20.0);
        }

        if (sat < 70) {
            pt_sat = 100.0 - ((70.0 - sat) / 70.0 * 100.0);
        } else if (sat > 220) {
            pt_sat = 100.0 - ((sat - 220.0) / 35.0 * 100.0);
        } else {
            pt_sat = 100;
        }

        if (hue > 159 && hue < 196) {
            if (hue < 180) {
                pt_hue = 100.0 - (50.0 * (hue - 159.0) / 21.0);
            } else {
                pt_hue = 100.0 - (50.0 * (196.0 - hue) / 16.0);
            }
        } else {
            pt_hue = 100;
        }

        pt = (pt_val + pt_sat + pt_hue) / 3;
        pt = weight_of_order[j] * pt;

        pt_and_color.insert(pt, color);
        debug_flag ? qDebug().noquote() << QString("%1, %2, %3 \033[48;2;%1;%2;%3m     \033[0m %5, %6, %7, weight: %4").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()), QString::number(pt), QString::number(pt_val), QString::number(pt_sat), QString::number(pt_hue)) : qDebug();
        j++;
        if (j >= i) {
            debug_flag ? qDebug().noquote() << "================" : qDebug();
            j = 0;
            i = 114514;
        }
    }
}

void KcmColorfulHelper::setWallpaper(QString pic)
{
    QProcess::execute("qdbus", QStringList() << "org.kde.plasmashell" << "/PlasmaShell" << "org.kde.PlasmaShell.evaluateScript" << QString("var a = desktops();\
                      for(i = 0; i < a.length; i++){\
                        d = a[i];d.wallpaperPlugin = \"org.kde.image\";\
                        d.currentConfigGroup = Array(\"Wallpaper\", \"org.kde.image\", \"General\");\
                        d.writeConfig(\"Image\", \"file://%1\");\
                        d.writeConfig(\"FillMode\", 2);\
                        d.writeConfig(\"Color\", \"#000\");\
    }").arg(QFileInfo(pic).canonicalFilePath()));
}

QColor KcmColorfulHelper::color_refine(QColor color)
{
    double A = 220.0 + (0.137 * (255.0 - color.saturation()));
    debug_flag ? qDebug() << A : qDebug();
    if (color.value() < A) {
        if ((A - color.value()) <= 80) {
            color.setHsv(color.hue(), color.saturation(), static_cast<int>(A));
        } else if (((A - color.value()) > 80) && ((A - color.value()) <= 130)) {
//            qDebug() << (((0.5 + (0.5 * ((color.value() - (A - 130)) / 50))) * (A - color.value())) + color.value());
            color.setHsv(color.hue(), color.saturation(), static_cast<int>(((0.5 + (0.5 * ((color.value() - (A - 130)) / 50))) * (A - color.value())) + color.value()));
        } else {
            color.setHsv(color.hue(), color.saturation(), static_cast<int>(((A - color.value()) / 2) + color.value()));
        }
    }
    return color;
}

void KcmColorfulHelper::changeThemeOpacity()
{
    auto edit_svg = [](QString file_name, double opacity) {
        QFile f(file_name);
        if (!f.open(QIODevice::ReadWrite | QIODevice::Text))
            return;
        QString svg_text;
        svg_text = f.readAll();
        f.resize(0);
        QTextStream out(&f);
        svg_text.replace(QRegularExpression("([^-]+opacity[:\\s]+)[.0-9]+([^>]+?ColorScheme-ViewHover)"), "\\1" + QString::number(opacity) + "\\2");
        out << svg_text;
        f.flush();
        f.close();
    };

    QStringList svg_files;
    svg_files = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "plasma/desktoptheme/Colorful/metadata.desktop");
    qDebug() << svg_files;

    if (svg_files.length() != 0) {
        if (svg_files[0].contains(".local/share")) {
            QFileInfo info(svg_files[0]);
            edit_svg(info.canonicalPath() + "/dialogs/background.svg", theme_opacity);
            edit_svg(info.canonicalPath() + "/widgets/panel-background.svg", theme_opacity);
            edit_svg(info.canonicalPath() + "/widgets/tooltip.svg", theme_opacity);
        } else {
            QDir dir;
            QString base_dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//            qDebug() << base_dir;
            if (!QFile::exists(base_dir + "/.local/share/plasma")) {
                dir.mkpath(base_dir + "/.local/share/plasma");
            }
            if (!QFile::exists(base_dir + "/.local/share/plasma/desktoptheme")) {
                dir.mkpath(base_dir + "/.local/share/plasma/desktoptheme");
            }
            QFileInfo info(svg_files[0]);
            QProcess::execute("cp \"" + info.canonicalPath() + "\" \"" + base_dir + "/.local/share/plasma/desktoptheme/Colorful\" -rf");
            QString new_location = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "plasma/desktoptheme/Colorful/metadata.desktop");
            if (new_location.contains(".local/share")) {
                QFileInfo new_info(new_location);
                edit_svg(new_info.canonicalPath() + "/dialogs/background.svg", theme_opacity);
                edit_svg(new_info.canonicalPath() + "/widgets/panel-background.svg", theme_opacity);
                edit_svg(new_info.canonicalPath() + "/widgets/tooltip.svg", theme_opacity);
            } else {
                qDebug() << "Copy theme files failed!";
            }
        }
    }
}
