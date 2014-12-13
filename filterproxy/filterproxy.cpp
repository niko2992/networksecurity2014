#include "filterproxy.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    FilterProxy proxy;

    QFile file;
    file.setFileName(":/blacklist.txt");
    if (!file.open(QFile::ReadOnly)) {
        qCritical() << "Can't access saved filter rules!";
        return 0;
    }
    QString blockRules = file.readAll();
    file.close();

    QStringList rules = blockRules.split('\n');
    rules.removeAll("");

    proxy.addRules(RuleType::block, rules);


    QFile file2;
    file2.setFileName(":/transform.txt");
    if (!file2.open(QFile::ReadOnly)) {
        qCritical() << "Can't access saved transform rules!";
        return 0;
    }
    QString transformRules = file2.readAll();
    file2.close();

    rules = transformRules.split('\n');
    rules.removeAll("");

    proxy.addRules(RuleType::transform, rules);

    return app.exec();
}
