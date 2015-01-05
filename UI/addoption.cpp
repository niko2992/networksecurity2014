#include "addoption.h"
#include "options.h"
#include "ui_addoption.h"

addoption::addoption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addoption)
{
    ui->setupUi(this);
    ui->typeList->addItem("Block");
    ui->typeList->addItem("Color");
    ui->typeList->addItem("Tranform");
    ui->msg->setStyleSheet("QLabel { color : green }");
    timer.setInterval(3000);
    connect(ui->Save, SIGNAL(clicked()), this, SLOT(saveOption()));
    connect(&timer, SIGNAL(timeout()),this, SLOT(clearLbl()));
}

addoption::~addoption()
{
    delete ui;
}

void addoption::saveOption()
{
    QString optionNew = "";
    optionNew += "Type : " + ui->typeList->currentText() + ", Rule : \"" + ui->rule->toPlainText() +"\"";
    options::opts << optionNew;
    ui->rule->setText("");
    ui->msg->setText("New rule added successfully.");
    timer.start();
}

void addoption::clearLbl()
{
    ui->msg->setText("");
}

void addoption::setFilterProxy(FilterProxy *f)
{
    filterproxy = f;
}
