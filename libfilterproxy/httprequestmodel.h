#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QByteArray>

struct HttpRequestModel
{
public:
    HttpRequestModel() {}
    HttpRequestModel(QByteArray source);

    QByteArray method, url, version, body;
    QByteArray transformedUrl;

    QByteArray toByteArray() const;
    QByteArray requestLine() const;
};

#endif // HTTPREQUESTMODEL_H
