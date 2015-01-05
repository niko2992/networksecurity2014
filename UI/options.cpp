#include "options.h"
#include "ui_options.h"

options::options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::options)
{
    ui->setupUi(this);
    ui->optionsDisplay->setReadOnly(true);
    ui->optionsDisplay->setTextColor(QColor(180, 180, 255));
    //affList();
    connect(ui->refresh, SIGNAL(clicked()), this, SLOT(affList()));
}

options::~options()
{
    delete ui;
}

void options::affList()
{
    QStringList blockRules = filterproxy->getBlockRules();
    QList<QPair<QString, QString> > transformRules = filterproxy->getTransformRules();
    QString txt = "";

    for (int i = 0; i < blockRules.size(); i++)
        txt += "Rule type : Block, Rule : " + blockRules.at(i) + "\n";
    for (int i = 0; i < transformRules.size(); i++)
        txt += "Rule type : Transform, Rule : " + transformRules.at(i).first + " -> " + transformRules.at(i).second + "\n";
    ui->optionsDisplay->setText(txt);
}

void options::setFilterProxy(FilterProxy *f)
{
    filterproxy = f;
}
