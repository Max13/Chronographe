#-------------------------------------------------
#
# Project created by QtCreator 2013-11-28T11:13:34
#
#-------------------------------------------------

QT              +=  core gui widgets svg

TARGET          =   Chronographe
TEMPLATE        =   app


SOURCES         +=  main.cpp\
                    Timer/Timer.cpp

HEADERS         +=  Timer/Timer.hpp

RESOURCES       +=  resources/resources.qrc

TRANSLATIONS    =   i18n/lang_fr.ts

mac:ICON        =   icons/timer.icns
win32:RC_FILE   =   icons/timer.rc
OTHER_FILES     +=  icons/timer.rc\
                    README.md

cache()
