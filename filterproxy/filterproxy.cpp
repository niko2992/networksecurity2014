#include "filterproxy.h"

#include <QFileSystemWatcher>
#include <functional>

#define BLOCK_FILE     QByteArray("block.txt")
#define TRANSFORM_FILE QByteArray("transform.txt")

RuleType::type ruleTypeFromFilename(QString filePath)
{
    QFileInfo fileInfo(filePath);
    QString   fileName = fileInfo.fileName();

    if (fileName == "blacklist.txt")
      return RuleType::block;
    if (fileName == "transform.txt")
      return RuleType::transform;
    return RuleType::unknown;
}

class FilterProxyApp
{
public:
private:
};

int main(int argc, char *argv[])
{
    QCoreApplication   app(argc, argv);
    QFileSystemWatcher fileSystemWatcher;
    FilterProxy proxy;

    std::function<void (QString)> loadRuleFile = [&proxy](QString filePath)
    {
      QFile file(filePath);

      if (file.open(QFile::ReadOnly))
      {
        QString        rules    = file.readAll();
        QStringList    ruleList = rules.split('\n');
        RuleType::type ruleType = ruleTypeFromFilename(filePath);

        ruleList.removeAll("");
        proxy.addRules(ruleType, ruleList);
      }
      file.close();
    };

    fileSystemWatcher.addPaths(QStringList() << QFileInfo(BLOCK_FILE).absoluteFilePath() << QFileInfo(TRANSFORM_FILE).absoluteFilePath());
    QObject::connect(&fileSystemWatcher, &QFileSystemWatcher::fileChanged, loadRuleFile);

    loadRuleFile(BLOCK_FILE);
    loadRuleFile(TRANSFORM_FILE);
    return app.exec();
}
