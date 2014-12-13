#include "filterproxy.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QFile file;
    file.setFileName(":/blacklist.txt");
    if (!file.open(QFile::ReadOnly)) {
        qCritical() << "Can't access saved filter rules!";
        return 0;
    }
    QString blockRules = file.readAll();
    file.close();

    QFile file2;
    file2.setFileName(":/transform.txt");
    if (!file2open(QFile::ReadOnly)) {
        qCritical() << "Can't access saved transform rules!";
        return 0;
    }
    QString transformRules = file2.readAll();
    file2.close();

    FilterProxy proxy;
    proxy.addRules(FilterProxy::RuleType::block, blockRules.split('\n'));
    proxy.addRules(FilterProxy::RuleType::transform, transformRules.split('\n'));
    return app.exec();
}
