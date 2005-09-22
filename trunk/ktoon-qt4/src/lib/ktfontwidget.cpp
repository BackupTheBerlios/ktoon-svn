#include <qcombobox.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qfont.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qscrollbar.h>
#include <qstringlist.h>
#include <qfontdatabase.h>
#include <q3whatsthis.h>
#include <qtooltip.h>

#include <qapplication.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <q3listbox.h>
#include <qspinbox.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <Q3ValueList>
#include <QVBoxLayout>

#ifdef Q_WS_X11
#include <X11/Xlib.h> 
#endif

#include "ktfontwidget.h"

static int minimumListWidth( const Q3ListBox *list )
{
	int w=0;
	for( uint i=0; i<list->count(); i++ )
	{
		int itemWidth = list->item(i)->width(list);
		w = QMAX(w,itemWidth);
	}
	if( w == 0 ) { w = 40; }
	w += list->frameWidth() * 2;
	w += list->verticalScrollBar()->sizeHint().width();
	return w;
}

static int minimumListHeight( const Q3ListBox *list, int numVisibleEntry )
{
int w = list->count() > 0 ? list->item(0)->height(list) :
		list->fontMetrics().lineSpacing();

if( w < 0 ) { w = 10; }
if( numVisibleEntry <= 0 ) { numVisibleEntry = 4; }
return ( w * numVisibleEntry + 2 * list->frameWidth() );
}

class KTFontWidget::KTFontWidgetPrivate
{
	public:
		KTFontWidgetPrivate()
		{ m_palette.setColor(QPalette::Active, QColorGroup::Text, Qt::black);
		m_palette.setColor(QPalette::Active, QColorGroup::Base, Qt::white); }
		QPalette m_palette;
};

KTFontWidget::KTFontWidget(QWidget *parent, const char *name,
			bool onlyFixed, const QStringList &fontList,
			bool makeFrame, int visibleListSize, bool diff,
			QCheckBox::ToggleState *sizeIsRelativeState )
	: QWidget(parent, name), usingFixed(onlyFixed)
{
// 	charsetsCombo = 0;

	QString mainWhatsThisText =
			 tr( "Here you can choose the font to be used." );
	Q3WhatsThis::add( this, mainWhatsThisText );

	d = new KTFontWidgetPrivate;
	QVBoxLayout *topLayout = new QVBoxLayout( this, 0 );
	int checkBoxGap = 10 / 2;

	QWidget *page;
	QGridLayout *gridLayout;
	int row = 0;
	if( makeFrame )
	{
		page = new Q3GroupBox(  tr("Requested Font"), this );
		topLayout->addWidget(page);
		gridLayout = new QGridLayout( page, 5, 3, 10,10);
		gridLayout->addRowSpacing( 0, fontMetrics().lineSpacing() );
		row = 1;
	}
	else
	{
		page = new QWidget( this );
		topLayout->addWidget(page);
		gridLayout = new QGridLayout( page, 4, 3, 0,10);
	}

	//
// first, create the labels across the top
	//
	QHBoxLayout *familyLayout = new QHBoxLayout();
	familyLayout->addSpacing( checkBoxGap );
	if (diff) {
		familyCheckbox = new QCheckBox( tr("Font"), page);
		connect(familyCheckbox, SIGNAL(toggled(bool)), SLOT(toggled_checkbox()));
		familyLayout->addWidget(familyCheckbox, 0, Qt::AlignLeft);
		QString familyCBToolTipText =
				 tr("Change font family?");
		QString familyCBWhatsThisText =
				 tr("Enable this checkbox to change the font family settings.");
		Q3WhatsThis::add( familyCheckbox, familyCBWhatsThisText );
		QToolTip::add(   familyCheckbox, familyCBToolTipText );
		familyLabel = 0;
	} else {
		familyCheckbox = 0;
		familyLabel = new QLabel(  tr("Font:"), page, "familyLabel" );
		familyLayout->addWidget(familyLabel, 1, Qt::AlignLeft);
	}
	gridLayout->addLayout(familyLayout, row, 0 );

	QHBoxLayout *styleLayout = new QHBoxLayout();
	if (diff) {
		styleCheckbox = new QCheckBox( tr("Font style"), page);
		connect(styleCheckbox, SIGNAL(toggled(bool)), SLOT(toggled_checkbox()));
		styleLayout->addWidget(styleCheckbox, 0, Qt::AlignLeft);
		QString styleCBToolTipText =
				 tr("Change font style?");
		QString styleCBWhatsThisText =
				 tr("Enable this checkbox to change the font style settings.");
		Q3WhatsThis::add( styleCheckbox, styleCBWhatsThisText );
		QToolTip::add(   styleCheckbox, styleCBToolTipText );
		styleLabel = 0;
	} else {
		styleCheckbox = 0;
		styleLabel = new QLabel(  tr("Font style:"), page, "styleLabel");
		styleLayout->addWidget(styleLabel, 1, Qt::AlignLeft);
	}
	styleLayout->addSpacing( checkBoxGap );
	gridLayout->addLayout(styleLayout, row, 1 );

	QHBoxLayout *sizeLayout = new QHBoxLayout();
	if (diff) {
		sizeCheckbox = new QCheckBox( tr("Size"),page);
		connect(sizeCheckbox, SIGNAL(toggled(bool)), SLOT(toggled_checkbox()));
		sizeLayout->addWidget(sizeCheckbox, 0, Qt::AlignLeft);
		QString sizeCBToolTipText =
				 tr("Change font size?");
		QString sizeCBWhatsThisText =
				 tr("Enable this checkbox to change the font size settings.");
		Q3WhatsThis::add( sizeCheckbox, sizeCBWhatsThisText );
		QToolTip::add(   sizeCheckbox, sizeCBToolTipText );
		sizeLabel = 0;
	} else {
		sizeCheckbox = 0;
		sizeLabel = new QLabel(  tr("Size:"), page, "sizeLabel");
		sizeLayout->addWidget(sizeLabel, 1, Qt::AlignLeft);
	}
	sizeLayout->addSpacing( checkBoxGap );
	sizeLayout->addSpacing( checkBoxGap ); // prevent label from eating border
	gridLayout->addLayout(sizeLayout, row, 2 );

	row ++;

	//
// now create the actual boxes that hold the info
	//
	familyListBox = new  Q3ListBox( page, "familyListBox");
	familyListBox->setEnabled( !diff );
	gridLayout->addWidget( familyListBox, row, 0 );
	QString fontFamilyWhatsThisText =
			 tr("Here you can choose the font family to be used." );
	Q3WhatsThis::add( familyListBox, fontFamilyWhatsThisText );
	Q3WhatsThis::add(diff?(QWidget *) familyCheckbox:(QWidget *) familyLabel, fontFamilyWhatsThisText );
	connect(familyListBox, SIGNAL(highlighted(const QString &)),
		SLOT(family_chosen_slot(const QString &)));
	if(!fontList.isEmpty())
	{
		familyListBox->insertStringList(fontList);
	}
	else
	{
		fillFamilyListBox(onlyFixed);
	}

	familyListBox->setMinimumWidth( minimumListWidth( familyListBox ) );
	familyListBox->setMinimumHeight(
			minimumListHeight( familyListBox, visibleListSize  ) );

	styleListBox = new  Q3ListBox( page, "styleListBox");
	styleListBox->setEnabled( !diff );
	gridLayout->addWidget(styleListBox, row, 1);
	QString fontStyleWhatsThisText =
			 tr("This is your font" );
	Q3WhatsThis::add( styleListBox, fontStyleWhatsThisText );
	Q3WhatsThis::add(diff?(QWidget *)styleCheckbox:(QWidget *)styleLabel, fontFamilyWhatsThisText );
	styleListBox->insertItem( tr("Regular"));
	styleListBox->insertItem( tr("Italic"));
	styleListBox->insertItem( tr("Bold"));
	styleListBox->insertItem( tr("Bold Italic"));
	styleListBox->setMinimumWidth( minimumListWidth( styleListBox ) );
	styleListBox->setMinimumHeight(
			minimumListHeight( styleListBox, visibleListSize  ) );

	connect(styleListBox, SIGNAL(highlighted(const QString &)),
		SLOT(style_chosen_slot(const QString &)));


	sizeListBox = new  Q3ListBox( page, "sizeListBox");
	sizeOfFont = new QSpinBox( page, "sizeOfFont");
	sizeOfFont->setMinValue(4);

	sizeListBox->setEnabled( !diff );
	sizeOfFont->setEnabled( !diff );
	if( sizeIsRelativeState ) {
		QString sizeIsRelativeCBText =
				 tr("Relative");
		QString sizeIsRelativeCBToolTipText =
				 tr("Font size<br><i>fixed</i> or <i>relative</i><br>to environment");
		QString sizeIsRelativeCBWhatsThisText =
				 tr("Here you can switch between fixed font size and font size "
				"to be calculated dynamically and adjusted to changing "
				"environment (e.g. widget dimensions, paper size)." );
		sizeIsRelativeCheckBox = new QCheckBox( sizeIsRelativeCBText,
				page,
				"sizeIsRelativeCheckBox" );
		sizeIsRelativeCheckBox->setTristate( diff );
		QGridLayout *sizeLayout2 = new QGridLayout( 3,2, 5, "sizeLayout2" );
		gridLayout->addLayout(sizeLayout2, row, 2);
		sizeLayout2->setColStretch( 1, 1 ); // to prevent text from eating the right border
		sizeLayout2->addMultiCellWidget( sizeOfFont, 0, 0, 0, 1);
		sizeLayout2->addMultiCellWidget(sizeListBox, 1,1, 0,1);
		sizeLayout2->addWidget(sizeIsRelativeCheckBox, 2, 0, Qt::AlignLeft);
		Q3WhatsThis::add( sizeIsRelativeCheckBox, sizeIsRelativeCBWhatsThisText );
		QToolTip::add(   sizeIsRelativeCheckBox, sizeIsRelativeCBToolTipText );
	}
	else {
		sizeIsRelativeCheckBox = 0L;
		QGridLayout *sizeLayout2 = new QGridLayout( 2,1, 5, "sizeLayout2" );
		gridLayout->addLayout(sizeLayout2, row, 2);
		sizeLayout2->addWidget( sizeOfFont, 0, 0);
		sizeLayout2->addMultiCellWidget(sizeListBox, 1,1, 0,0);
	}
	QString fontSizeWhatsThisText =
			 tr("Here you can choose the font size to be used." );
	Q3WhatsThis::add( sizeListBox, fontSizeWhatsThisText );
	Q3WhatsThis::add( diff?(QWidget *)sizeCheckbox:(QWidget *)sizeLabel, fontSizeWhatsThisText );

	fillSizeList();
	sizeListBox->setMinimumWidth( minimumListWidth(sizeListBox) +
			sizeListBox->fontMetrics().maxWidth() );
	sizeListBox->setMinimumHeight(
			minimumListHeight( sizeListBox, visibleListSize  ) );

	connect( sizeOfFont, SIGNAL( valueChanged(int) ),
		SLOT(size_value_slot(int)));

	connect( sizeListBox, SIGNAL(highlighted(const QString&)),
		SLOT(size_chosen_slot(const QString&)) );
	sizeListBox->setSelected(sizeListBox->findItem(QString::number(10)), true); // default to 10pt.

	row ++;

	row ++;
	sampleEdit = new QLineEdit( page, "sampleEdit");
	QFont tmpFont( QApplication::font().family(), 64, QFont::Black );
	sampleEdit->setFont(tmpFont);
	sampleEdit->setText( tr("Please choose your font"));
	sampleEdit->setMinimumHeight( sampleEdit->fontMetrics().lineSpacing() );
	sampleEdit->setAlignment(Qt::AlignCenter);
	gridLayout->addMultiCellWidget(sampleEdit, 4, 4, 0, 2);
	QString sampleEditWhatsThisText =
			 tr("This sample text illustrates the current settings. "
			"You may edit it to test special characters." );
	Q3WhatsThis::add( sampleEdit, sampleEditWhatsThisText );
	connect(this, SIGNAL(fontSelected(const QFont &)),
		SLOT(displaySample(const QFont &)));

	QVBoxLayout *vbox;
	if( makeFrame )
	{
		page = new Q3GroupBox(  tr("Actual Font"), this );
		topLayout->addWidget(page);
		vbox = new QVBoxLayout( page, 10 );
		vbox->addSpacing( fontMetrics().lineSpacing() );
	}
	else
	{
		page = new QWidget( this );
		topLayout->addWidget(page);
		vbox = new QVBoxLayout( page, 0, 10);
		QLabel *label = new QLabel(  tr("Actual Font"), page );
		vbox->addWidget( label );
	}

	xlfdEdit = new QLineEdit( page, "xlfdEdit" );
	vbox->addWidget( xlfdEdit );

// lets initialize the display if possible
	setFont( QApplication::font(), usingFixed );
// check or uncheck or gray out the "relative" checkbox
	if( sizeIsRelativeState && sizeIsRelativeCheckBox )
		setSizeIsRelative( *sizeIsRelativeState );
}

KTFontWidget::~KTFontWidget()
{
	delete d;
}

void KTFontWidget::fillSizeList() {
	if(! sizeListBox) return; //assertion.

	static const int c[] =
	{
		4,  5,  6,  7,
		8,  9,  10, 11,
		12, 13, 14, 15,
		16, 17, 18, 19,
		20, 22, 24, 26,
		28, 32, 48, 64,
		0
	};
	for(int i = 0; c[i]; ++i)
	{
		sizeListBox->insertItem(QString::number(c[i]));
	}
}

void KTFontWidget::setColor( const QColor & col )
{
	d->m_palette.setColor( QPalette::Active, QColorGroup::Text, col );
	QPalette pal = sampleEdit->palette();
	pal.setColor( QPalette::Active, QColorGroup::Text, col );
	sampleEdit->setPalette( pal );
}

QColor KTFontWidget::color() const
{
	return d->m_palette.color( QPalette::Active, QColorGroup::Text );
}

void KTFontWidget::setBackgroundColor( const QColor & col )
{
	d->m_palette.setColor( QPalette::Active, QColorGroup::Base, col );
	QPalette pal = sampleEdit->palette();
	pal.setColor( QPalette::Active, QColorGroup::Base, col );
	sampleEdit->setPalette( pal );
}

QColor KTFontWidget::backgroundColor() const
{
	return d->m_palette.color( QPalette::Active, QColorGroup::Base );
}

void KTFontWidget::setSizeIsRelative( QCheckBox::ToggleState relative )
{
// check or uncheck or gray out the "relative" checkbox
	if( sizeIsRelativeCheckBox ) {
		if( QCheckBox::NoChange == relative )
			sizeIsRelativeCheckBox->setNoChange();
		else
			sizeIsRelativeCheckBox->setChecked(  QCheckBox::On == relative );
	}
}

QCheckBox::ToggleState KTFontWidget::sizeIsRelative() const
{
	return sizeIsRelativeCheckBox
			? sizeIsRelativeCheckBox->state()
	: QCheckBox::NoChange;
}

QSize KTFontWidget::sizeHint( void ) const
{
	return minimumSizeHint();
}


void KTFontWidget::enableColumn( int column, bool state )
{
	if( column & FamilyList )
	{
		familyListBox->setEnabled(state);
	}
	if( column & StyleList )
	{
		styleListBox->setEnabled(state);
	}
	if( column & SizeList )
	{
		sizeListBox->setEnabled(state);
	}
}


void KTFontWidget::setFont( const QFont& aFont, bool onlyFixed )
{
	selFont = aFont;
	selectedSize=aFont.pointSize();
	if (selectedSize == -1)
		selectedSize = QFontInfo(aFont).pointSize();

	if( onlyFixed != usingFixed)
	{
		usingFixed = onlyFixed;
		fillFamilyListBox(usingFixed);
	}
	setupDisplay();
	displaySample(selFont);
}


int KTFontWidget::fontDiffFlags() {
	int diffFlags = 0;
	if (familyCheckbox && styleCheckbox && sizeCheckbox) {
		diffFlags = (int)(familyCheckbox->isChecked() ? FontDiffFamily : 0)
				| (int)( styleCheckbox->isChecked() ? FontDiffStyle  : 0)
				| (int)(  sizeCheckbox->isChecked() ? FontDiffSize   : 0);
	}
	return diffFlags;
}

void KTFontWidget::toggled_checkbox()
{
	familyListBox->setEnabled( familyCheckbox->isChecked() );
	styleListBox->setEnabled( styleCheckbox->isChecked() );
	sizeListBox->setEnabled( sizeCheckbox->isChecked() );
	sizeOfFont->setEnabled( sizeCheckbox->isChecked() );
}

void KTFontWidget::family_chosen_slot(const QString& family)
{
	QFontDatabase dbase;
	QStringList styles = QStringList(dbase.styles(family));
	styleListBox->clear();
	currentStyles.clear();
	for ( QStringList::Iterator it = styles.begin(); it != styles.end(); ++it ) {
		QString style = *it;
		int pos = style.find("Plain");
		if(pos >=0) style = style.replace(pos,5, tr("Regular"));
		pos = style.find("Normal");
		if(pos >=0) style = style.replace(pos,6, tr("Regular"));
		pos = style.find("Oblique");
		if(pos >=0) style = style.replace(pos,7, tr("Italic"));
		if(!styleListBox->findItem(style)) {
			styleListBox->insertItem( tr(style.utf8()));
			currentStyles.insert( tr(style.utf8()), *it);
		}
	}
	if(styleListBox->count()==0) {
		styleListBox->insertItem( tr("Regular"));
		currentStyles.insert( tr("Regular"), "Normal");
	}

	styleListBox->blockSignals(true);
	Q3ListBoxItem *item = styleListBox->findItem(selectedStyle);
	if (item)
		styleListBox->setSelected(styleListBox->findItem(selectedStyle), true);
	else
		styleListBox->setSelected(0, true);
	styleListBox->blockSignals(false);

	style_chosen_slot(QString::null);
}

void KTFontWidget::size_chosen_slot(const QString& size){

	selectedSize=size.toInt();
	sizeOfFont->setValue(selectedSize);
	selFont.setPointSize(selectedSize);
	emit fontSelected(selFont);
}

void KTFontWidget::size_value_slot(int val) {
	selFont.setPointSize(val);
	emit fontSelected(selFont);
}

void KTFontWidget::style_chosen_slot(const QString& style)
{
	QString currentStyle;
	if (style.isEmpty())
		currentStyle = styleListBox->currentText();
	else
		currentStyle = style;

	int diff=0; // the difference between the font size requested and what we can show.

	sizeListBox->clear();
	QFontDatabase dbase;
	if(dbase.isSmoothlyScalable(familyListBox->currentText(), currentStyles[currentStyle])) {  // is vector font
	//sampleEdit->setPaletteBackgroundPixmap( VectorPixmap ); // TODO
		fillSizeList();
	} else {                                // is bitmap font.
	//sampleEdit->setPaletteBackgroundPixmap( BitmapPixmap ); // TODO
		Q3ValueList<int> sizes = dbase.smoothSizes(familyListBox->currentText(), currentStyles[currentStyle]);
		if(sizes.count() > 0) {
			Q3ValueList<int>::iterator it;
			diff=1000;
			for ( it = sizes.begin(); it != sizes.end(); ++it ) {
				if(*it <= selectedSize || diff > *it - selectedSize) diff = selectedSize - *it;
				sizeListBox->insertItem(QString::number(*it));
			}
		} else // there are times QT does not provide the list..
			fillSizeList();
	}
	sizeListBox->blockSignals(true);
	sizeListBox->setSelected(sizeListBox->findItem(QString::number(selectedSize)), true);
	sizeListBox->blockSignals(false);
	sizeListBox->ensureCurrentVisible();

//kdDebug() << "Showing: " << familyListBox->currentText() << ", " << currentStyles[currentStyle] << ", " << selectedSize-diff << endl;
	selFont = dbase.font(familyListBox->currentText(), currentStyles[currentStyle], selectedSize-diff);
	emit fontSelected(selFont);
	if (!style.isEmpty())
		selectedStyle = style;
}

void KTFontWidget::displaySample(const QFont& font)
{
	sampleEdit->setFont(font);
	sampleEdit->setCursorPosition(0);
	xlfdEdit->setText(font.rawName());
	xlfdEdit->setCursorPosition(0);

//QFontInfo a = QFontInfo(font);
//kdDebug() << "font: " << a.family () << ", " << a.pointSize () << endl;
//kdDebug() << "      (" << font.toString() << ")\n";
}

void KTFontWidget::setupDisplay()
{
// Calling familyListBox->setCurrentItem() causes the value of selFont
// to change, so we save the family, style and size beforehand.
	QString family = selFont.family().lower();
	int style = (selFont.bold() ? 2 : 0) + (selFont.italic() ? 1 : 0);
	int size = selFont.pointSize();
	if (size == -1)
		size = QFontInfo(selFont).pointSize();
	QString sizeStr = QString::number(size);

	int numEntries, i;

	numEntries = familyListBox->count();
	for (i = 0; i < numEntries; i++) {
		if (family == familyListBox->text(i).lower()) {
			familyListBox->setCurrentItem(i);
			break;
		}
	}

// 1st Fallback
	if ( (i == numEntries) )
	{
		if (family.contains('['))
		{
			family = family.left(family.find('[')).stripWhiteSpace();
			for (i = 0; i < numEntries; i++) {
				if (family == familyListBox->text(i).lower()) {
					familyListBox->setCurrentItem(i);
					break;
				}
			}
		}
	}

// 2nd Fallback
	if ( (i == numEntries) )
	{
		QString fallback = family+" [";
		for (i = 0; i < numEntries; i++) {
			if (familyListBox->text(i).lower().startsWith(fallback)) {
				familyListBox->setCurrentItem(i);
				break;
			}
		}
	}

// 3rd Fallback
	if ( (i == numEntries) )
	{
		for (i = 0; i < numEntries; i++) {
			if (familyListBox->text(i).lower().startsWith(family)) {
				familyListBox->setCurrentItem(i);
				break;
			}
		}
	}

// Fall back in case nothing matched. Otherwise, diff doesn't work
	if ( i == numEntries )
		familyListBox->setCurrentItem( 0 );

	styleListBox->setCurrentItem(style);

	numEntries = sizeListBox->count();
	for (i = 0; i < numEntries; i++){
		if (sizeStr == sizeListBox->text(i)) {
			sizeListBox->setCurrentItem(i);
			break;
		}
	}

	sizeOfFont->setValue(size);
}


void KTFontWidget::getFontList( QStringList &list, uint fontListCriteria)
{
	QFontDatabase dbase;
	QStringList lstSys(dbase.families());

// if we have criteria; then check fonts before adding
	if (fontListCriteria)
	{
		QStringList lstFonts;
		for (QStringList::Iterator it = lstSys.begin(); it != lstSys.end(); ++it)
		{
			if ((fontListCriteria & FixedWidthFonts) > 0 && !dbase.isFixedPitch(*it)) continue;
			if (((fontListCriteria & (SmoothScalableFonts | ScalableFonts)) == ScalableFonts) &&
						!dbase.isBitmapScalable(*it)) continue;
			if ((fontListCriteria & SmoothScalableFonts) > 0 && !dbase.isSmoothlyScalable(*it)) continue;
			lstFonts.append(*it);
		}

		if((fontListCriteria & FixedWidthFonts) > 0) {
	// Fallback.. if there are no fixed fonts found, it's probably a
	// bug in the font server or Qt.  In this case, just use 'fixed'
			if (lstFonts.count() == 0)
				lstFonts.append("fixed");
		}

		lstSys = lstFonts;
	}

	lstSys.sort();

	list = lstSys;
}

void KTFontWidget::addFont( QStringList &list, const char *xfont )
{
	const char *ptr = strchr( xfont, '-' );
	if ( !ptr )
		return;

	ptr = strchr( ptr + 1, '-' );
	if ( !ptr )
		return;

	QString font = QString::fromLatin1(ptr + 1);

	int pos;
	if ( ( pos = font.find( '-' ) ) > 0 ) {
		font.truncate( pos );

		if ( font.find( QString::fromLatin1("open look"), 0, false ) >= 0 )
			return;

		QStringList::Iterator it = list.begin();

		for ( ; it != list.end(); ++it )
			if ( *it == font )
				return;
		list.append( font );
	}
}

void KTFontWidget::fillFamilyListBox(bool onlyFixedFonts)
{
	QStringList fontList;
	getFontList(fontList, onlyFixedFonts?FixedWidthFonts:0);
	familyListBox->clear();
	familyListBox->insertStringList(fontList);
}

void KTFontWidget::showXLFDArea(bool show)
{
	if( show )
	{
		xlfdEdit->parentWidget()->show();
	}
	else
	{
		xlfdEdit->parentWidget()->hide();
	}
}

void KTFontWidget::virtual_hook( int, void* )
{ /*BASE::virtual_hook( id, data );*/ }
