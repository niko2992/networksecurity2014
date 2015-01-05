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
    ui->typeList->addItem("Transform");
    timer.setInterval(5000);
    connect(ui->Save, SIGNAL(clicked()), this, SLOT(saveOption()));
    connect(&timer, SIGNAL(timeout()),this, SLOT(clearLbl()));
}

addoption::~addoption()
{
    delete ui;
}

void addoption::saveOption()
{
    bool ret = false;
    if (ui->typeList->currentText() == "Block")
        ret = filterproxy->addRule(RuleType::block, ui->rule->toPlainText());
    else if (ui->typeList->currentText() == "Transform")
        ret = filterproxy->addRule(RuleType::transform, ui->rule->toPlainText());
    if (ret)
    {
        ui->rule->setText("");
        ui->msg->setStyleSheet("QLabel { color : green }");
        ui->msg->setText("New rule added successfully.");
    }
    else
    {
        ui->msg->setStyleSheet("QLabel { color : red }");
        ui->msg->setText("Bad format. Please edit your rule.");
    }
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
