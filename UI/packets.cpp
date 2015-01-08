#include <QtNetwork>
#include "packets.h"
#include "ui_packets.h"
#include <QDebug>
#include <QTcpSocket>

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
      unsigned short port = 80;
      const HttpRequestModel& request = receivedPackets.at(row);
      QString line = ui->packetList->currentItem()->text();
      QTcpSocket* socket = new QTcpSocket(this);

      if (request.transformedUrl.port() < 0)
        port = request.transformedUrl.port();
      socket->connectToHost(request.transformedUrl.host(), port);
      socket->write(request.toByteArray());
      connect(socket, SIGNAL(readyRead()), this, SLOT(resentPacketResponse()));
      //connect(socket, SIGNAL(connected()), this, SLOT(resentPacketResponse()));
      //connect(socket, SIGNAL(disconnected()), this, SLOT(resentPacketResponse()));
      connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
      qDebug() << "Packet successfully resent";
    }
}

void packets::resentPacketResponse()
{
    qDebug() << "Received resent response";
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    qDebug() << socket->readAll();
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
