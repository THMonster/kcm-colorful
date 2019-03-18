#include <KConfigWidgets/KCModule>
#include <QString>
#include <QtCore>
#include <QQuickWidget>
#include <KF5/KConfigWidgets/KColorScheme>
#include <KF5/KConfigCore/KConfigGroup>

class KCMColorful : public KCModule
{
    Q_OBJECT
public:
    KCMColorful(QWidget* parent, const QVariantList& args);
    ~KCMColorful();
    void readConfig();
    void saveConfig();
    void addColorGV1(QString color);




private:
    QQuickWidget *root_qml = nullptr;
    QString current_wallpaper;
    QStringList recent_colors;
    QStringList user_colors;
    bool extracted_flag = false;
    QStringList colors;
    QProcess *helper = nullptr;


public Q_SLOTS:
    void apply_color(const QString &c);
    void loadExtractedColors();
    void runHelper();
    void set_wp_view();
};
