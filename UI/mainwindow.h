#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filterproxy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    FilterProxy*    filterproxy;

private slots:
    void showViewPackets();
    void showOptions();
    void showAddOptions();
    void reset();
};

#endif // MAINWINDOW_H
