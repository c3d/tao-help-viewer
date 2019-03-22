// *****************************************************************************
// centralwidget.h                                                 Tao3D project
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
#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QtCore/QUrl>
#include <QWidget>
#include <QPrinter>

QT_BEGIN_NAMESPACE

class FindWidget;
class HelpViewer;
class MainWindow;
class QHelpSearchEngine;
class QTabWidget;
class SearchWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    CentralWidget(MainWindow *parent);
    ~CentralWidget();

    void setupWidget();
    bool hasSelection() const;
    QUrl currentSource() const;
    QString currentTitle() const;
    bool isHomeAvailable() const;
    bool isForwardAvailable() const;
    bool isBackwardAvailable() const;
    QList<QAction*> globalActions() const;
    void setGlobalActions(const QList<QAction*> &actions);

    HelpViewer *viewerAt(int index) const;
    HelpViewer *currentHelpViewer() const;

    bool searchWidgetAttached() const;
    void createSearchWidget(QHelpSearchEngine *searchEngine);
    void activateSearchWidget(bool updateLastTabPage = false);
    void removeSearchWidget();

    int availableHelpViewer() const;
    bool enableTabCloseAction() const;

    void closeOrReloadTabs(const QList<int> &indices, bool tryReload);
    void closeTabAt(int index);
    QMap<int, QString> currentSourceFileList() const;

    static CentralWidget *instance();

public slots:
    void zoomIn();
    void zoomOut();
    void nextPage();
    void resetZoom();
    void previousPage();
    void copySelection();
    void showTextSearch();
    void print();
    void pageSetup();
    void printPreview();
    void updateBrowserFont();
    void setSource(const QUrl &url);
    void setSourceInNewTab(const QUrl &url, qreal zoom = 0.0);
    HelpViewer *newEmptyTab();
    void home();
    void forward();
    void backward();

    void activateTab(bool onlyHelpViewer = false);

    void findNext();
    void findPrevious();
    void find(const QString &text, bool forward);

signals:
    void currentViewerChanged();
    void copyAvailable(bool yes);
    void sourceChanged(const QUrl &url);
    void highlighted(const QString &link);
    void forwardAvailable(bool available);
    void backwardAvailable(bool available);
    void addBookmark(const QString &title, const QString &url);

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void newTab();
    void closeTab();
    void setTabTitle(const QUrl& url);
    void currentPageChanged(int index);
    void showTabBarContextMenu(const QPoint &point);
    void printPreview(QPrinter *printer);
    void setSourceFromSearch(const QUrl &url);
    void setSourceFromSearchInNewTab(const QUrl &url);
    void highlightSearchTerms();

private:
    void connectSignals();
    bool eventFilter(QObject *object, QEvent *e);
    bool findInWebPage(const QString &ttf, bool forward);
    bool findInTextBrowser(const QString &ttf, bool forward);
    void initPrinter();
    QString quoteTabTitle(const QString &title) const;
    void setLastShownPages();

private:
    int lastTabPage;
    QList<QAction*> globalActionList;

    QTabWidget *tabWidget;
    FindWidget *findWidget;
    QPrinter *printer;
    bool usesDefaultCollection;

    SearchWidget *m_searchWidget;
};

QT_END_NAMESPACE

#endif  // CENTRALWIDGET_H
