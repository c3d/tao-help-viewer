# ******************************************************************************
# help_viewer.pro                                                  Tao3D project
# ******************************************************************************
#
# File description:
# Main Qt build file for the Tao help viewer application, based on the
# Qt Assistant of the Qt Toolkit.
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2013-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
# ******************************************************************************
# This file is part of Tao3D
#
# Tao3D is free software: you can r redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tao3D is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tao3D, in a file named COPYING.
# If not, see <https://www.gnu.org/licenses/>.
# ******************************************************************************

# Copyright (C) 2012 Taodyne SAS (contact@taodyne.com). All rights reserved.
#
# Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# Contact: Nokia Corporation (qt-info@nokia.com)
# GNU Lesser General Public License Usage
# This file may be used under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation and
# appearing in the file LICENSE.LGPL included in the packaging of this
# file. Please review the following information to ensure the GNU Lesser
# General Public License version 2.1 requirements will be met:
# http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
#
# In addition, as a special exception, Nokia gives you certain additional
# rights. These rights are described in the Nokia Qt LGPL Exception
# version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU General
# Public License version 3.0 as published by the Free Software Foundation
# and appearing in the file LICENSE.GPL included in the packaging of this
# file. Please review the following information to ensure the GNU General
# Public License version 3.0 requirements will be met:
# http://www.gnu.org/copyleft/gpl.html.
#
# Other Usage
# Alternatively, this file may be used in accordance with the terms and
# conditions contained in a signed written agreement between you and Nokia.
#

include(../main.pri)
include(../version.pri)
TEMPLATE = app
TARGET = assistant
QT_CONFIG += webkit
contains(QT_CONFIG, webkit):QT += webkit
CONFIG += qt \
    warn_on
QT += network
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += help printsupport webkitwidgets sql
} else {
    CONFIG += help
}
PROJECTNAME = Assistant
DEPENDPATH += ../shared
DEFINES += APP_NAME=\"\\\"$$APP_NAME\\\"\"

HEADERS += aboutdialog.h \
    bookmarkdialog.h \
    bookmarkfiltermodel.h \
    bookmarkitem.h \
    bookmarkmanager.h \
    bookmarkmanagerwidget.h \
    bookmarkmodel.h \
    centralwidget.h \
    cmdlineparser.h \
    collectionconfiguration.h \
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
    xbelsupport.h

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
    collectionconfiguration.cpp \
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
    xbelsupport.cpp

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
    TARGET = "Tao3D Help"
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
  # Workaround install problem: on Mac, the standard way of installing (the 'else'
  # part of this block) starts by recursively deleting $$target.path/$${TARGET}.app.
  # This is bad since we have previously stuff there :(
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
