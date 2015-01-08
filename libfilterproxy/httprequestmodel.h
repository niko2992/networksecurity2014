#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QByteArray>
#include <QUrl>

struct HttpRequestModel
{
public:
    HttpRequestModel() {}
    HttpRequestModel(QByteArray source);

    QByteArray method, url, version, body;
    QUrl transformedUrl;

    QByteArray toByteArray() const;
    QByteArray requestLine() const;
};

#endif // HTTPREQUESTMODEL_H
