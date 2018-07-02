#ifndef KCMCOLORFULHELPER_H
#define KCMCOLORFULHELPER_H

#include <QCoreApplication>
#include <QObject>
#include <QProcess>
#include <KF5/KConfigWidgets/KColorScheme>
#include <KF5/KConfigCore/KConfigGroup>
#include <QDBusConnection>
#include <QDBusMessage>

class KcmColorfulHelper : public QObject
{
    Q_OBJECT
public:
    explicit KcmColorfulHelper(int argc, char *argv[], QObject *parent = nullptr);
    ~KcmColorfulHelper();
    void run();

private:
    QList<QColor> palette;
    QColor *c = nullptr;
    QString colorSchemeName;
    QString prevColorSchemeName;
    KSharedConfigPtr mConfig;
    KSharedConfigPtr tConfig;
    QString wallpaperFilePath;
    QProcess *colorExtractProc = nullptr;
    void getPrevCSName();
    void readTemplateCS();
    void changeColorScheme();
    void changeColorScheme(KSharedConfigPtr config);
    void save();
    void genCSName();
    void saveCSFile();
    QColor addJitter(QColor color); // due to a stupid kde's bug;
    bool isDarkTheme();
    void calcColor();

signals:

public slots:
    void dealStdOut();
};

#endif // KCMCOLORFULHELPER_H
