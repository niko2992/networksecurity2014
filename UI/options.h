#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include <QTextEdit>
#include "filterproxy.h"

namespace Ui {
class options;
}

class options : public QWidget
{
    Q_OBJECT

public:
    explicit options(QWidget *parent = 0);
    ~options();
    void setFilterProxy(FilterProxy*);

private:
    Ui::options *ui;
    FilterProxy *filterproxy;

private slots:
    void affList();
};

#endif // OPTIONS_H
