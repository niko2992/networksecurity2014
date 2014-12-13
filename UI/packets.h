#ifndef PACKETS_H
#define PACKETS_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class packets;
}

class packets : public QWidget
{
    Q_OBJECT

public:
    explicit packets(QWidget *parent = 0);
    ~packets();

private:
    Ui::packets *ui;
    QTimer timer;

private slots:
    void showPackets();
    void startStop();
    void clearPacketList();
};

#endif // PACKETS_H
