#include "network/incoming_call_listener.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

IncomingCallListener::IncomingCallListener(const QString &serverUrl, QObject *parent)
    : QObject(parent), m_nam(new QNetworkAccessManager(this)), m_timer(new QTimer(this)), m_serverUrl(serverUrl) {
    connect(m_nam, &QNetworkAccessManager::finished, this, &IncomingCallListener::onReply);
}

void IncomingCallListener::start(int intervalMs) {
    connect(m_timer, &QTimer::timeout, this, &IncomingCallListener::poll);
    m_timer->start(intervalMs);
}

void IncomingCallListener::stop() { m_timer->stop(); }

void IncomingCallListener::poll() { m_nam->get(QNetworkRequest(QUrl(m_serverUrl + "/poll"))); }

void IncomingCallListener::onReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    if (doc.isNull() || !doc.isObject()) {
        return;
    }
    emit callReceived(doc["number"].toString(), doc["country"].toString());
}
