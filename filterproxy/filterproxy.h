#ifndef FILTERPROXY_H
#define FILTERPROXY_H
/*
    This file is part of the OfiLabs X2 project.

    Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtNetwork>
//#include <QSsl>
//#include <QSslSocket>

class FilterProxy: public QObject
{
    Q_OBJECT

    struct RuleType {
        enum RuleType {
            block,
            transform
        };
    };
private:
    QStringList blockRules;
    QList<QPair<QString, QString>> transformRules;

    bool blockedURL(const QUrl &url) {
        QString s = url.toString(QUrl::RemoveScheme |
                                 QUrl::RemovePassword |
                                 QUrl::RemoveUserInfo);
        if (s.startsWith("//"))
            s.remove(0, 2);
        foreach (const QString &rule, urlRules)
            if (s.startsWith(rule))
                return true;
        return false;
    }

    QUrl transformURL(const QUrl &url)
    {
        QUrl ret;

//        foreach (const QString &rule, urlRules)
//            if (s.startsWith(rule))
//                return true;

        return ret;
    }

public:
    FilterProxy(QObject *parent = 0)
        : QObject(parent)
    {
        QTcpServer *proxyServer = new QTcpServer(this);
        proxyServer->listen(QHostAddress::Any, 8080);
        connect(proxyServer, SIGNAL(newConnection()), this, SLOT(manageQuery()));
        qDebug() << "Proxy server running at port" << proxyServer->serverPort();
    }

    QString cleanRule(const Qstring &r)
    {
        QString rule = r;

        rule = rule.simplified();
        if (rule.startsWith("http://"))
            rule.remove(0, 7);
        if (rule.startsWith("https://"))
            rule.remove(0, 8);

        return rule;
    }

    QStringList cleanRules(const QStringList &l)
    {
        QStringList ret;

        foreach (const QString &r, l)
            ret.append(cleanRule(r));

        return ret;
    }

    void addRule(RuleType type, const QString &r) {
        QList *ruleList;
        void *rule;

        switch (type)
        {
        case RuleType::block:
            ruleList = blockRules;
            rule = cleanRule(r);
            if (rule.isEmpty())
            {
                qDebug() << "Incorrect block rule skipped";
                return;
            }
            break;
        case RuleType::transform:
            ruleList = transformRules;
            QStringList ruleParts = cleanRules(r.split(" "));
            if (ruleParts.count() != 2 || ruleParts[0].isEmpty() || ruleParts[1].isEmpty())
            {
                qDebug() << "Incorrect transform rule skipped";
                return;
            }
            rule = new QPair<QString, QString>(ruleParts[0], ruleParst[1]);
            break;
        default:
            qDebug() << "Unhandled rule type";
        }
        if (!ruleList.contains(rule))
            ruleList += rule;
    }

    void addRules(RuleType type, const QStringList &rules) {
        foreach (QString rule, rules)
            addRule(rule);
    }

private slots:
    void manageQuery() {
        qDebug() << "new connection";
        QTcpServer *proxyServer = qobject_cast<QTcpServer*>(sender());
        QTcpSocket *socket = proxyServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(processQuery()));
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    }

    void processQuery() {
        qDebug() << "new query";
        QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
        QByteArray requestData = socket->readAll();

        int pos = requestData.indexOf("\r\n");
        QByteArray requestLine = requestData.left(pos);
        requestData.remove(0, pos + 2);

        QList<QByteArray> entries = requestLine.split(' ');
        QByteArray method = entries.value(0);
        QByteArray address = entries.value(1);
        QByteArray version = entries.value(2);

        //QUrl url = QUrl::fromEncoded(address);
        QUrl url = QUrl::fromUserInput(address);
        if (!url.isValid()) {
            qWarning() << "Invalid URL:" << url;
            socket->disconnectFromHost();
            return;
        }
        if (blockedURL(url)) {
            qWarning() << "Blocked URL:" << url;
            socket->disconnectFromHost();
            return;
        }

        url = transformURL(url);

        if (blockedURL(url)) {
            qWarning() << "Blocked URL:" << url;
            socket->disconnectFromHost();
            return;
        }
        qDebug() << "proxy URL:" << url;

        QString host = url.host();
        int port = (url.port() < 0) ? 80 : url.port();
        QByteArray req = url.toEncoded();

        requestLine = method + " " + req + " " + version + "\r\n";
        requestData.prepend(requestLine);

        QString key = host + ':' + QString::number(port);

        if (port == 443)
        {
            QSslSocket *proxySocket = socket->findChild<QSslSocket*>(key);
            if (proxySocket) {
                proxySocket->setObjectName(key);
                proxySocket->setProperty("url", url);
                proxySocket->setProperty("requestData", requestData);
                proxySocket->write(requestData);
            } else {
                proxySocket = new QSslSocket(socket);
                proxySocket->setObjectName(key);
                proxySocket->setProperty("url", url);
                proxySocket->setProperty("requestData", requestData);
                connect(proxySocket, SIGNAL(connected()), this, SLOT(sendRequest()));
                connect(proxySocket, SIGNAL(readyRead()), this, SLOT(transferData()));
                connect(proxySocket, SIGNAL(disconnected()), this, SLOT(closeConnection()));
                connect(proxySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(closeConnection()));
                proxySocket->connectToHostEncrypted(host, port);
                //proxySocket->connectToHost(host, port);
            }
        }
        else
        {
            QTcpSocket *proxySocket = socket->findChild<QTcpSocket*>(key);
            if (proxySocket) {
                proxySocket->setObjectName(key);
                proxySocket->setProperty("url", url);
                proxySocket->setProperty("requestData", requestData);
                proxySocket->write(requestData);
            } else {
                proxySocket = new QTcpSocket(socket);
                proxySocket->setObjectName(key);
                proxySocket->setProperty("url", url);
                proxySocket->setProperty("requestData", requestData);
                connect(proxySocket, SIGNAL(connected()), this, SLOT(sendRequest()));
                connect(proxySocket, SIGNAL(readyRead()), this, SLOT(transferData()));
                connect(proxySocket, SIGNAL(disconnected()), this, SLOT(closeConnection()));
                connect(proxySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(closeConnection()));
                proxySocket->connectToHost(host, port);
            }
        }
    }

    void sendRequest() {
        QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
        QByteArray requestData = proxySocket->property("requestData").toByteArray();
        proxySocket->write(requestData);
    }

    void transferData() {
        QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
        QTcpSocket *socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
        socket->write(proxySocket->readAll());
    }

    void closeConnection() {
        QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
        if (proxySocket) {
            QTcpSocket *socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
            if (socket)
                socket->disconnectFromHost();
            if (proxySocket->error() != QTcpSocket::RemoteHostClosedError)
                qWarning() << "Error for:" << proxySocket->property("url").toUrl()
                        << proxySocket->errorString();
            proxySocket->deleteLater();;
        }
    }
};

#endif // FILTERPROXY_H