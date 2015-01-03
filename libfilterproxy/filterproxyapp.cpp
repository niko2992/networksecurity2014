#include "filterproxyapp.h"

FilterProxyApp::FilterProxyApp(FilterProxy* proxy) : QObject(proxy), proxy(proxy)
{
    QObject::connect(&settingsWatcher, &QFileSystemWatcher::fileChanged, this, &FilterProxyApp::loadRuleFile);
}

void FilterProxyApp::addFilterFile(QString path)
{
    settingsWatcher.addPath(QFileInfo(path).absoluteFilePath());
    loadRuleFile(path);
}

void FilterProxyApp::loadRuleFile(QString filePath)
{
    QFile file(filePath);

    if (file.open(QFile::ReadOnly))
    {
      QString        rules    = file.readAll();
      QStringList    ruleList = rules.split('\n');
      RuleType::type ruleType = ruleTypeFromFilename(filePath);

      ruleList.removeAll("");
      proxy->addRules(ruleType, ruleList);
    }
    file.close();
}

RuleType::type FilterProxyApp::ruleTypeFromFilename(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString   fileName = fileInfo.fileName();

    if (fileName == BLOCK_FILE)
      return RuleType::block;
    if (fileName == TRANSFORM_FILE)
      return RuleType::transform;
    return RuleType::unknown;
}
