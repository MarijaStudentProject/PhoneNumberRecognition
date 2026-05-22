#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QString>

#include "controller/contacts_controller.hpp"
#include "detailed_contact_model.hpp"

class ContactsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged)

  public:
    enum Roles { IdRole = Qt::UserRole + 1, NameRole, SurnameRole, PhoneRole };

    explicit ContactsModel(ContactsController *controller, DetailesContactModel *detailedContactModel,
                           QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    QString searchText() const;

    Q_INVOKABLE void setSearchText(const QString &text);

    Q_INVOKABLE int contactIdAt(int row) const;

    Q_INVOKABLE void setContactToSearchText(int row);

    Q_INVOKABLE void import(const QString &text);

    Q_INVOKABLE void makeCall();

    Q_INVOKABLE void call(const QString &number, const QString &country);

    Q_INVOKABLE void select(int row);

    void refresh();
  signals:
    void searchTextChanged();

  private:
    ContactsController *m_controller;
    DetailesContactModel *m_detailedContactModel;
    QString m_editedSearchText;
    std::vector<int> m_visibleIds;
};

#endif // CONTACTS_MODEL_H
