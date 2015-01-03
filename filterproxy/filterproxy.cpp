#include "filterproxy.h"

FilterProxy::FilterProxy(QObject *parent)
    : QObject(parent)
{
    QTcpServer *proxyServer = new QTcpServer(this);
    proxyServer->listen(QHostAddress::Any, getPort());
    connect(proxyServer, SIGNAL(newConnection()), this, SLOT(manageQuery()));
    qDebug() << "Proxy server running at port" << proxyServer->serverPort();
}

unsigned short FilterProxy::getPort(void) const
{
    QStringList arguments = QCoreApplication::arguments();

    for (int i = 0 ; i < arguments.length() ; ++i)
    {
      if (arguments[i] == "-p" || arguments[i] == "--port")
      {
        if (arguments.length() <= i + 1)
        {
          qDebug() << "/!\\ option --port was provided without a value.";
          break ;
        }
        return (arguments[i + 1].toShort());
      }
    }
    return (PROXY_DEFAULT_PORT);
}

bool FilterProxy::applyBlockURLRules(const QUrl &url) {
    QString s = url.toString(QUrl::RemoveScheme |
                             QUrl::RemovePassword |
                             QUrl::RemoveUserInfo);

    if (s.startsWith("//"))
        s.remove(0, 2);

    foreach (const QString &rule, blockRules)
    {
        if (s.startsWith(rule))
        {
            qDebug() << "Block rule blocked following URL: " << s;
            return true;
        }
    }
    return false;
}

bool FilterProxy::applyTransformURLRules(QUrl &url)
{
//        QString pattern = ".*\\.jpg";
//        QString replace = "http://web-tech.fr/wp-content/uploads/2012/04/trollface1.jpg";
//        QRegExp reg = QRegExp(pattern);
    bool changed = false;
    typedef QPair<QString, QString> Rule;

    foreach (const Rule &rule, transformRules)
    {
        QString save = url.toString();

        qDebug() << "debug##" << rule.first << "##" << rule.second;
        url = url.toString().replace(QRegExp(rule.first), rule.second);

        if (url.toString() != save)
        {
            qDebug() << "Transform rule transformed following URL: " << save << " to: " << url.toString();
            changed = true;
        }
    }
    if (changed)
        return true;
    else
        return false;
}

QString FilterProxy::cleanRule(const QString &r, bool deleteHttpPrefix)
{
    QString rule = r;

    rule = rule.simplified();

    if (deleteHttpPrefix)
    {
        if (rule.startsWith("http://"))
            rule.remove(0, 7);
        if (rule.startsWith("https://"))
            rule.remove(0, 8);
    }

    return rule;
}

QStringList FilterProxy::cleanRules(const QStringList &l, bool deleteHttpPrefix)
{
    QStringList ret;

    foreach (const QString &r, l)
        ret.append(cleanRule(r, deleteHttpPrefix));

    return ret;
}

void FilterProxy::addRule(RuleType::type type, const QString &r) {
    switch (type)
    {
    case RuleType::block:
    {
        QString rule = cleanRule(r);

        if (rule.isEmpty())
        {
            qDebug() << "Incorrect block rule skipped";
            return;
        }

        if (!blockRules.contains(rule))
        {
            blockRules += rule;
            qDebug() << "Block rule added:" << rule;
        }
        break;
    }
    case RuleType::transform:
    {
        QStringList ruleParts = cleanRules(r.split(" "), false);

        if (ruleParts.count() != 2 || ruleParts[0].isEmpty() || ruleParts[1].isEmpty())
        {
            qDebug() << "Incorrect transform rule skipped";
            return;
        }

        QPair<QString, QString> rule = QPair<QString, QString>(ruleParts[0], ruleParts[1]);

        if (!transformRules.contains(rule))
        {
            transformRules += rule;
            qDebug() << "Transform rule added:" << ruleParts[0] << " -> " + ruleParts[1];
        }
        break;
    }
    default:
    {
        qDebug() << "Unhandled rule type";
        break;
    }
    }
}

void FilterProxy::addRules(RuleType::type type, const QStringList &rules) {
    foreach (QString rule, rules)
        addRule(type, rule);
}

void FilterProxy::manageQuery() {
    //qDebug() << "new connection";
    QTcpServer *proxyServer = qobject_cast<QTcpServer*>(sender());
    QTcpSocket *socket = proxyServer->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(processQuery()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void FilterProxy::processQuery() {
    //qDebug() << "new query";
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
    if (applyBlockURLRules(url)) {
        socket->disconnectFromHost();
        return;
    }

    applyTransformURLRules(url);

    qDebug() << "Proxying URL:" << url;

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

void FilterProxy::sendRequest() {
    QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
    QByteArray requestData = proxySocket->property("requestData").toByteArray();
    proxySocket->write(requestData);
}

void FilterProxy::transferData() {
    QTcpSocket *proxySocket = qobject_cast<QTcpSocket*>(sender());
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
    socket->write(proxySocket->readAll());
}

void FilterProxy::closeConnection() {
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
