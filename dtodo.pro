TEMPLATE = app
TARGET = dtodo
DEPENDPATH += .
INCLUDEPATH += .
QT += xml
QT += network


# Input
HEADERS += MainWindow.h WidgetDialog.h Struct.h
SOURCES += main.cpp MainWindow.cpp WidgetDialog.cpp
TRANSLATIONS = dtodo_en.ts dtodo_fr.ts
RESOURCES = dtodo.qrc
RC_FILE += rsrc.rc

