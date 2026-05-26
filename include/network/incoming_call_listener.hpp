#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QTimer>

class QNetworkReply;

class IncomingCallListener : public QObject {
    Q_OBJECT
  public:
    explicit IncomingCallListener(QString serverUrl, QObject *parent = nullptr);
    void start(int intervalMs = 500);
    void stop();
    Q_INVOKABLE void resumePolling();

  signals:
    void callReceived(const QString &number, const QString &country);

  private:
    QNetworkAccessManager *m_nam;
    QTimer *m_timer;
    QString m_serverUrl;
    int m_intervalMs = 500;

    void poll();
    void onReply(QNetworkReply *reply);
};
