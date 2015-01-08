#ifndef PACKETS_H
#define PACKETS_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QFlags>
#include "filterproxy.h"
#include <QVector>

namespace Ui {
class packets;
}

class packets : public QWidget
{
    Q_OBJECT

public:
    explicit packets(QWidget *parent = 0);
    ~packets();
    void setFilterProxy(FilterProxy*);

private:
    Ui::packets *ui;
    QTimer timer;
    FilterProxy *filterproxy;
    QVector<HttpRequestModel> receivedPackets;

private slots:
    void showPackets();
    void startStop();
    void clearPacketList();
    void editPacket(QListWidgetItem*);
    void resendPacket();
    void receivedRequest(HttpRequestModel);
    void ignoredRequest(HttpRequestModel,QByteArray reason);
};

#endif // PACKETS_H
