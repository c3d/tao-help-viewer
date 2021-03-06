// *****************************************************************************
// mainwindow.h                                                    Tao3D project
// *****************************************************************************
//
// File description:
//
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2013,2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011,2013, Jérôme Forissier <jerome@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************
/*
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QUrl>
#include <QMainWindow>

QT_BEGIN_NAMESPACE

class QAction;
class QFileSystemWatcher;
class QLineEdit;
class QComboBox;
class QMenu;

class IndexWindow;
class QHelpEngineCore;
class QHelpEngine;
class CentralWidget;
class ContentWindow;
class CmdLineParser;
class QtDocInstaller;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(CmdLineParser *cmdLine, QWidget *parent = 0);
    ~MainWindow();

    static void activateCurrentBrowser();
    static QString collectionFileDirectory(bool createDir = false,
        const QString &cacheDir = QString());
    static QString defaultHelpCollectionFileName();

public:
    void setIndexString(const QString &str);
    void expandTOC(int depth);
    bool usesDefaultCollection() const;

signals:
    void initDone();

public slots:
    void setContentsVisible(bool visible);
    void setIndexVisible(bool visible);
    void setBookmarksVisible(bool visible);
    void setSearchVisible(bool visible);
    void showSearchWidget();
    void syncContents();
    void activateCurrentCentralWidgetTab();
    void currentFilterChanged(const QString &filter);

private slots:
    void showContents();
    void showIndex();
    void showSearch();
    void insertLastPages();
    void gotoAddress();
    void showPreferences();
    void showNewAddress();
    void showAboutDialog();
    void copyAvailable(bool yes);
    void updateNavigationItems();
    void updateTabCloseAction();
    void showNewAddress(const QUrl &url);
    void showTopicChooser(const QMap<QString, QUrl> &links, const QString &keyword);
    void updateApplicationFont();
    void filterDocumentation(const QString &customFilter);
    void setupFilterCombo();
    void lookForNewQtDocumentation();
    void indexingStarted();
    void indexingFinished();
    void qtDocumentationInstalled();
    void registerDocumentation(const QString &component,
        const QString &absFileName);
    void resetQtDocInfo(const QString &component);
    void checkInitState();
    void documentationRemoved(const QString &namespaceName);
    void documentationUpdated(const QString &namespaceName);

private:
    bool initHelpDB(bool registerInternalDoc);
    void setupActions();
    void closeEvent(QCloseEvent *e);
    void activateDockWidget(QWidget *w);
    void updateAboutMenuText();
    void setupFilterToolbar();
    void setupAddressToolbar();
    QMenu *toolBarMenu();
    void hideContents();
    void hideIndex();
    void hideSearch();

private slots:
    void showBookmarksDockWidget();
    void hideBookmarksDockWidget();

private:
    QWidget *m_bookmarkWidget;

private:
    CentralWidget *m_centralWidget;
    IndexWindow *m_indexWindow;
    ContentWindow *m_contentWindow;
    QLineEdit *m_addressLineEdit;
    QComboBox *m_filterCombo;

    QAction *m_backAction;
    QAction *m_nextAction;
    QAction *m_homeAction;
    QAction *m_syncAction;
    QAction *m_copyAction;
    QAction *m_findAction;
    QAction *m_printAction;
    QAction *m_printPreviewAction;
    QAction *m_pageSetupAction;
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_resetZoomAction;
    QAction *m_aboutAction;
    QAction *m_closeTabAction;
    QAction *m_newTabAction;

    QMenu *m_viewMenu;
    QMenu *m_toolBarMenu;

    CmdLineParser *m_cmdLine;

    QWidget *m_progressWidget;
    QtDocInstaller *m_qtDocInstaller;

    bool m_connectedInitSignals;
};

QT_END_NAMESPACE

#endif // MAINWINDOW_H
