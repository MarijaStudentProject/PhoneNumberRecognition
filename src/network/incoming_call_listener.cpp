#include "network/incoming_call_listener.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <utility>

IncomingCallListener::IncomingCallListener(QString serverUrl, QObject *parent)
    : QObject(parent), m_nam(new QNetworkAccessManager(this)), m_timer(new QTimer(this)),
      m_serverUrl(std::move(serverUrl)) {
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
    const QJsonDocument Doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    if (Doc.isNull() || !Doc.isObject()) {
        return;
    }
    emit callReceived(Doc["number"].toString(), Doc["country"].toString());
}
