/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtLocation module of the Qt Toolkit.
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
****************************************************************************/

#include "initreply.h"
#include "traverser.h"

CategoryInitReply::CategoryInitReply(QPlaceManagerEngineJsonDb *engine)
    : QPlaceReply(engine),
      m_engine(engine),
      m_traverser(new CategoryTraverser(m_engine->db(), this))
{
    Q_ASSERT(m_traverser);
    connect(m_traverser, SIGNAL(finished()),
            this, SLOT(requestFinished()));
}

CategoryInitReply::~CategoryInitReply()
{
    delete m_traverser;
}

void CategoryInitReply::start()
{
    m_traverser->start();
}

void CategoryInitReply::requestFinished()
{
    if (!m_traverser->errorString().isEmpty()) {
        QString errorString =
                QString::fromLatin1("Unknown error occurred during initialize catgory operation: %1")
                .arg(m_traverser->errorString());
        triggerDone(QPlaceReply::UnknownError, errorString);
        return;
    }

    CategoryTree tree = CategoryTraverser::convertToTree(m_traverser->results(), m_engine);
    m_engine->setCategoryTree(tree);
    triggerDone(QPlaceReply::NoError);
}
