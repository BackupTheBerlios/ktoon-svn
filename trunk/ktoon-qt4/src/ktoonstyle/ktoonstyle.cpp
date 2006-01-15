/****************************************************************************
**
** Copyright (C) 2005-2005 Trolltech AS. All rights reserved.
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "ktoonstyle.h"
#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPixmapCache>
#include <QRadioButton>
#include <QString>
#include <QStyleOption>
#include <QtDebug>
#include <QPushButton>
#include <QGroupBox>

QPixmap cached(const QString &img)
{
    if (QPixmap *p = QPixmapCache::find(img))
        return *p;

    QPixmap pm;
    pm = QPixmap::fromImage(QImage(img), Qt::OrderedDither | Qt::OrderedAlphaDither);
    if (pm.isNull())
        return QPixmap();

    QPixmapCache::insert(img, pm);
    return pm;
}


KToonStyle::KToonStyle()
    : QPlastiqueStyle()
{
    Q_INIT_RESOURCE(ktoonstyle);
}


void KToonStyle::drawHoverRect(QPainter *painter, const QRect &r) const
{
    double h = r.height();
    double h2 = r.height() / 2.0;
    QPainterPath path;
    path.addRect(r.x() + h2, r.y() + 0, r.width() - h2 * 2, r.height());
    path.addEllipse(r.x(), r.y(), h, h);
    path.addEllipse(r.x() + r.width() - h, r.y(), h, h);
    path.setFillRule(Qt::WindingFill);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0xB5B5A7));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(path);
}


void KToonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,QPainter *painter, const QWidget *widget) const
{
	Q_ASSERT(option);
	
	
	switch (element)
	{
		case PE_FrameFocusRect:
			break;

		case PE_IndicatorRadioButton:
		{
			if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option))
			{
				bool hover = (button->state & State_Enabled) && (button->state & State_MouseOver);
				painter->save();
				QPixmap radio;
				if (hover)
					drawHoverRect(painter, widget->rect());

				if (button->state & State_Sunken)
					radio = cached(":res/images/radiobutton-on.png");
				else if (button->state & State_On)
					radio = cached(":res/images/radiobutton_on.png");
				else
					radio = cached(":res/images/radiobutton_off.png");
				painter->drawPixmap(button->rect.topLeft(), radio);

				painter->restore();
			}
			else
			{
				QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
			}
			break;
		}
		case PE_PanelButtonCommand:
		{
			const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option);
			if (button)
			{
				bool hover = (button->state & State_Enabled) && (button->state & State_MouseOver);

				painter->save();
				const QPushButton *pushButton = qobject_cast<const QPushButton *>(widget);
				
				if ( ! pushButton  )
				{
					QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
					break;
				}
				
				QWidget *parent = pushButton->parentWidget();
				if (parent && qobject_cast<QGroupBox *>(parent))
				{
					QLinearGradient lg(0, 0, 0, parent->height());
					lg.setColorAt(0, QColor(224,224,224));
					lg.setColorAt(1, QColor(255,255,255));
					painter->setPen(Qt::NoPen);
					painter->setBrush(lg);
					painter->setBrushOrigin(-widget->mapToParent(QPoint(0,0)));
					painter->drawRect(button->rect);
					painter->setBrushOrigin(0,0);
				}
								
				bool down = pushButton && ((button->state & State_Sunken) || (button->state & State_On));
				
				bool goodSize = pushButton && pushButton->height() > 22;
				
				QPixmap left, right, mid;
				if (down && goodSize)
				{
					left = cached(":res/images/button_pressed_cap_left.png");
					
					right = cached(":res/images/button_pressed_cap_right.png");
					mid = cached(":res/images/button_pressed_stretch.png");
				} 
				else if (goodSize)
				{
					left = cached(":res/images/button_normal_cap_left.png");
					right = cached(":res/images/button_normal_cap_right.png");
					mid = cached(":res/images/button_normal_stretch.png");
				}
				
				if ( goodSize )
				{
					painter->drawPixmap(button->rect.topLeft(), left);
					painter->drawTiledPixmap(QRect(button->rect.x() + left.width(),button->rect.y(),button->rect.width() - left.width() - right.width(),
							left.height()),
					mid);
					painter->drawPixmap(button->rect.x() + button->rect.width() - right.width(),
							button->rect.y(),
							right);
				}
				
				if (hover)
				{
					painter->fillRect(widget->rect().adjusted(3,5,-3,-5), QColor(41,57,127,63));
				}
				painter->restore();
				
				if ( !goodSize )
				{
					QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
				}
			}
			else
			{
				QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
			}
			break;
		}
		case PE_FrameGroupBox:
		{
#if QT_VERSION >= 0x040100
			const QStyleOptionFrameV2 *group = qstyleoption_cast<const QStyleOptionFrameV2 *>(option);
			if (group)
			{
				const QRect &r = group->rect;
				if ( r.height() < 30 ) // FIXME FIXME FIXME
				{
					QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
					return;
				}
				
				painter->save();
				int radius = 14;
				int radius2 = radius*2;
				QPainterPath clipPath;
				clipPath.moveTo(radius, 0);
				clipPath.arcTo(r.right() - radius2, 0, radius2, radius2, 90, -90);
				clipPath.arcTo(r.right() - radius2, r.bottom() - radius2, radius2, radius2, 0, -90);
				clipPath.arcTo(r.left(), r.bottom() - radius2, radius2, radius2, 270, -90);
				clipPath.arcTo(r.left(), r.top(), radius2, radius2, 180, -90);
				painter->setClipPath(clipPath);
				QPixmap titleStretch = cached(":res/images/title_stretch.png");
				QPixmap topLeft = cached(":res/images/groupframe_topleft.png");
				QPixmap topRight = cached(":res/images/groupframe_topright.png");
				QPixmap bottomLeft = cached(":res/images/groupframe_bottom_left.png");
				QPixmap bottomRight = cached(":res/images/groupframe_bottom_right.png");
				QPixmap leftStretch = cached(":res/images/groupframe_left_stretch.png");
				QPixmap topStretch = cached(":res/images/groupframe_top_stretch.png");
				QPixmap rightStretch = cached(":res/images/groupframe_right_stretch.png");
				QPixmap bottomStretch = cached(":res/images/groupframe_bottom_stretch.png");
				QLinearGradient lg(0, 0, 0, r.height());
				
				lg.setColorAt(0, widget->palette().color(QPalette::Background));
				lg.setColorAt(1, widget->palette().color(QPalette::Light) );
				
				painter->setPen(Qt::NoPen);
				painter->setBrush(lg);
				painter->drawRect(r.adjusted(0, titleStretch.height()/2, 0, 0));
				painter->setClipping(false);

				int topFrameOffset = titleStretch.height()/2 - 2;
				painter->drawPixmap(r.topLeft() + QPoint(0, topFrameOffset), topLeft);
				painter->drawPixmap(r.topRight() - QPoint(topRight.width()-1, 0)
						+ QPoint(0, topFrameOffset), topRight);
				painter->drawPixmap(r.bottomLeft() - QPoint(0, bottomLeft.height()-1), bottomLeft);
				painter->drawPixmap(r.bottomRight() - QPoint(bottomRight.width()-1,
						bottomRight.height()-1), bottomRight);

				QRect left = r;
				left.setY(r.y() + topLeft.height() + topFrameOffset);
				left.setWidth(leftStretch.width());
				left.setHeight(r.height() - topLeft.height() - bottomLeft.height() - topFrameOffset);
				painter->drawTiledPixmap(left, leftStretch);

				QRect top = r;
				top.setX(r.x() + topLeft.width());
				top.setY(r.y() + topFrameOffset);
				top.setWidth(r.width() - topLeft.width() - topRight.width());
				top.setHeight(topLeft.height());
				painter->drawTiledPixmap(top, topStretch);

				QRect right = r;
				right.setX(r.right() - rightStretch.width()+1);
				right.setY(r.y() + topRight.height() + topFrameOffset);
				right.setWidth(rightStretch.width());
				right.setHeight(r.height() - topRight.height()
						- bottomRight.height() - topFrameOffset);
				painter->drawTiledPixmap(right, rightStretch);

				QRect bottom = r;
				bottom.setX(r.x() + bottomLeft.width());
				bottom.setY(r.bottom() - bottomStretch.height()+1);
				bottom.setWidth(r.width() - bottomLeft.width() - bottomRight.width());
				bottom.setHeight(bottomLeft.height());
				painter->drawTiledPixmap(bottom, bottomStretch);
				painter->restore();
			}
			else
			{
				QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
			}
#else
			QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
#endif
			break;
		}
		default:
			QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
			break;
	}
	
	
	return;
}

void KToonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	switch (element )
	{
		case CE_DockWidgetTitle:
		{
		}
		break;
		default:
		{
			QPlastiqueStyle::drawControl(element, option, painter, widget);
		}
		break;
	}
}

void KToonStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
	switch (control) 
	{
		case CC_Slider:
		{
			if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(option))
			{
				QRect groove = subControlRect(CC_Slider, option, SC_SliderGroove, widget);
				QRect handle = subControlRect(CC_Slider, option, SC_SliderHandle, widget);

				painter->save();

				bool hover = (slider->state & State_Enabled) && (slider->state & State_MouseOver);
				if (hover) {
					QRect moderated = widget->rect().adjusted(0, 4, 0, -4);
					drawHoverRect(painter, moderated);
				}

				if ((option->subControls & SC_SliderGroove) && groove.isValid()) {
					QPixmap grv = cached(":res/images/slider_bar.png");
					painter->drawPixmap(QRect(groove.x() + 5, groove.y(),
							groove.width() - 10, grv.height()),
					grv);
				}
				if ((option->subControls & SC_SliderHandle) && handle.isValid()) {
					QPixmap hndl = cached(":res/images/slider_thumb_on.png");
					painter->drawPixmap(handle.topLeft(), hndl);
				}

				painter->restore();
			}
			else
			{
				QPlastiqueStyle::drawComplexControl(control, option, painter, widget);
			}
			
			break;
		}
#if QT_VERSION >= 0x040100
		case CC_GroupBox:
		{
			const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option);
			if (groupBox) 
			{
				QStyleOptionGroupBox groupBoxCopy(*groupBox);
				groupBoxCopy.subControls &= ~SC_GroupBoxLabel;
				QPlastiqueStyle::drawComplexControl(control, &groupBoxCopy, painter, widget);

				if (groupBox->subControls & SC_GroupBoxLabel) 
				{
					const QRect &r = groupBox->rect;
					QPixmap titleLeft = cached(":res/images/title_cap_left.png");
					QPixmap titleRight = cached(":res/images/title_cap_right.png");
					QPixmap titleStretch = cached(":res/images/title_stretch.png");
					int txt_width = groupBox->fontMetrics.width(groupBox->text) + 20;
					painter->drawPixmap(r.center().x() - txt_width/2, 0, titleLeft);
					QRect tileRect = subControlRect(control, groupBox, SC_GroupBoxLabel, widget);
					painter->drawTiledPixmap(tileRect, titleStretch);
					painter->drawPixmap(tileRect.x() + tileRect.width(), 0, titleRight);
					int opacity = 31;
					painter->setPen(QColor(0, 0, 0, opacity));
					painter->drawText(tileRect.translated(0, 1),
							Qt::AlignVCenter | Qt::AlignHCenter, groupBox->text);
					painter->drawText(tileRect.translated(2, 1),
							Qt::AlignVCenter | Qt::AlignHCenter, groupBox->text);
					painter->setPen(QColor(0, 0, 0, opacity * 2));
					painter->drawText(tileRect.translated(1, 1),
							Qt::AlignVCenter | Qt::AlignHCenter, groupBox->text);
					painter->setPen(Qt::white);
					painter->drawText(tileRect, Qt::AlignVCenter | Qt::AlignHCenter, groupBox->text);
				}
			}
			else
			{
				QPlastiqueStyle::drawComplexControl(control, option, painter, widget);
			}
			break;
		}
#endif
		default:
			QPlastiqueStyle::drawComplexControl(control, option, painter, widget);
			break;
	}
	return;
}

QRect KToonStyle::subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
	QRect rect;

	switch (control)
	{
		default:
		{
			rect = QPlastiqueStyle::subControlRect(control, option, subControl, widget);
			break;
		}
#if QT_VERSION >= 0x040100
		case CC_GroupBox:
		{
			if (const QStyleOptionGroupBox *group = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) 
			{
				switch (subControl) 
				{
					default:
						rect = QPlastiqueStyle::subControlRect(control, option, subControl, widget);
						break;
#if QT_VERSION >= 0x040100
					case SC_GroupBoxContents:
						rect = QPlastiqueStyle::subControlRect(control, option, subControl, widget);
						rect.adjust(0, -8, 0, 0);
						break;
#endif
					case SC_GroupBoxFrame:
						rect = group->rect;
						break;
					case SC_GroupBoxLabel:
						QPixmap titleLeft = cached(":res/images/title_cap_left.png");
						QPixmap titleRight = cached(":res/images/title_cap_right.png");
						QPixmap titleStretch = cached(":res/images/title_stretch.png");
						int txt_width = group->fontMetrics.width(group->text) + 20;
						rect = QRect(group->rect.center().x() - txt_width/2 + titleLeft.width(), 0,
								txt_width - titleLeft.width() - titleRight.width(),
								titleStretch.height());
						break;
				}
			}
			break;
		}
#endif
	}

	if (control == CC_Slider && subControl == SC_SliderHandle) 
	{
		rect.setWidth(13);
		rect.setHeight(27);
	} 
	else if (control == CC_Slider && subControl == SC_SliderGroove) 
	{
		rect.setHeight(9);
		rect.moveTop(27/2 - 9/2);
	}
	return rect;
}

QSize KToonStyle::sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
	QSize newSize = QPlastiqueStyle::sizeFromContents(type, option, size, widget);


	switch (type) {

//     case CT_PushButton:
// 	    newSize.boundedTo (QSize(27, 20));
//         break;

		case CT_Slider:
			newSize.setHeight(20);
			break;

		default:
			break;
	}

	return newSize;
}

int KToonStyle::pixelMetric(PixelMetric pm, const QStyleOption *opt, const QWidget *widget) const
{
    if (pm == PM_SliderLength)
        return 13;
    return QPlastiqueStyle::pixelMetric(pm, opt, widget);
}

void KToonStyle::polish(QWidget *widget)
{
	QPlastiqueStyle::polish(widget);
    if (widget->layout() && qobject_cast<QGroupBox *>(widget)) 
    {
        if (qFindChildren<QGroupBox *>(widget).size() == 0)
            widget->layout()->setSpacing(0);
        else
            widget->layout()->setMargin(10);
    }

    if (qobject_cast<QPushButton *>(widget)
        || qobject_cast<QRadioButton *>(widget)
        || qobject_cast<QSlider *>(widget)) {
        widget->setAttribute(Qt::WA_Hover);
    }

//     QPalette pal = widget->palette();
//     if (widget->isWindow()) 
//     {
//         pal.setColor(QPalette::Background, QColor(241, 241, 241));
//         widget->setPalette(pal);
//     }

}

void KToonStyle::unpolish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
        || qobject_cast<QRadioButton *>(widget)
        || qobject_cast<QSlider *>(widget)) 
    {
        widget->setAttribute(Qt::WA_Hover, false);
    }
}

void KToonStyle::polish(QPalette &palette)
{
//     palette.setColor(QPalette::Background, QColor(241, 241, 241));
}

QRect KToonStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
	QRect r;
	switch(element) 
	{
		case SE_RadioButtonClickRect:
			r = widget->rect();
			break;
		case SE_RadioButtonContents:
			r = widget->rect().adjusted(20, 0, 0, 0);
			break;
		default:
			r = QPlastiqueStyle::subElementRect(element, option, widget);
			break;
	}

	if (qobject_cast<const QRadioButton*>(widget))
		r = r.adjusted(5, 0, -5, 0);

	return r;
}
