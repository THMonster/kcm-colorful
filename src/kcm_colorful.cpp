#include "kcm_colorful.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QtCore>
#include <QtQuickWidgets/QQuickWidget>
#include <QQuickItem>
#include <QProcess>
#include <QString>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KAboutData>
#include <QDebug>
#include <QStandardPaths>
#include <KLocalizedString>
#include <KDeclarative/KDeclarative>



K_PLUGIN_FACTORY(KCMColorfulFactory, registerPlugin<KCMColorful>();)
//K_EXPORT_PLUGIN(KCMColorfulFactory("kcm_colorful"))



KCMColorful::KCMColorful(QWidget *parent, const QVariantList &args)
    : KCModule(parent)
{
    KAboutData* aboutData = new KAboutData(QStringLiteral("kcm-colorful"), i18n("Colorful"), QLatin1String(PROJECT_VERSION));
    aboutData->setShortDescription(i18n("Make your KDE Plasma colorful"));
    aboutData->setLicense(KAboutLicense::GPL_V2);
    aboutData->setHomepage(QStringLiteral("https://github.com/IsoaSFlus/kcm-colorful"));
    aboutData->addAuthor(QStringLiteral("IsoaSFlus"), i18n("Author"), QStringLiteral("me@isoasflus.com"));
    setAboutData(aboutData);
    QVBoxLayout *vb;
    vb = new QVBoxLayout(this);

    root_qml = new QQuickWidget;
    kdeclarative = new KDeclarative::KDeclarative();
    kdeclarative->setTranslationDomain(QStringLiteral("kcm_colorful"));
    kdeclarative->setDeclarativeEngine(root_qml->engine());
    kdeclarative->setupContext();
    root_qml->setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    root_qml->setResizeMode(QQuickWidget::SizeRootObjectToView);

    vb->addWidget(root_qml);
    setLayout(vb);

    set_wp_view();

    helper = new QProcess(this);
    QObject::connect(helper, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(loadExtractedColors()));

    QQuickItem *object = root_qml->rootObject();
    QQuickItem *wb_generate = object->findChild<QQuickItem *>(QStringLiteral("wb_generate"));
    QQuickItem *wb_refresh = object->findChild<QQuickItem *>(QStringLiteral("wb_refresh"));
    QQuickItem *cuw_row = object->findChild<QQuickItem *>(QStringLiteral("cuw_row"));
    QList<QQuickItem *> gv = object->findChildren<QQuickItem *>(QStringLiteral("grid_view"));
    QObject::connect(wb_generate, SIGNAL(clicked()), this, SLOT(runHelper()));
    QObject::connect(wb_refresh, SIGNAL(clicked()), this, SLOT(set_wp_view()));
    QObject::connect(cuw_row, SIGNAL(cuw_clicked(QString)), this, SLOT(apply_color(QString)));
    QObject::connect(gv[0], SIGNAL(cu_clicked(QString)), this, SLOT(apply_color(QString)));
    QObject::connect(gv[1], SIGNAL(cu_clicked(QString)), this, SLOT(apply_color(QString)));

    readConfig();
}

KCMColorful::~KCMColorful()
{
    saveConfig();
    delete root_qml;
    delete kdeclarative;
}

void KCMColorful::readConfig()
{
    KSharedConfigPtr cfg;
    cfg = KSharedConfig::openConfig();
    KConfigGroup kcg(cfg, "KCMColorful");
//    kcg.writeEntry(QStringLiteral("test"), QStringLiteral("test"));
    auto out = kcg.readEntry(QStringLiteral("RecentColors"), QStringLiteral(""));
    recent_colors = out.split(QStringLiteral(","), QString::SkipEmptyParts);
    out = kcg.readEntry(QStringLiteral("UserColors"), QStringLiteral(""));
    user_colors = out.split(QStringLiteral(","), QString::SkipEmptyParts);
    cfg->sync();
    cfg->markAsClean();

    for (auto c = recent_colors.rbegin(); c != recent_colors.rend(); ++c) {
        addColorGV1(*c);
    }
    QList<QQuickItem *> gv = root_qml->rootObject()->findChildren<QQuickItem *>(QStringLiteral("grid_view"));
    QVariant color_code;
    for (auto c : user_colors) {
        color_code = c;
        QMetaObject::invokeMethod(gv[1], "add_color",
                Q_ARG(QVariant, color_code));
    }
}

void KCMColorful::saveConfig()
{
    QList<QQuickItem *> gv = root_qml->rootObject()->findChildren<QQuickItem *>(QStringLiteral("grid_view"));
    QVariant returnedValue1;
    QVariant returnedValue2;
    QMetaObject::invokeMethod(gv[0], "get_colors",
            Q_RETURN_ARG(QVariant, returnedValue1));
    QMetaObject::invokeMethod(gv[1], "get_colors",
            Q_RETURN_ARG(QVariant, returnedValue2));

    KSharedConfigPtr cfg;
    cfg = KSharedConfig::openConfig();
//    qDebug() << cfg->groupList();
    KConfigGroup kcg(cfg, "KCMColorful");
    kcg.writeEntry(QStringLiteral("RecentColors"), returnedValue1);
    kcg.writeEntry(QStringLiteral("UserColors"), returnedValue2);
    cfg->sync();
    cfg->markAsClean();
}

void KCMColorful::addColorGV1(QString color)
{
//    QList<QQuickItem *> gv_model = root_qml->rootObject()->findChildren<QQuickItem *>(QStringLiteral("gv_model"));
    QList<QQuickItem *> gv = root_qml->rootObject()->findChildren<QQuickItem *>(QStringLiteral("grid_view"));
    QVariant color_code(color);
    QVariant returnedValue;
    QMetaObject::invokeMethod(gv[0], "prepend_color",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, color_code));
}

void KCMColorful::set_wp_view()
{
    QProcess p;
    p.start(QStringLiteral("bash"), QStringList() << QStringLiteral("-c") <<
                  QStringLiteral("qdbus org.kde.plasmashell /PlasmaShell org.kde.PlasmaShell.evaluateScript \"var d = desktops();\
                                 d[0].currentConfigGroup = Array(\\\"Wallpaper\\\", \\\"org.kde.image\\\", \\\"General\\\");\
                                 a = d[0].readConfig(\\\"Image\\\");a.a()\" | sed -n 's/.*\\(file:\\/\\/.*\\) is not a function.*/\\1/p'"));

    p.waitForFinished();
    QByteArray stdout = p.readAll();
    stdout.chop(1);
    current_wallpaper = QString::fromUtf8(stdout);
    qDebug() << current_wallpaper;
    extracted_flag = false;
    QQuickItem *wp = root_qml->rootObject()->findChild<QQuickItem *>(QStringLiteral("wp_view"));
    wp->setProperty("source", current_wallpaper);
}

void KCMColorful::runHelper()
{
    if (!extracted_flag) {
        helper->start(QStringLiteral("kcmcolorfulhelper"), QStringList() << QStringLiteral("--kcm") << QStringLiteral("-p") << current_wallpaper.mid(7));
//        QQuickItem *object = root_qml->rootObject();
        QQuickItem *wp = root_qml->rootObject()->findChild<QQuickItem *>(QStringLiteral("wp_view"));
        wp->setProperty("state", QStringLiteral("busy"));
    } else {
        loadExtractedColors();
    }
}

void KCMColorful::loadExtractedColors()
{
    if (!extracted_flag) {
        int i = 0;
        QByteArray stdout(helper->readAll());
        stdout.chop(1);
        colors = QString::fromUtf8(stdout).split(QStringLiteral(","), QString::SkipEmptyParts);
        QList<QQuickItem *> cuw = root_qml->rootObject()->findChildren<QQuickItem *>(QStringLiteral("cuw"));
        for (auto c : colors) {
            cuw[i++]->setProperty("inner_color", c);
        }
        extracted_flag = true;
    }
    QQuickItem *wp = root_qml->rootObject()->findChild<QQuickItem *>(QStringLiteral("wp_view"));
    wp->setProperty("state", QStringLiteral("generated"));
}

void KCMColorful::apply_color(const QString &c)
{
    qDebug() << c;
    QProcess::startDetached(QStringLiteral("kcmcolorfulhelper"), QStringList() << QStringLiteral("-c") << c);
    addColorGV1(c);
}
#include "kcm_colorful.moc"
