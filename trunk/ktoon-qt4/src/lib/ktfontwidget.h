/****************************************
 * This class is based on KFontChooser:
 * Copyright (C) 1997 Bernd Johannes Wuebben <wuebben@kde.org>
 * Copyright (c) 1999 Preston Brown <pbrown@kde.org>
 * Copyright (c) 1999 Mario Weilguni <mweilguni@kde.org>
 * 
 * I clean kde code for use in only-Qt apps
 * 
 ***************************************/

#ifndef KTFONTWIDGET_H
#define KTFONTWIDGET_H

#include <q3frame.h>
#include <q3button.h>
#include <qspinbox.h>
#include <q3listbox.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qmap.h>

#include <QListWidget>

/**
 * @class KTFontWidget
 * This class is based on KDE KFontChooser, I adapted for use in only-qt applications
 * @author David Cuadrado
 */
class KTFontWidget : public QWidget
{
	Q_OBJECT
// 	Q_PROPERTY( QFont font READ font WRITE setFont )
 
	public:
		enum FontColumn { FamilyList=0x01, StyleList=0x02, SizeList=0x04};
 
		enum FontDiff { FontDiffFamily=0x01, FontDiffStyle=0x02, FontDiffSize=0x04 };
 
		KTFontWidget(QWidget *parent = 0L, const char *name = 0L,
			     bool onlyFixed = false,
			     const QStringList &fontList = QStringList(),
			     bool makeFrame = true, int visibleListSize=8,
			     bool diff = false, QCheckBox::ToggleState *sizeIsRelativeState = 0L );
 
		virtual ~KTFontWidget();
 
		void enableColumn( int column, bool state );
 
		void setFont( const QFont &font, bool onlyFixed = false );
 
		int fontDiffFlags();
 
		QFont font() const { return selFont; }
 
		void setColor( const QColor & col );
 
		QColor color() const;
 
		void setBackgroundColor( const QColor & col );
 
		QColor backgroundColor() const;
 
		void setSizeIsRelative( QCheckBox::ToggleState relative );
 
		QCheckBox::ToggleState sizeIsRelative() const;
 
 
		QString sampleText() const { return sampleEdit->text(); }
 
		void setSampleText( const QString &text )
		{
			sampleEdit->setText( text );
		}
 
		static QString getXLFD( const QFont &theFont )
		{ return theFont.rawName(); }
 
		enum FontListCriteria { FixedWidthFonts=0x01, ScalableFonts=0x02, SmoothScalableFonts=0x04 };
 
		static void getFontList( QStringList &list, uint fontListCriteria);
 
		virtual QSize sizeHint( void ) const;
 
	signals:
		void fontSelected( const QFont &font );
		
	public slots:
		void showXLFDArea(bool);
 
	private slots:
		void toggled_checkbox();
		void family_chosen_slot(const QString&);
		void size_chosen_slot(const QString&);
		void style_chosen_slot(const QString&);
		void displaySample(const QFont &font);
		void size_value_slot(int);
	private:
		void fillFamilyListBox(bool onlyFixedFonts = false);
		void fillSizeList();
		// This one must be static since getFontList( QStringList, char*) is so
		static void addFont( QStringList &list, const char *xfont );
 
		void setupDisplay();
 
		// pointer to an optinally supplied list of fonts to
		// inserted into the fontdialog font-family combo-box
		QStringList  fontList;
 
		QSpinBox *sizeOfFont;
 
		QLineEdit    *sampleEdit;
		QLineEdit    *xlfdEdit;
 
		QLabel       *familyLabel;
		QLabel       *styleLabel;
		QCheckBox    *familyCheckbox;
		QCheckBox    *styleCheckbox;
		QCheckBox    *sizeCheckbox;
		QLabel       *sizeLabel;
		QListWidget     *familyListBox;
		QListWidget     *styleListBox;
		QListWidget     *sizeListBox;
// 		QComboBox    *charsetsCombo; // BIC: remove in KDE4
		QCheckBox    *sizeIsRelativeCheckBox;
 
		QFont        selFont;
 
		QString      selectedStyle;
		int          selectedSize;
		QMap<QString, QString> currentStyles;
 
		bool usingFixed;
 
	protected:
		class KTFontWidgetPrivate;
		KTFontWidgetPrivate *d;
};

#endif
