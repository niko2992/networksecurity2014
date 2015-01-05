#ifndef ADDOPTION_H
#define ADDOPTION_H

#include <QWidget>
#include <QTimer>
#include "filterproxy.h"

namespace Ui {
class addoption;
}

class addoption : public QWidget
{
    Q_OBJECT

public:
    explicit addoption(QWidget *parent = 0);
    ~addoption();
    void setFilterProxy(FilterProxy*);

private:
    Ui::addoption *ui;
    QTimer timer;
    FilterProxy *filterproxy;

private slots:
    void saveOption();
    void clearLbl();
};

#endif // ADDOPTION_H
