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

#ifndef KEXPORTWIZARD_H
#define KEXPORTWIZARD_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "kgui/kvhbox.h"
#include <kcore/kglobal.h>
#include <kgui/ktoolview.h>

class KExportWizardPage;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KExportWizard : public QDialog
{
    Q_OBJECT;

    public:
        KExportWizard(QWidget *parent = 0);
        ~KExportWizard();
        KExportWizardPage *addPage(KExportWizardPage *page);
        void showPage(int index);
        void showPage(KExportWizardPage *page);

    private slots:
        void cancel();
        void back();
        void next();
        void pageCompleted();
        void disableButton();
        void closeDialog();
        void setFormat(int code, const QString &extension);

    signals:
        void cancelled();
        void updateScenes();
        void saveFile();
        void exportArray();
        void setFileName();
        void isDone();

    private:
        QStackedWidget m_history;
        QPushButton *m_cancelButton;
        QPushButton *m_backButton;
        QPushButton *m_nextButton;
        QHBoxLayout *m_buttonLayout;
        QVBoxLayout *m_mainLayout;
        QString format;
};

#include <QFrame>
#include <QGridLayout>
#include <QLabel>

class KExportWizardPage : public KVHBox
{
    Q_OBJECT
    public:
        KExportWizardPage(const QString &title, QWidget *parent = 0);
        virtual ~KExportWizardPage();
        virtual bool isComplete() const = 0;
        virtual void reset() = 0;

        void setPixmap(const QPixmap &px);
        void setWidget(QWidget *w);
        void setTag(const QString &label);
        const QString getTag();

    public slots:
        virtual void aboutToNextPage() {}
        virtual void aboutToBackPage() {}

    signals:
        void completed();
        void emptyField();
        void formatSelected(int format, const QString &extension);

    private:
        QFrame *m_container;
        QGridLayout *m_layout;
        QLabel *m_image;
        QString tag;
};

#endif
