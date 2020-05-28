#include "mylogger.h"

QFile MyLogger::myLogFileHandle;
/******************************************************************************
** logMessage                                                                 *
*******************************************************************************/
void MyLogger::logMessage(const QString &msg)
{
    static bool isOnetime = false;

    if(!isOnetime)
    {
        isOnetime = true;
        myLogFileHandle.setFileName(QDateTime::currentDateTime().toString("log_yyyy-MMMM-d-hh-mm-ss-zzz"));
    }
    if (!myLogFileHandle.isOpen())
    {
        myLogFileHandle.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    QTextStream ts(&myLogFileHandle);

    ts << msg << endl << flush;
}
/* ***************************** End of File ******************************* */
