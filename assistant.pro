include(../main.pri)
TEMPLATE = app
#LANGUAGE = C++
TARGET = assistant
contains(QT_CONFIG, webkit):QT += webkit
CONFIG += qt \
    warn_on \
    help
QT += network
PROJECTNAME = Assistant
DEPENDPATH += ../shared

HEADERS += aboutdialog.h \
    bookmarkdialog.h \
    bookmarkfiltermodel.h \
    bookmarkitem.h \
    bookmarkmanager.h \
    bookmarkmanagerwidget.h \
    bookmarkmodel.h \
    centralwidget.h \
    cmdlineparser.h \
    contentwindow.h \
    findwidget.h \
    filternamedialog.h \
    fontpanel.h \
    helpenginewrapper.h \
    helpviewer.h \
    indexwindow.h \
    installdialog.h \
    mainwindow.h \
    preferencesdialog.h \
    qtdocinstaller.h \
    remotecontrol.h \
    searchwidget.h \
    topicchooser.h \
    tracer.h \
    xbelsupport.h \
    collectionconfiguration.h
contains(QT_CONFIG, webkit) {
    HEADERS += helpviewer_qwv.h
} else {
   HEADERS += helpviewer_qtb.h
 }
win32:HEADERS += remotecontrol_win.h

SOURCES += aboutdialog.cpp \
    bookmarkdialog.cpp \
    bookmarkfiltermodel.cpp \
    bookmarkitem.cpp \
    bookmarkmanager.cpp \
    bookmarkmanagerwidget.cpp \
    bookmarkmodel.cpp \
    centralwidget.cpp \
    cmdlineparser.cpp \
    contentwindow.cpp \
    findwidget.cpp \
    filternamedialog.cpp \
    fontpanel.cpp \
    helpenginewrapper.cpp \
    helpviewer.cpp \
    indexwindow.cpp \
    installdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    preferencesdialog.cpp \
    qtdocinstaller.cpp \
    remotecontrol.cpp \
    searchwidget.cpp \
    topicchooser.cpp \
    xbelsupport.cpp \
    collectionconfiguration.cpp
 contains(QT_CONFIG, webkit) {
    SOURCES += helpviewer_qwv.cpp
} else {
    SOURCES += helpviewer_qtb.cpp
}

FORMS += bookmarkdialog.ui \
    bookmarkmanagerwidget.ui \
    bookmarkwidget.ui \
    filternamedialog.ui \
    installdialog.ui \
    preferencesdialog.ui \
    topicchooser.ui

RESOURCES += assistant.qrc \
    assistant_images.qrc

win32 {
    !wince*:LIBS += -lshell32
    RC_FILE = assistant.rc
}

mac {
    ICON = assistant.icns
    TARGET = "Tao Presentations Help"
    QMAKE_INFO_PLIST = Info_mac.plist

    # In *.lproj/InfoPlist.strings:
    # CFBundleName = The name in the application menu
    # CFBundleDisplayName = The name in finder and on the dock
    # Please use UTF-8 encoding
    lproj.path = $$APPINST/$${TARGET}.app/Contents/Resources
    lproj.files = *.lproj
    INSTALLS += lproj
}
macx {
  # See comment in tao.pro
  app.path    = $$APPINST
  app.extra   = \$(INSTALL_DIR) \"$${TARGET}.app\" \"$$APPINST\"
  INSTALLS   += app
  QMAKE_POST_LINK = sh ../fix_qt_refs_app  "$(TARGET)"
} else {
  target.path = $$APPINST
  INSTALLS   += target
}

contains(CONFIG, static): {
    SQLPLUGINS = $$unique(sql-plugins)
    contains(SQLPLUGINS, sqlite): {
        QTPLUGIN += qsqlite
        DEFINES += USE_STATIC_SQLITE_PLUGIN
    }
}

TRANSLATIONS = assistant_fr.ts
include(../translations.pri)
macx:translations.path = $$APPINST/$${TARGET}.app/Contents/MacOS
!macx:translations.path = $$APPINST
translations.files = *_fr.qm $$[QT_INSTALL_TRANSLATIONS]/qt_fr.qm $$[QT_INSTALL_TRANSLATIONS]/qt_help_fr.qm
INSTALLS += translations
