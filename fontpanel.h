// *****************************************************************************
// fontpanel.h                                                     Tao3D project
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
** This file is part of the tools applications of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the Qt tools.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef FONTPANEL_H
#define FONTPANEL_H

#include <QGroupBox>
#include <QFont>
#include <QFontDatabase>

QT_BEGIN_NAMESPACE

class QComboBox;
class QFontComboBox;
class QTimer;
class QLineEdit;

class FontPanel: public QGroupBox
{
    Q_OBJECT
public:
    FontPanel(QWidget *parentWidget = 0);

    QFont selectedFont() const;
    void setSelectedFont(const QFont &);

    QFontDatabase::WritingSystem writingSystem() const;
    void setWritingSystem(QFontDatabase::WritingSystem ws);

private slots:
    void slotWritingSystemChanged(int);
    void slotFamilyChanged(const QFont &);
    void slotStyleChanged(int);
    void slotPointSizeChanged(int);
    void slotUpdatePreviewFont();

private:
    QString family() const;
    QString styleString() const;
    int pointSize() const;
    int closestPointSizeIndex(int ps) const;

    void updateWritingSystem(QFontDatabase::WritingSystem ws);
    void updateFamily(const QString &family);
    void updatePointSizes(const QString &family, const QString &style);
    void delayedPreviewFontUpdate();

    QFontDatabase m_fontDatabase;
    QLineEdit *m_previewLineEdit;
    QComboBox *m_writingSystemComboBox;
    QFontComboBox* m_familyComboBox;
    QComboBox *m_styleComboBox;
    QComboBox *m_pointSizeComboBox;
    QTimer *m_previewFontUpdateTimer;
};

QT_END_NAMESPACE

#endif // FONTPANEL_H
