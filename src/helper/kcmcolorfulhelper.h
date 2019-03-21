#ifndef KCMCOLORFULHELPER_H
#define KCMCOLORFULHELPER_H

#include <QCoreApplication>
#include <QObject>
#include <QProcess>
#include <KF5/KConfigWidgets/KColorScheme>
#include <KF5/KConfigCore/KConfigGroup>
#include <QDBusConnection>
#include <QDBusMessage>

#include "mmcq.h"

class KcmColorfulHelper : public QObject
{
    Q_OBJECT
public:
    explicit KcmColorfulHelper(QStringList args, QObject *parent = nullptr);
    ~KcmColorfulHelper();
    void run();

private:
    QList<QColor> palette;
    QList<QColor> palette_16;
    MMCQ *mmcq = nullptr;
    QColor *c = nullptr;
    QMap<double, QColor> pt_and_color;
    QString colorSchemeName;
    QString prevColorSchemeName;
    KSharedConfigPtr mConfig;
    KSharedConfigPtr tConfig;
    QString wallpaperFilePath;
    int paletteNum = 8;
    int selectNum = 1;
    bool kcm_flag = false;
    bool debug_flag = false;
    void getPrevCSName();
    void readTemplateCS();
    void changeColorScheme();
    void changeColorSchemeB(); // change ColorScheme based on user's ColorScheme file
    void changeColorScheme(KSharedConfigPtr config);
    void save();
    void genCSName();
    void saveCSFile();
    QColor addJitter(QColor color); // due to a stupid bug of kde;
    bool isDarkTheme();
    void calcColor();
    void setWallpaper(QString pic);
    QColor color_refine(QColor color);

signals:

public slots:
//    void dealStdOut();
};

#endif // KCMCOLORFULHELPER_H
