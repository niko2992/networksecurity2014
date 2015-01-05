#ifndef PACKETS_H
#define PACKETS_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTimer>
#include <QFlags>
#include "filterproxy.h"

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

private slots:
    void showPackets();
    void startStop();
    void clearPacketList();
    void editPacket(QListWidgetItem*);
    void resendPacket();
};

#endif // PACKETS_H
