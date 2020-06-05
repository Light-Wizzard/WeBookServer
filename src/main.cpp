/******************************************************************************
** WeBookServer                                                               *
** WeBook is Pronounced Web-Book, it is a Web-Book Content Management System  *
*******************************************************************************/
#include "webookserver.h"
/******************************************************************************
** main                                                                       *
*******************************************************************************/
int main(int argc, char *argv[])
{
    ConsoleAppender* consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    cuteLogger->registerAppender(consoleAppender);
    LOG_INFO() << "Startging Main";
    WeBookServer service(argc, argv);
    service.exec();
} // end main
/******************************* End of File *********************************/
