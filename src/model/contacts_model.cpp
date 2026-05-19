#include "model/contacts_model.h"

ContactsModel::ContactsModel(ContactsController* controller)
    : m_controller(controller) {
    refresh();
}

int ContactsModel::rowCount(const QModelIndex &) const{
    return m_visibleIds.size();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return {};
    }

    const auto id = m_visibleIds[index.row()];

    const auto* c = m_controller->findById(id);


    if(!c){
        return {};
    }

    switch(role){
        case IdRole:
            return id;
        case NameRole:
            return QString::fromStdString(c->getName());
        case SurnameRole:
            return QString::fromStdString(c->getSurname());
        case PhoneRole:
            return QString::fromStdString(c->getPrimaryPhoneNumber().getRawValue());
    }

    return {};

}

QHash<int, QByteArray> ContactsModel::roleNames() const{
    return {
        {IdRole, "contactId"},
        {NameRole, "name"},
        {SurnameRole, "surname"},
        {PhoneRole, "phone"}
    };
}

int ContactsModel::contactIdAt(int row) const{
    if(row < 0 || row >= m_visibleIds.size()){
        return -1;
    }
    return m_visibleIds[row];
}

void ContactsModel::setSearchText(const QString &text){
    beginResetModel();

    m_visibleIds = m_controller->searchByName(text.toStdString());

    endResetModel();
}

void ContactsModel::import(const QString &text){
    m_controller->importContacts(text.toStdString());
    refresh();
}

void ContactsModel::refresh(){
    beginResetModel();

    m_visibleIds.clear();

    for(int i = 0; i < m_controller->contacts().size(); i++){
        m_visibleIds.push_back(i);
    }
    endResetModel();
}
