#include "packets.h"
#include "ui_packets.h"

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
}

packets::~packets()
{
    delete ui;
}

static bool started = false;
void packets::startStop()
{
    if (started)
    {
        timer.stop();
        ui->startStop->setText("Start");
    }
    else
    {
        timer.start();
        ui->startStop->setText("Stop");
        started = true;
    }
}

static int i = 0;
void packets::showPackets()
{
   ui->packetList->insertItem(0, "test" + QString::number(i));
   ui->packetList->insertItem(0, "tesqewqwet");
   ui->packetList->insertItem(0, "teqweqwes1221t");
   i++;
}

void packets::clearPacketList()
{
    ui->packetList->clear();
}
