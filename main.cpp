/**
 * @brief       Chronographe
 *
 * @details     This project is a cross-platform timer, allowing you to start/stop
 *              the timer in order to time a task for instance...
 *              Project inspired by http://joaomoreno.github.io/thyme/
 *
 * @version     0.1
 * @author      Adnan "Max13" RIHAN <adnan@rihan.fr>
 * @link        http://rihan.fr/
 * @copyright	http://creativecommons.org/licenses/by-nc-sa/3.0/	CC-by-nc-sa 3.0
 *
 * LICENSE: This source file is subject to the "Attribution-NonCommercial-ShareAlike 3.0 Unported"
 * of the Creative Commons license, that is available through the world-wide-web
 * at the following URI: http://creativecommons.org/licenses/by-nc-sa/3.0/.
 * If you did not receive a copy of this Creative Commons License and are unable
 * to obtain it through the web, please send a note to:
 * "Creative Commons, 171 Second Street, Suite 300,
 * San Francisco, California 94105, USA" so we can mail you a copy immediately.
 */

#include <QApplication>
#include <QIcon>
#include <QSettings>

#include "Timer/Timer.hpp"

int main(int argc, char *argv[])
{
    QApplication    a(argc, argv);
    QIcon           icon(":/icons/timer.svg");
    Timer           *t;

#ifdef  Q_OS_WIN
    a.setLibraryPaths(QStringList(a.applicationDirPath()+"/plugins"));
#endif
    a.setApplicationDisplayName("Chronographe");
    a.setApplicationName(a.applicationDisplayName());
    a.setApplicationVersion("0.1");
    a.setOrganizationDomain("rihan.fr");
    a.setOrganizationName("rihan.fr");
    a.setWindowIcon(icon);
    a.setQuitOnLastWindowClosed(false);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    t = new Timer(icon);
    t->show();

    return a.exec();
}
