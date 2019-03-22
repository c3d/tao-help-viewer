// *****************************************************************************
// xbelsupport.cpp                                                 Tao3D project
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
#include "tracer.h"

#include "xbelsupport.h"

#include "bookmarkitem.h"
#include "bookmarkmodel.h"

#include <QtCore/QDate>
#include <QtCore/QModelIndex>

QT_BEGIN_NAMESPACE

struct Bookmark {
    QString title;
    QString url;
    bool folded;
};

XbelWriter::XbelWriter(BookmarkModel *model)
    : QXmlStreamWriter()
    , bookmarkModel(model)
{
    TRACE_OBJ
    setAutoFormatting(true);
}

void XbelWriter::writeToFile(QIODevice *device)
{
    TRACE_OBJ
    setDevice(device);

    writeStartDocument();
    writeDTD(QLatin1String("<!DOCTYPE xbel>"));
    writeStartElement(QLatin1String("xbel"));
    writeAttribute(QLatin1String("version"), QLatin1String("1.0"));

    const QModelIndex &root = bookmarkModel->index(0,0, QModelIndex());
    for (int i = 0; i < bookmarkModel->rowCount(root); ++i)
        writeData(bookmarkModel->index(i, 0, root));
    writeEndDocument();
}

void XbelWriter::writeData(const QModelIndex &index)
{
    TRACE_OBJ
    if (index.isValid()) {
        Bookmark entry;
        entry.title = index.data().toString();
        entry.url = index.data(UserRoleUrl).toString();

        if (index.data(UserRoleFolder).toBool()) {
            writeStartElement(QLatin1String("folder"));
            entry.folded = !index.data(UserRoleExpanded).toBool();
            writeAttribute(QLatin1String("folded"), entry.folded
                ? QLatin1String("yes") : QLatin1String("no"));
            writeTextElement(QLatin1String("title"), entry.title);

            for (int i = 0; i < bookmarkModel->rowCount(index); ++i)
                writeData(bookmarkModel->index(i, 0 , index));
            writeEndElement();
        } else {
            writeStartElement(QLatin1String("bookmark"));
            writeAttribute(QLatin1String("href"), entry.url);
            writeTextElement(QLatin1String("title"), entry.title);
            writeEndElement();
        }
    }
}

// -- XbelReader

XbelReader::XbelReader(BookmarkModel *model)
    : QXmlStreamReader()
    , bookmarkModel(model)
{
    TRACE_OBJ
}

bool XbelReader::readFromFile(QIODevice *device)
{
    TRACE_OBJ
    setDevice(device);

    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == QLatin1String("xbel")
                && attributes().value(QLatin1String("version"))
                    == QLatin1String("1.0")) {
                const QModelIndex &root = bookmarkModel->index(0,0, QModelIndex());
                parents.append(bookmarkModel->addItem(root, true));
                readXBEL();
                bookmarkModel->setData(parents.first(),
                    QDate::currentDate().toString(Qt::ISODate), Qt::EditRole);
            } else {
                raiseError(QLatin1String("The file is not an XBEL version 1.0 file."));
            }
        }
    }

    return !error();
}

void XbelReader::readXBEL()
{
    TRACE_OBJ
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == QLatin1String("folder"))
                readFolder();
            else if (name() == QLatin1String("bookmark"))
                readBookmark();
            else
                readUnknownElement();
        }
    }
}

void XbelReader::readFolder()
{
    TRACE_OBJ
    parents.append(bookmarkModel->addItem(parents.last(), true));
    bookmarkModel->setData(parents.last(),
        attributes().value(QLatin1String("folded")) == QLatin1String("no"),
        UserRoleExpanded);

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == QLatin1String("title")) {
                bookmarkModel->setData(parents.last(), readElementText(),
                    Qt::EditRole);
            } else if (name() == QLatin1String("folder"))
                readFolder();
            else if (name() == QLatin1String("bookmark"))
                readBookmark();
            else
                readUnknownElement();
        }
    }

    parents.removeLast();
}

void XbelReader::readBookmark()
{
    TRACE_OBJ
    const QModelIndex &index = bookmarkModel->addItem(parents.last(), false);
    if (BookmarkItem* item = bookmarkModel->itemFromIndex(index)) {
        item->setData(UserRoleUrl, attributes().value(QLatin1String("href"))
            .toString());
    }

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == QLatin1String("title"))
                bookmarkModel->setData(index, readElementText(), Qt::EditRole);
            else
                readUnknownElement();
        }
    }
}

void XbelReader::readUnknownElement()
{
    TRACE_OBJ
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            readUnknownElement();
    }
}

QT_END_NAMESPACE
