#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <QtCore>

#define BEGIN_LOG "\n-------- B E G I N --------\n"
#define END_LOG "\n-------- E N D ------------\n"

/******************************************************************************
** class MyLogger                                                             *
*******************************************************************************/
class MyLogger
{
    public:
        static void logMessage(const QString& msg);

    private:
        static QFile  myLogFileHandle;

}; // end class MyLogger
#endif // end MYLOGGER_H
/* ***************************** End of File ******************************* */
