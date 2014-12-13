#include "filterproxy.h"
#include "filterproxyapp.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    FilterProxy      proxy;
    FilterProxyApp   proxyApp(&proxy);

    proxyApp.addFilterFile(BLOCK_FILE);
    proxyApp.addFilterFile(TRANSFORM_FILE);
    return app.exec();
}

