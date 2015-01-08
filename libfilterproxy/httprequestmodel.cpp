#include "httprequestmodel.h"
#include <QList>

HttpRequestModel::HttpRequestModel(QByteArray source)
{
    int pos = source.indexOf("\r\n");
    QByteArray requestLine = source.left(pos);

    // set request line
    QList<QByteArray> entries = requestLine.split(' ');
    method  = entries.value(0);
    url     = entries.value(1);
    version = entries.value(2);
    // set body
    source.remove(0, pos + 2);
    body = source;
}

QByteArray HttpRequestModel::toByteArray() const {
    return requestLine() + "\r\n" + body;
}

QByteArray HttpRequestModel::requestLine() const {
    return method + ' ' + transformedUrl + ' ' + version;
}
