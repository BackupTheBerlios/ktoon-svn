//
// C++ Interface: tweenertranslate
//
// Description: 
//
//
// Author: Jorge Cuadrado <kuadrosx@toonka.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TWEENERTRANSLATE_H
#define TWEENERTRANSLATE_H

#include <kttoolplugin.h>

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 * plugin experimental
*/

class TweenerTranslate : public KTToolPlugin
{
	Q_OBJECT;
	public:
		TweenerTranslate();
		virtual ~TweenerTranslate();
		virtual void init(KTGraphicsScene *scene);

		virtual QStringList keys() const;
		virtual void press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);
		virtual void release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene);

		virtual QMap<QString, DAction *>actions() const;

		int toolType() const;

		virtual QWidget *configurator();

		virtual void aboutToChangeTool();
		
		bool isComplete() const;
		
	private:
		void setupActions();
	
	private:
		struct Private;
		Private *const d;
		
	public slots:
		void setCreatePath();
		void setSelect();
		void applyTweener();
		
};

#endif
