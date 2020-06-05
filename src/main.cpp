#include "webookserver.h"

#include "src/CuteLogger/ConsoleAppender.h"

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
