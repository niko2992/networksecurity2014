#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    filterproxy = new FilterProxy(this);
    ui->setupUi(this);
    reset();
    ui->widget->setFilterProxy(filterproxy);
    ui->widget_2->setFilterProxy(filterproxy);
    ui->widget_3->setFilterProxy(filterproxy);
    ui->widget->show();
    connect(ui->actionView_packets, SIGNAL(triggered()), this, SLOT(showViewPackets()));
    connect(ui->actionView_options, SIGNAL(triggered()), this, SLOT(showOptions()));
    connect(ui->actionAdd_option, SIGNAL(triggered()), this, SLOT(showAddOptions()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showOptions()
{
    reset();
    ui->widget_2->show();
}

void MainWindow::showViewPackets()
{
    reset();
    ui->widget->show();
}

void MainWindow::showAddOptions()
{
    reset();
    ui->widget_3->show();
}

void MainWindow::reset()
{
    ui->widget->hide();
    ui->widget_2->hide();
    ui->widget_3->hide();
}
