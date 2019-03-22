// *****************************************************************************
// bookmarkmanagerwidget.cpp                                       Tao3D project
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
#include "bookmarkmanagerwidget.h"
#include "bookmarkitem.h"
#include "bookmarkmodel.h"
#include "tracer.h"
#include "xbelsupport.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QUrl>

#include <QtGui/QCloseEvent>
#include <QFileDialog>
#include <QKeySequence>
#include <QMessageBox>
#include <QShortcut>

QT_BEGIN_NAMESPACE

namespace {
    #define TR(x) QCoreApplication::translate("BookmarkManager", x)
}

BookmarkManagerWidget::BookmarkManagerWidget(BookmarkModel *sourceModel,
        QWidget *parent)
    : QWidget(parent)
    , bookmarkModel(sourceModel)
{
    TRACE_OBJ
    ui.setupUi(this);

    ui.treeView->setModel(bookmarkModel);

    ui.treeView->expandAll();
    ui.treeView->installEventFilter(this);
    ui.treeView->viewport()->installEventFilter(this);
    ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.treeView, SIGNAL(customContextMenuRequested(QPoint)), this,
        SLOT(customContextMenuRequested(QPoint)));

    connect(ui.remove, SIGNAL(clicked()), this, SLOT(removeItem()));
    connect(ui.lineEdit, SIGNAL(textChanged(QString)), this,
        SLOT(textChanged(QString)));
    new QShortcut(QKeySequence::Find, ui.lineEdit, SLOT(setFocus()));

    importExportMenu.addAction(tr("Import..."), this, SLOT(importBookmarks()));
    importExportMenu.addAction(tr("Export..."), this, SLOT(exportBookmarks()));
    ui.importExport->setMenu(&importExportMenu);

    new QShortcut(QKeySequence::FindNext, this, SLOT(findNext()));
    new QShortcut(QKeySequence::FindPrevious, this, SLOT(findPrevious()));

    connect(bookmarkModel, SIGNAL(rowsRemoved(QModelIndex, int, int)), this,
        SLOT(refeshBookmarkCache()));
    connect(bookmarkModel, SIGNAL(rowsInserted(QModelIndex, int, int)), this,
        SLOT(refeshBookmarkCache()));
    connect(bookmarkModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this,
        SLOT(refeshBookmarkCache()));

    ui.treeView->setCurrentIndex(ui.treeView->indexAt(QPoint(2, 2)));
}

BookmarkManagerWidget::~BookmarkManagerWidget()
{
    TRACE_OBJ
}

void BookmarkManagerWidget::closeEvent(QCloseEvent *event)
{
    TRACE_OBJ
    event->accept();
    emit managerWidgetAboutToClose();
}

void BookmarkManagerWidget::renameItem(const QModelIndex &index)
{
    TRACE_OBJ
    // check if we should rename the "Bookmarks Menu", bail
    if (!bookmarkModel->parent(index).isValid())
        return;

    bookmarkModel->setItemsEditable(true);
    ui.treeView->edit(index);
    bookmarkModel->setItemsEditable(false);
}

static int nextIndex(int current, int count, bool forward)
{
    TRACE_OBJ
    if (current >= 0)
        return (forward ? (current + 1) : ((current - 1) + count)) % count;
    return 0;
}

void BookmarkManagerWidget::selectNextIndex(bool direction) const
{
    QModelIndex current = ui.treeView->currentIndex();
    if (current.isValid() && !cache.isEmpty()) {
        current = cache.at(nextIndex(cache.indexOf(current), cache.count(),
            direction));
    }
    ui.treeView->setCurrentIndex(current);
}

bool BookmarkManagerWidget::eventFilter(QObject *object, QEvent *event)
{
    TRACE_OBJ
    if (object != ui.treeView && object != ui.treeView->viewport())
        return QWidget::eventFilter(object, event);

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        switch (ke->key()) {
            case Qt::Key_F2: {
                renameItem(ui.treeView->currentIndex());
            }   break;

            case Qt::Key_Delete: {
                removeItem(ui.treeView->currentIndex());
            }   break;

            default: break;
        }
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        switch (me->button()) {
            case Qt::LeftButton: {
                if (me->modifiers() & Qt::ControlModifier)
                    setSourceFromIndex(ui.treeView->currentIndex(), true);
            }   break;

            case Qt::MidButton: {
                setSourceFromIndex(ui.treeView->currentIndex(), true);
            }   break;

            default: break;
        }
    }
    return QObject::eventFilter(object, event);
}

void BookmarkManagerWidget::findNext()
{
    TRACE_OBJ
    selectNextIndex(true);
}

void BookmarkManagerWidget::findPrevious()
{
    TRACE_OBJ
    selectNextIndex(false);
}

void BookmarkManagerWidget::importBookmarks()
{
    TRACE_OBJ
    const QString &fileName = QFileDialog::getOpenFileName(0, TR("Open File"),
        QDir::currentPath(), TR("Files (*.xbel)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        XbelReader reader(bookmarkModel);
        reader.readFromFile(&file);
    }
}

void BookmarkManagerWidget::exportBookmarks()
{
    TRACE_OBJ
    QString fileName = QFileDialog::getSaveFileName(0, TR("Save File"),
        QLatin1String("untitled.xbel"), TR("Files (*.xbel)"));

    const QLatin1String suffix(".xbel");
    if (!fileName.endsWith(suffix))
        fileName.append(suffix);

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        XbelWriter writer(bookmarkModel);
        writer.writeToFile(&file);
    } else {
        QMessageBox::information(this, TR("Qt Assistant"),
            TR("Unable to save bookmarks."), TR("OK"));
    }
}

void BookmarkManagerWidget::refeshBookmarkCache()
{
    TRACE_OBJ
    cache.clear();

    const QString &text = ui.lineEdit->text();
    if (!text.isEmpty())
        cache = bookmarkModel->indexListFor(text);
}

void BookmarkManagerWidget::textChanged(const QString &/*text*/)
{
    TRACE_OBJ
    refeshBookmarkCache();
    if (!cache.isEmpty())
        ui.treeView->setCurrentIndex(cache.at(0));
}

void BookmarkManagerWidget::removeItem(const QModelIndex &index)
{
    TRACE_OBJ
    QModelIndex current = index.isValid() ? index : ui.treeView->currentIndex();
    if (!bookmarkModel->parent(current).isValid())
        return;  // check if we should delete the "Bookmarks Menu", bail

    if (bookmarkModel->hasChildren(current)) {
        int value = QMessageBox::question(this, TR("Remove"), TR("You are going"
            "to delete a Folder, this will also<br> remove it's content. Are "
            "you sure to continue?"),
            QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
        if (value == QMessageBox::Cancel)
            return;
    }
    bookmarkModel->removeItem(current);
}

void BookmarkManagerWidget::customContextMenuRequested(const QPoint &point)
{
    TRACE_OBJ
    const QModelIndex &index = ui.treeView->indexAt(point);
    if (!index.isValid())
        return;

    // check if we should open the menu on "Bookmarks Menu", bail
    if (!bookmarkModel->parent(index).isValid())
        return;

    QAction *remove = 0;
    QAction *rename = 0;
    QAction *showItem = 0;
    QAction *showItemInNewTab = 0;

    QMenu menu(QLatin1String(""));
    if (bookmarkModel->data(index, UserRoleFolder).toBool()) {
        remove = menu.addAction(TR("Delete Folder"));
        rename = menu.addAction(TR("Rename Folder"));
    } else {
        showItem = menu.addAction(TR("Show Bookmark"));
        showItemInNewTab = menu.addAction(TR("Show Bookmark in New Tab"));
        menu.addSeparator();
        remove = menu.addAction(TR("Delete Bookmark"));
        rename = menu.addAction(TR("Rename Bookmark"));
    }

    QAction *pickedAction = menu.exec(ui.treeView->mapToGlobal(point));
    if (pickedAction == rename)
        renameItem(index);
    else if (pickedAction == remove)
        removeItem(index);
    else if (pickedAction == showItem || pickedAction == showItemInNewTab)
        setSourceFromIndex(index, pickedAction == showItemInNewTab);
}

void
BookmarkManagerWidget::setSourceFromIndex(const QModelIndex &index, bool newTab)
{
    TRACE_OBJ
    if (bookmarkModel->data(index, UserRoleFolder).toBool())
        return;

    const QVariant &data = bookmarkModel->data(index, UserRoleUrl);
    if (data.canConvert<QUrl>()) {
        if (newTab)
            emit setSourceInNewTab(data.toUrl());
        else
            emit setSource(data.toUrl());
    }
}

QT_END_NAMESPACE
