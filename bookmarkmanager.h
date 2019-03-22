// *****************************************************************************
// bookmarkmanager.h                                               Tao3D project
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
#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QtCore/QMutex>
#include <QTreeView>

#include "ui_bookmarkwidget.h"

QT_BEGIN_NAMESPACE

class BookmarkManagerWidget;
class BookmarkModel;
class BookmarkFilterModel;
class QKeyEvent;
class QSortFilterProxyModel;

class BookmarkManager : public QObject
{
    Q_OBJECT
    class BookmarkWidget;
    class BookmarkTreeView;
    class BookmarkListView;
    Q_DISABLE_COPY(BookmarkManager);

public:
    static BookmarkManager* instance();
    static void destroy();

    QWidget* bookmarkDockWidget() const;
    void takeBookmarksMenu(QMenu* menu);

public slots:
    void addBookmark(const QString &title, const QString &url);

signals:
    void escapePressed();
    void setSource(const QUrl &url);
    void setSourceInNewTab(const QUrl &url);

private:
    BookmarkManager();
    ~BookmarkManager();

    void removeItem(const QModelIndex &index);
    bool eventFilter(QObject *object, QEvent *event);
    void buildBookmarksMenu(const QModelIndex &index, QMenu *menu);
    void showBookmarkDialog(const QString &name, const QString &url);

private slots:
    void setupFinished();

    void addBookmark();
    void removeBookmark();
    void manageBookmarks();
    void refeshBookmarkMenu();
    void renameBookmark(const QModelIndex &index);

    void setSourceFromAction(QAction *action);
    void setSourceFromIndex(const QModelIndex &index, bool newTab = false);

    void focusInEvent();
    void managerWidgetAboutToClose();
    void textChanged(const QString &text);
    void customContextMenuRequested(const QPoint &point);

private:
    bool typeAndSearch;

    static QMutex mutex;
    static BookmarkManager *bookmarkManager;

    QMenu *bookmarkMenu;

    BookmarkModel *bookmarkModel;
    BookmarkFilterModel *bookmarkFilterModel;
    QSortFilterProxyModel *typeAndSearchModel;

    BookmarkWidget *bookmarkWidget;
    BookmarkTreeView *bookmarkTreeView;
    BookmarkManagerWidget *bookmarkManagerWidget;
};

class BookmarkManager::BookmarkWidget : public QWidget
{
    Q_OBJECT
public:
    BookmarkWidget(QWidget *parent = 0)
        : QWidget(parent) { ui.setupUi(this); }
    virtual ~BookmarkWidget() {}

    Ui::BookmarkWidget ui;

signals:
    void focusInEvent();

private:
    void focusInEvent(QFocusEvent *event);
};

class BookmarkManager::BookmarkTreeView : public QTreeView
{
    Q_OBJECT
public:
    BookmarkTreeView(QWidget *parent = 0);
    ~BookmarkTreeView() {}

    void subclassKeyPressEvent(QKeyEvent *event);

private slots:
    void setExpandedData(const QModelIndex &index);
};

QT_END_NAMESPACE

#endif  // BOOKMARKMANAGER_H
