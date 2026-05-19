#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QString>

#include "controller/contacts_controller.h"

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        SurnameRole,
        PhoneRole
    };

    explicit ContactsModel(ContactsController* controller);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int contactIdAt(int row) const;

    Q_INVOKABLE void setSearchText(const QString& text);

    Q_INVOKABLE void import(const QString& text);

    void refresh();

  private:
    ContactsController* m_controller;

    std::vector<int> m_visibleIds;
};



#endif // CONTACTS_MODEL_H
