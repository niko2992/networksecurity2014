#include "filterproxy.h"

unsigned short FilterProxy::getPort(void) const
{
    QStringList arguments = QCoreApplication::arguments();

    for (int i = 0 ; i < arguments.length() ; ++i)
    {
      if (arguments[i] == "-p" || arguments[i] == "--port")
      {
        if (arguments.length() <= i + 1)
        {
          qDebug() << "/!\\ option --port was provided without a value.";
          break ;
        }
        return (arguments[i + 1].toShort());
      }
    }
    return (PROXY_DEFAULT_PORT);
}
