#include "model/contacts_model.hpp"
#include "contacts_model.hpp"

ContactsModel::ContactsModel(ContactsController* controller, DetailesContactModel* detailedContactModel,QObject* parent)
    : m_controller(controller), m_detailedContactModel(detailedContactModel), QAbstractListModel(parent) {
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

QString ContactsModel::searchText() const {
    return m_editedSearchText;
}

int ContactsModel::contactIdAt(int row) const{
    if(row < 0 || row >= m_visibleIds.size()){
        return -1;
    }
    return m_visibleIds[row];
}

void ContactsModel::setSearchText(const QString &text){
    m_editedSearchText=text;

    beginResetModel();
    m_visibleIds = m_controller->search(text.toStdString());
    endResetModel();

    emit searchTextChanged();
}

void ContactsModel::setContactToSearchText(int row){
    Contact* c = m_controller->findById(contactIdAt(row));
    if(!c){
        return;
    }

    setSearchText(QString::fromStdString(c->getPrimaryPhoneNumber().getRawValue()));
}

void ContactsModel::import(const QString &text){
    m_controller->importContacts(text.toStdString());
    refresh();
}

void ContactsModel::makeCall() { 
    QString number = searchText();
    if(number.isEmpty()){
         return;
    }
    auto opt = m_controller->match(number.toStdString());
    if(opt){
        Contact* c = m_controller->findById(opt.value());
        if(c){
            m_detailedContactModel->showDetails(opt.value());
        }
    }
    setSearchText("");  
 }
void ContactsModel::select(int row) { 
    int id = contactIdAt(row);
    m_detailedContactModel->showDetails(id);
}

 void ContactsModel::refresh() {
     beginResetModel();

     m_visibleIds.clear();

     for (int i = 0; i < m_controller->contacts().size(); i++) {
         m_visibleIds.push_back(i);
     }
     endResetModel();
 }
