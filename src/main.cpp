#include "controller/contacts_controller.hpp"
#include "model/contacts_model.hpp"
#include "model/detailed_contact_model.hpp"
#include "network/incoming_call_listener.hpp"

#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>

#include <QFontDatabase>
#include <iostream>
#include <qcoreapplication.h>

#include <QSettings>

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    if (QFile::exists(":/PhoneApp/ui/fonts/fa-solid-900.ttf")) {
        int id = QFontDatabase::addApplicationFont(":/PhoneApp/ui/fonts/fa-solid-900.ttf");
    } else {
        qDebug() << "Font file not found in resources";
    }

    QString appPath = QCoreApplication::applicationDirPath();
    QSettings settings(appPath + "/resources/app_config.ini", QSettings::IniFormat);
    QString metadataPath = settings.value("parser/metadata_path").toString();
    QString parsingRegion = settings.value("parser/region").toString();

    ContactsController controller(metadataPath.toStdString(), parsingRegion.toStdString());
    DetailesContactModel detailesContactModel(&controller, nullptr);
    ContactsModel contactsModel(&controller, &detailesContactModel);
    detailesContactModel.setContactsModel(&contactsModel);
    QString path = QDir(QCoreApplication::applicationDirPath()).filePath("resources/contacts_100.vcf");
    contactsModel.import(path);

    QSettings demoSettings(appPath + "/resources/demo_config.ini", QSettings::IniFormat);
    QString url = demoSettings.value("server/api_url").toString();

    IncomingCallListener callPoller(url);
    callPoller.start(2000);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("cppContactsModel", &contactsModel);

    engine.rootContext()->setContextProperty("detailesContactModel", &detailesContactModel);

    engine.rootContext()->setContextProperty("callPoller", &callPoller);

    engine.load(QUrl(QStringLiteral("qrc:/PhoneApp/ui/Main.qml")));

    return QGuiApplication::exec();
}
