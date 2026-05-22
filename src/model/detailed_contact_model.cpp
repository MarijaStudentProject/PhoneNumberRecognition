#include "model/detailed_contact_model.hpp"
#include "model/contacts_model.hpp"
#include <QVariant>

DetailesContactModel::DetailesContactModel(ContactsController *controller, ContactsModel *contactsModel)
    : m_controller(controller), m_contactsModel(contactsModel) {}

QString DetailesContactModel::name() const { return m_editedName; }

QString DetailesContactModel::surname() const { return m_editedSurname; }

QString DetailesContactModel::email() const { return m_editedEmail; }

QString DetailesContactModel::city() const { return m_editedCity; }

QString DetailesContactModel::country() const { return m_editedCountry; }

QString DetailesContactModel::street() const { return m_editedStreet; }

QVariantList DetailesContactModel::phoneNumbers() const {
    QVariantList list;

    for (const auto &number : m_editedPhoneNumbers) {
        list.append(QVariant::fromValue(number));
    }

    return list;
}

bool DetailesContactModel::hasSelection() const { return m_selectedId != -1; }

void DetailesContactModel::setName(const QString &value) {
    if (m_editedName == value) {
        return;
    }

    m_editedName = value;

    emit nameChanged();
}

void DetailesContactModel::setSurname(const QString &value) {
    if (m_editedSurname == value) {
        return;
    }

    m_editedSurname = value;

    emit surnameChanged();
}

void DetailesContactModel::setEmail(const QString &value) {
    if (m_editedEmail == value) {
        return;
    }

    m_editedEmail = value;

    emit emailChanged();
}

void DetailesContactModel::setCity(const QString &value) {
    if (m_editedCity == value) {
        return;
    }

    m_editedCity = value;

    emit cityChanged();
}
void DetailesContactModel::setCountry(const QString &value) {
    if (m_editedCountry == value) {
        return;
    }

    m_editedCountry = value;

    emit countryChanged();
}
void DetailesContactModel::setStreet(const QString &value) {
    if (m_editedStreet == value) {
        return;
    }

    m_editedStreet = value;

    emit streetChanged();
}
void DetailesContactModel::showDetails(int id) {
    m_selectedId = id;

    auto *c = m_controller->findById(id);
    if (c == nullptr) {
        return;
    }

    m_editedName = QString::fromStdString(c->getName());
    m_editedSurname = QString::fromStdString(c->getSurname());
    m_editedEmail = QString::fromStdString(c->getEmail());
    m_editedCity = QString::fromStdString(c->getAddress().getCity());
    m_editedCountry = QString::fromStdString(c->getAddress().getCountry());
    m_editedStreet = QString::fromStdString(c->getAddress().getStreet());

    m_editedPhoneNumbers.clear();
    for (const auto &p : c->getPhoneNumbers()) {
        m_editedPhoneNumbers.append(QString::fromStdString(p.getRawValue()));
    }

    emit nameChanged();
    emit surnameChanged();
    emit emailChanged();
    emit cityChanged();
    emit countryChanged();
    emit streetChanged();
    emit phoneNumbersChanged();

    emit hasSelectionChanged();
}

void DetailesContactModel::prepareNew() {
    m_selectedId = -1;
    m_isNew = true;
    m_editedName.clear();
    m_editedSurname.clear();
    m_editedEmail.clear();
    m_editedCity.clear();
    m_editedCountry.clear();
    m_editedStreet.clear();
    m_editedPhoneNumbers.clear();
    m_editedPhoneNumbers.append("");
    emit nameChanged();
    emit surnameChanged();
    emit emailChanged();
    emit phoneNumbersChanged();
    emit hasSelectionChanged();
}

void DetailesContactModel::save() {
    std::vector<std::string> phoneNumbers;
    for (const auto &p : m_editedPhoneNumbers) {
        if (!p.isEmpty()) {
            phoneNumbers.push_back(p.toStdString());
        }
    }

    if (m_isNew) {
        m_isNew = false;
        m_selectedId = m_controller->addContact(
            m_editedName.toStdString(), m_editedSurname.toStdString(), phoneNumbers, m_editedEmail.toStdString(),
            Address(m_editedStreet.toStdString(), "", m_editedCity.toStdString(), m_editedCountry.toStdString()));
        m_contactsModel->refresh();
        return;
    }

    if (m_selectedId == -1) {
        return;
    }

    auto *c = m_controller->findById(m_selectedId);

    if (c == nullptr) {
        return;
    }

    m_controller->updateContact(m_selectedId, m_editedName.toStdString(), m_editedSurname.toStdString(), phoneNumbers,
                                m_editedEmail.toStdString(),
                                Address(m_editedStreet.toStdString(), c->getAddress().getPostalCode(),
                                        m_editedCity.toStdString(), m_editedCountry.toStdString()));

    m_contactsModel->refresh();
}

void DetailesContactModel::setPhoneNumber(int index, const QString &number) {
    if (index < 0 || index >= m_editedPhoneNumbers.size()) {
        return;
    }

    m_editedPhoneNumbers[index] = number;

    emit phoneNumbersChanged();
}

void DetailesContactModel::clear() {
    m_selectedId = -1;

    setName("");
    setSurname("");
    m_editedPhoneNumbers.clear();

    setEmail("");
    setCity("");
    setCountry("");
    setStreet("");

    emit hasSelectionChanged();
}