// *****************************************************************************
// searchwidget.cpp                                                Tao3D project
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
#include "tracer.h"

#include "mainwindow.h"
#include "searchwidget.h"

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMimeData>

#include <QMenu>
#include <QLayout>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include <QTextBrowser>

#include <QtHelp/QHelpSearchEngine>
#include <QtHelp/QHelpSearchQueryWidget>
#include <QtHelp/QHelpSearchResultWidget>

QT_BEGIN_NAMESPACE

SearchWidget::SearchWidget(QHelpSearchEngine *engine, QWidget *parent)
    : QWidget(parent)
    , zoomCount(0)
    , attached(false)
    , searchEngine(engine)
{
    TRACE_OBJ
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    resultWidget = searchEngine->resultWidget();
    QHelpSearchQueryWidget *queryWidget = searchEngine->queryWidget();

    vLayout->addWidget(queryWidget);
    vLayout->addWidget(resultWidget);

    setFocusProxy(queryWidget);

    connect(queryWidget, SIGNAL(search()), this, SLOT(search()));
    connect(resultWidget, SIGNAL(requestShowLink(QUrl)), this,
        SIGNAL(requestShowLink(QUrl)));

    connect(searchEngine, SIGNAL(searchingStarted()), this,
        SLOT(searchingStarted()));
    connect(searchEngine, SIGNAL(searchingFinished(int)), this,
        SLOT(searchingFinished(int)));

    QTextBrowser* browser = qFindChild<QTextBrowser*>(resultWidget);
    if (browser) // Will be null if lib was configured not to use CLucene.
        browser->viewport()->installEventFilter(this);
}

SearchWidget::~SearchWidget()
{
    TRACE_OBJ
    // nothing todo
}

void SearchWidget::zoomIn()
{
    TRACE_OBJ
    QTextBrowser* browser = qFindChild<QTextBrowser*>(resultWidget);
    if (browser && zoomCount != 10) {
        zoomCount++;
        browser->zoomIn();
    }
}

void SearchWidget::zoomOut()
{
    TRACE_OBJ
    QTextBrowser* browser = qFindChild<QTextBrowser*>(resultWidget);
    if (browser && zoomCount != -5) {
        zoomCount--;
        browser->zoomOut();
    }
}

void SearchWidget::resetZoom()
{
    TRACE_OBJ
    if (zoomCount == 0)
        return;

    QTextBrowser* browser = qFindChild<QTextBrowser*>(resultWidget);
    if (browser) {
        browser->zoomOut(zoomCount);
        zoomCount = 0;
    }
}

bool SearchWidget::isAttached() const
{
    TRACE_OBJ
    return attached;
}

void SearchWidget::setAttached(bool state)
{
    TRACE_OBJ
    attached = state;
}

void SearchWidget::search() const
{
    TRACE_OBJ
    QList<QHelpSearchQuery> query = searchEngine->queryWidget()->query();
    searchEngine->search(query);
}

void SearchWidget::searchingStarted()
{
    TRACE_OBJ
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
}

void SearchWidget::searchingFinished(int hits)
{
    TRACE_OBJ
    Q_UNUSED(hits)
    qApp->restoreOverrideCursor();
}

bool SearchWidget::eventFilter(QObject* o, QEvent *e)
{
    TRACE_OBJ
    QTextBrowser* browser = qFindChild<QTextBrowser*>(resultWidget);
    if (browser && o == browser->viewport()
        && e->type() == QEvent::MouseButtonRelease){
        QMouseEvent *me = static_cast<QMouseEvent*>(e);
        QUrl link = resultWidget->linkAt(me->pos());
        if (!link.isEmpty() || link.isValid()) {
            bool controlPressed = me->modifiers() & Qt::ControlModifier;
            if((me->button() == Qt::LeftButton && controlPressed)
                || (me->button() == Qt::MidButton)) {
                    emit requestShowLinkInNewTab(link);
            }
        }
    }
    return QWidget::eventFilter(o,e);
}

void SearchWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    TRACE_OBJ
    if (keyEvent->key() == Qt::Key_Escape)
        MainWindow::activateCurrentBrowser();
    else
        keyEvent->ignore();
}

void SearchWidget::contextMenuEvent(QContextMenuEvent *contextMenuEvent)
{
    TRACE_OBJ
    QMenu menu;
    QPoint point = contextMenuEvent->globalPos();

    QTextBrowser* browser = qFindChild<QTextBrowser*>(resultWidget);
    if (!browser)
        return;

    point = browser->mapFromGlobal(point);
    if (!browser->rect().contains(point, true))
        return;

    QUrl link = browser->anchorAt(point);

    QKeySequence keySeq(QKeySequence::Copy);
    QAction *copyAction = menu.addAction(tr("&Copy") + QLatin1String("\t") +
        keySeq.toString(QKeySequence::NativeText));
    copyAction->setEnabled(QTextCursor(browser->textCursor()).hasSelection());

    QAction *copyAnchorAction = menu.addAction(tr("Copy &Link Location"));
    copyAnchorAction->setEnabled(!link.isEmpty() && link.isValid());

    keySeq = QKeySequence(Qt::CTRL);
    QAction *newTabAction = menu.addAction(tr("Open Link in New Tab") +
        QLatin1String("\t") + keySeq.toString(QKeySequence::NativeText) +
        QLatin1String("LMB"));
    newTabAction->setEnabled(!link.isEmpty() && link.isValid());

    menu.addSeparator();

    keySeq = QKeySequence::SelectAll;
    QAction *selectAllAction = menu.addAction(tr("Select All") +
        QLatin1String("\t") + keySeq.toString(QKeySequence::NativeText));

    QAction *usedAction = menu.exec(mapToGlobal(contextMenuEvent->pos()));
    if (usedAction == copyAction) {
        QTextCursor cursor = browser->textCursor();
        if (!cursor.isNull() && cursor.hasSelection()) {
            QString selectedText = cursor.selectedText();
            QMimeData *data = new QMimeData();
            data->setText(selectedText);
            QApplication::clipboard()->setMimeData(data);
        }
    }
    else if (usedAction == copyAnchorAction) {
        QApplication::clipboard()->setText(link.toString());
    }
    else if (usedAction == newTabAction) {
        emit requestShowLinkInNewTab(link);
    }
    else if (usedAction == selectAllAction) {
        browser->selectAll();
    }
}

QT_END_NAMESPACE
