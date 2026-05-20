#include "controller/contacts_controller.hpp"
#include "model/contacts_model.hpp"
#include "model/detailed_contact_model.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QQmlContext>
#include <QFile>
#include <QDir>

#include <iostream>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    ContactsController controller;

    DetailesContactModel  detailesContactModel(&controller, nullptr);
    ContactsModel contactsModel(&controller, &detailesContactModel);
    detailesContactModel.setContactsModel(&contactsModel);

    //temporarly, because we dont have button for import in application
    //it will import from prepared file in resources

    QString path = QDir(QCoreApplication::applicationDirPath())
                       .filePath("resources/contacts_100.vcf");
    contactsModel.import(path);


    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "cppContactsModel",
        &contactsModel
    );

    engine.rootContext()->setContextProperty(
        "detailesContactModel",
        &detailesContactModel
    );

    engine.load(QUrl(QStringLiteral("qrc:/PhoneApp/ui/Main.qml")));

    return app.exec();
}
