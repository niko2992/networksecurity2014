#include "options.h"
#include "ui_options.h"

QStringList options::opts;

options::options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::options)
{

    ui->setupUi(this);
    ui->optionsDisplay->setReadOnly(true);
    ui->optionsDisplay->setTextColor(QColor(180, 180, 255));
    affList();
    connect(ui->refresh, SIGNAL(clicked()), this, SLOT(affList()));
}

options::~options()
{
    delete ui;
}

void options::affList()
{
    QString txt = "";
    for (int i = 0; i<options::opts.size(); i++)
        txt += options::opts.at(i) + "\n";
    ui->optionsDisplay->setText(txt);
}

void options::setFilterProxy(FilterProxy *f)
{
    filterproxy = f;
}
