/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef KWIZARD_H
#define KWIZARD_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "kgui/kvhbox.h"
#include <kcore/kglobal.h>

class KWizardPage;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KWizard : public QDialog
{
    Q_OBJECT;

    public:
        KWizard(QWidget *parent = 0);
        ~KWizard();
        KWizardPage *addPage(KWizardPage *page);
        void showPage(int index);
        void showPage(KWizardPage *page);

    private slots:
        void back();
        void next();
        void pageCompleted();
        void finish();

    private:
        QStackedWidget m_history;
        QPushButton *m_cancelButton;
        QPushButton *m_backButton;
        QPushButton *m_nextButton;
        QPushButton *m_finishButton;
        QHBoxLayout *m_buttonLayout;
        QVBoxLayout *m_mainLayout;
};

#include <QFrame>
#include <QGridLayout>
#include <QLabel>

class KWizardPage : public KVHBox
{
    Q_OBJECT
    public:
        KWizardPage(const QString &title, QWidget *parent = 0 );
        virtual ~KWizardPage();

        virtual bool isComplete() const = 0;
        virtual void reset() = 0;

        void setPixmap(const QPixmap &px);
        void setWidget(QWidget *w);

    public slots:
        virtual void aboutToNextPage() {}
        virtual void aboutToBackPage() {}
        virtual void aboutToFinish() {};

    private:
        QFrame *m_container;
        QGridLayout *m_layout;
        QLabel *m_image;

    signals:
        void completed();
};

#endif
