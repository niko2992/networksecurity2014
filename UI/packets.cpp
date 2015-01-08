#include <QtNetwork>
#include "packets.h"
#include "ui_packets.h"
#include <QDebug>

packets::packets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::packets)
{
    ui->setupUi(this);
    timer.setInterval(3000);
    timer.setSingleShot(false);
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clearPacketList()));
    connect(ui->startStop, SIGNAL(clicked()), this, SLOT(startStop()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(showPackets()));
    connect(ui->packetList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editPacket(QListWidgetItem*)));
    connect(ui->resend, SIGNAL(clicked()), this, SLOT(resendPacket()));
}

packets::~packets()
{
    delete ui;
}

void packets::startStop()
{
    static bool started = false;

    if (started)
    {
        timer.stop();
        ui->startStop->setText("Start");
        started = false;
    }
    else
    {
        timer.start();
        ui->startStop->setText("Stop");
        started = true;
    }
}

void packets::showPackets()
{
}

void packets::clearPacketList()
{
    ui->packetList->clear();
    receivedPackets.clear();
}

void packets::editPacket(QListWidgetItem *item)
{
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

void packets::resendPacket()
{
    if (ui->packetList->currentItem() != 0)
    {
      int row = ui->packetList->currentRow();
      const HttpRequestModel& request = receivedPackets.at(row);
      QString line = ui->packetList->currentItem()->text();

      if (line.indexOf("| BLOCKED") != -1)
      {
        // Ce paquet a été bloqué, on peut pas le renvoyer ?
        return ;
      }
      request.toByteArray();
    }
}

void packets::setFilterProxy(FilterProxy *f)
{
    filterproxy = f;
    connect(filterproxy, SIGNAL(receivedRequest(HttpRequestModel)), this, SLOT(receivedRequest(HttpRequestModel)));
    connect(filterproxy, SIGNAL(ignoredRequest(HttpRequestModel,QByteArray)), this, SLOT(ignoredRequest(HttpRequestModel,QByteArray)));
}

void packets::receivedRequest(HttpRequestModel request)
{
    ui->packetList->insertItem(0, request.requestLine());
    receivedPackets.insert(0, request);
}

void packets::ignoredRequest(HttpRequestModel request, QByteArray reason)
{
    ui->packetList->insertItem(0, request.requestLine() + " | BLOCKED: " + reason);
    receivedPackets.insert(0, request);
}
