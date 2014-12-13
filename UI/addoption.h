#ifndef ADDOPTION_H
#define ADDOPTION_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class addoption;
}

class addoption : public QWidget
{
    Q_OBJECT

public:
    explicit addoption(QWidget *parent = 0);
    ~addoption();

private:
    Ui::addoption *ui;
    QTimer timer;

private slots:
    void saveOption();
    void clearLbl();
};

#endif // ADDOPTION_H
