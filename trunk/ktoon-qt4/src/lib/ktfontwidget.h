/****************************************
 * This class is based on KFontWidget:
 * Copyright (C) 1997 Bernd Johannes Wuebben <wuebben@kde.org>
 * Copyright (c) 1999 Preston Brown <pbrown@kde.org>
 * Copyright (c) 1999 Mario Weilguni <mweilguni@kde.org>
 * 
 * I clean kde code for use in only-Qt apps
 * 
 ***************************************/

#ifndef KTFONTWIDGET_H
#define KTFONTWIDGET_H

#include <QLineEdit>
#include <QCheckBox>

class KTFontWidget : public QWidget
{
	public:
		KTFontWidget()
		{
		}
		~KTFontWidget()
		{
		}
};

#if 0

class QComboBox;
class QCheckBox;
class QFont;
class QLabel;
class QStringList;

/**
 * @class KTFontWidget
 * This class is based on KDE KFontWidget, It was adapted for use in only-qt applications
 */
class KFontWidget : public QWidget
{
	Q_OBJECT;
	Q_PROPERTY( QFont font READ font WRITE setFont USER true );

	public:
		enum FontColumn { FamilyList=0x01, StyleList=0x02, SizeList=0x04};

		enum FontDiff { FontDiffFamily=0x01, FontDiffStyle=0x02, FontDiffSize=0x04 };

		KFontWidget(QWidget *parent = 0L,
				   bool onlyFixed = false,
				   const QStringList &fontList = QStringList(),
				   bool makeFrame = true, int visibleListSize=8,
				   bool diff = false, Qt::CheckState *sizeIsRelativeState = 0L );

		   virtual ~KFontWidget();
		   void enableColumn( int column, bool state );

		   void setFont( const QFont &font, bool onlyFixed = false );


		   int fontDiffFlags();

		   QFont font() const { return selFont; }

		   void setColor( const QColor & col );

		   QColor color() const;

		   void setBackgroundColor( const QColor & col );

		   QColor backgroundColor() const;

		   void setSizeIsRelative( Qt::CheckState relative );

		   Qt::CheckState sizeIsRelative() const;

		   QString sampleText() const { return sampleEdit->text(); }

		   void setSampleText( const QString &text )
		   {
			   sampleEdit->setText( text );
		   }

		   void setSampleBoxVisible( bool visible )
		   {
			   sampleEdit->setVisible( visible );
		   }

		   static QString getXLFD( const QFont &theFont )
		   { return theFont.rawName(); }

		   enum FontListCriteria { FixedWidthFonts=0x01, ScalableFonts=0x02, SmoothScalableFonts=0x04 };
		   static void getFontList( QStringList &list, uint fontListCriteria);

		   virtual QSize sizeHint( void ) const;

	signals:
		void fontSelected( const QFont &font );

	private slots:
		void toggled_checkbox();
		void family_chosen_slot(const QString&);
		void size_chosen_slot(const QString&);
		void style_chosen_slot(const QString&);
		void displaySample(const QFont &font);
		void showXLFDArea(bool);
		void size_value_slot(int);
	private:
		void fillFamilyListBox(bool onlyFixedFonts = false);
		void fillSizeList();
		static void addFont( QStringList &list, const char *xfont );

		void setupDisplay();

		QStringList  fontList;

		KIntNumInput *sizeOfFont;

		QLineEdit    *sampleEdit;
		QLineEdit    *xlfdEdit;

		QLabel       *familyLabel;
		QLabel       *styleLabel;
		QCheckBox    *familyCheckbox;
		QCheckBox    *styleCheckbox;
		QCheckBox    *sizeCheckbox;
		QLabel       *sizeLabel;
		KListBox     *familyListBox;
		KListBox     *styleListBox;
		KListBox     *sizeListBox;
		QCheckBox    *sizeIsRelativeCheckBox;

		QFont        selFont;

		QString      selectedStyle;
		int          selectedSize;
		QMap<QString, QString> currentStyles;

		bool usingFixed;

	protected:
		virtual void virtual_hook( int id, void* data );
	private:
		class KFontWidgetPrivate;
		KFontWidgetPrivate *d;
};

#endif 

#endif // KTFONTWIDGET_H
