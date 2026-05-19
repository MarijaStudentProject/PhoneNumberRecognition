#pragma once
#ifndef SELECTED_CONTACT_MODEL_H
#define SELECTED_CONTACT_MODEL_H


#include <QObject>
#include <QString>
#include "controller/contacts_controller.hpp"

class ContactsModel;

class DetailesContactModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString surname READ surname WRITE setSurname NOTIFY surnameChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(QString street READ street WRITE setStreet NOTIFY streetChanged)
    
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY hasSelectionChanged)

    Q_PROPERTY(QVariantList phoneNumbers READ phoneNumbers NOTIFY phoneNumbersChanged)
    
public:
    DetailesContactModel(
        ContactsController* controller,
        ContactsModel* contactsModel
    );

    QString name() const;
    QString surname() const;
    QString email() const;
    QString city() const;
    QString country() const;
    QString street() const;
    QVariantList phoneNumbers() const;
    bool hasSelection() const;

    void setName(const QString& value); //invokable?
    void setSurname(const QString& value);
    void setEmail(const QString& value);
    void setCity(const QString& value);
    void setCountry(const QString& value);
    void setStreet(const QString& value);

    void showDetails(int id);
    
    Q_INVOKABLE void clear();
    Q_INVOKABLE void save();

    Q_INVOKABLE void setPhoneNumber(int index, const QString& number);


signals:
    void nameChanged();
    void surnameChanged();
    void emailChanged();
    void cityChanged();
    void countryChanged();
    void streetChanged();
    void phoneNumbersChanged();
    void hasSelectionChanged();

private:
    void load();

private:
    ContactsController* m_controller;
    ContactsModel* m_contactsModel;

    int m_selectedId = -1;

    QString m_editedName;
    QString m_editedSurname;
    QString m_editedPhone;
    QString m_editedEmail;
    QString m_editedCity;
    QString m_editedCountry;
    QString m_editedStreet;
    QStringList m_editedPhoneNumbers;

};

#endif // SELECTED_CONTACT_MODEL_H