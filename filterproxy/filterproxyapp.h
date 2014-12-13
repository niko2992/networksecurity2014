#ifndef FILTERPROXYAPP_H
#define FILTERPROXYAPP_H

#include <QObject>
#include <QFileSystemWatcher>
#include "filterproxy.h"

#define BLOCK_FILE     QByteArray("block.txt")
#define TRANSFORM_FILE QByteArray("transform.txt")

class FilterProxyApp : public QObject
{
    Q_OBJECT
public:
    FilterProxyApp(FilterProxy* proxy);

    void addFilterFile(QString filePath);

private slots:
    void loadRuleFile(QString filePath);

private:
    RuleType::type ruleTypeFromFilename(QString filePath);

    FilterProxy*       proxy;
    QFileSystemWatcher settingsWatcher;
};

#endif // FILTERPROXYAPP_H
