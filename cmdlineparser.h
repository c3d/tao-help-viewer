// *****************************************************************************
// cmdlineparser.h                                                 Tao3D project
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

#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class CmdLineParser
{
    Q_DECLARE_TR_FUNCTIONS(CmdLineParser)
public:
    enum Result {Ok, Help, Error};
    enum ShowState {Untouched, Show, Hide, Activate};
    enum RegisterState {None, Register, Unregister};

    CmdLineParser(const QStringList &arguments);
    Result parse();

    void setCollectionFile(const QString &file);
    QString collectionFile() const;
    bool collectionFileGiven() const;
    QString cloneFile() const;
    QUrl url() const;
    bool enableRemoteControl() const;
    ShowState contents() const;
    ShowState index() const;
    ShowState bookmarks() const;
    ShowState search() const;
    QString currentFilter() const;
    bool removeSearchIndex() const;
    bool rebuildSearchIndex() const;
    RegisterState registerRequest() const;
    QString helpFile() const;

    void showMessage(const QString &msg, bool error);

private:
    QString getFileName(const QString &fileName);
    bool hasMoreArgs() const;
    const QString &nextArg();
    void handleCollectionFileOption();
    void handleShowUrlOption();
    void handleShowOption();
    void handleHideOption();
    void handleActivateOption();
    void handleShowOrHideOrActivateOption(ShowState state);
    void handleRegisterOption();
    void handleUnregisterOption();
    void handleRegisterOrUnregisterOption(RegisterState state);
    void handleSetCurrentFilterOption();

    QStringList m_arguments;
    int m_pos;
    QString m_collectionFile;
    QString m_cloneFile;
    QString m_helpFile;
    QUrl m_url;
    bool m_enableRemoteControl;

    ShowState m_contents;
    ShowState m_index;
    ShowState m_bookmarks;
    ShowState m_search;
    RegisterState m_register;
    QString m_currentFilter;
    bool m_removeSearchIndex;
    bool m_rebuildSearchIndex;
    bool m_quiet;
    QString m_error;
};

QT_END_NAMESPACE

#endif
