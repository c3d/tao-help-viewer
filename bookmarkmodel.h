// *****************************************************************************
// bookmarkmodel.h                                                 Tao3D project
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
// (C) 2019, Christophe de Dinechin <christophe@dinechin.org>
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
#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QtCore/QAbstractItemModel>

#include <QtGui/QIcon>

QT_BEGIN_NAMESPACE

class BookmarkItem;
class QMimeData;
class QTreeView;

typedef QMap<BookmarkItem*, QPersistentModelIndex> ItemModelIndexCache;

class BookmarkModel : public QAbstractItemModel
{
     Q_OBJECT
public:
    BookmarkModel();
    ~BookmarkModel();

    QByteArray bookmarks() const;
    void setBookmarks(const QByteArray &bookmarks);

    void setItemsEditable(bool editable);
    void expandFoldersIfNeeeded(QTreeView *treeView);

    QModelIndex addItem(const QModelIndex &parent, bool isFolder = false);
    bool removeItem(const QModelIndex &index);

    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &index) const;

    Qt::DropActions supportedDropActions () const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;
    void setData(const QModelIndex &index, const QVector<QVariant> &data);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex indexFromItem(BookmarkItem *item) const;
    BookmarkItem *itemFromIndex(const QModelIndex &index) const;
    QList<QPersistentModelIndex> indexListFor(const QString &label) const;

    bool insertRows(int position, int rows, const QModelIndex &parent);
    bool removeRows(int position, int rows, const QModelIndex &parent);

    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
        int column, const QModelIndex &parent);

private:
    void setupCache(const QModelIndex &parent);
    QModelIndexList collectItems(const QModelIndex &parent) const;
    void collectItems(const QModelIndex &parent, qint32 depth,
        QDataStream *stream) const;

private:
    int columns;
    bool m_folder;
    bool m_editable;
    QIcon folderIcon;
    QIcon bookmarkIcon;
    QTreeView *treeView;
    BookmarkItem *rootItem;
    ItemModelIndexCache cache;
};

QT_END_NAMESPACE

#endif  // BOOKMARKMODEL_H
