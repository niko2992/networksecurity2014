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
#define PROXY_DEFAULT_PORT 8080

struct RuleType {
    enum type {
        block,
        transform,
        unknown
    };
};

class FilterProxy: public QObject
{
    Q_OBJECT

    QStringList blockRules;
    QList<QPair<QString, QString> > transformRules;

    enum Protocol {
        http,
        https
    };

public:
    FilterProxy(QObject *parent = 0);

    unsigned short getPort(void) const;

    QString cleanRule(const QString &r, bool deleteHttpPrefix = true);
    QStringList cleanRules(const QStringList &l, bool deleteHttpPrefix = true);

    void addRule(RuleType::type type, const QString &r);
    void addRules(RuleType::type type, const QStringList &rules);

private:
    bool applyBlockURLRules(const QUrl &url);
    bool applyTransformURLRules(QUrl &url);

private slots:
    void manageQuery();
    void processQuery();
    void sendRequest();
    void transferData();
    void closeConnection();
    void initializeSocket(QTcpSocket* socket, const QUrl& url, const QByteArray& requestData);
    void openConnection(QTcpSocket* client, const QUrl& url, const QByteArray& requestData);
    void openEncryptedConnection  (QTcpSocket* client, const QUrl& url, const QByteArray& requestData);
    void openUnencryptedConnection(QTcpSocket* client, const QUrl& url, const QByteArray& requestData);

    static QTcpSocket*    socketFromUrl(QTcpSocket* client, const QUrl& url);
    static QString        socketNameFromUrl(const QUrl& url);
    static Protocol       protocolFromUrl(const QUrl& url);
    static unsigned short portFromUrl(const QUrl& url);
};

#endif // FILTERPROXY_H
