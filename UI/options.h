#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class options;
}

class options : public QWidget
{
    Q_OBJECT

public:
    explicit options(QWidget *parent = 0);
    ~options();
    static QStringList opts;

private:
    Ui::options *ui;

private slots:
    void affList();
};

#endif // OPTIONS_H
