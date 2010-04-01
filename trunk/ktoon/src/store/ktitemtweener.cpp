/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
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
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ktitemtweener.h"
#include <kcore/kdebug.h>

#include <QGraphicsItemAnimation>
#include <QGraphicsItem>
#include <QHash>

class Animator : public QGraphicsItemAnimation
{
    public:
        Animator(KTItemTweener *parent);
        ~Animator();
        
        void afterAnimationStep(qreal step);

    private:
        KTItemTweener *m_tweener;
};

Animator::Animator(KTItemTweener *parent) : QGraphicsItemAnimation(parent), m_tweener(parent)
{
}

Animator::~Animator()
{
}

void Animator::afterAnimationStep(qreal step)
{
    Q_UNUSED(step);
}

#define VERIFY_STEP(s) if (s > k->frames || k->frames == 0) { \
                           kWarning("items") << "Invalid step " << s << " for tweening, maximun step are " << k->frames << "; In " << __FUNCTION__; \
                           return; }

#define STEP(s) s / (double)k->frames

struct KTItemTweener::Private
{
    Private() : frames(0) {}
    
    QString name;
    int frames;
    //QGraphicsItem *object;
    
    Animator *animator;
    QHash<int, KTTweenerStep *> steps; // TODO: remove when Qt 4.3

    inline KTTweenerStep *step(int step)
    {
        KTTweenerStep *s = steps[step];
        if (!s) {
            s = new KTTweenerStep(step);
            steps.insert(step, s);
        }
        
        return s;
    }
};

KTItemTweener::KTItemTweener(int frames, QObject *parent) : QObject(parent), k(new Private)
{
    k->animator = new Animator(this);
    k->frames = frames;
}

QString KTItemTweener::name()
{
    return k->name;
}

KTItemTweener::~KTItemTweener()
{
    qDeleteAll(k->steps);
    delete k;
}

double KTItemTweener::horizontalScaleAt(int step) const
{
    return k->animator->horizontalScaleAt(STEP(step));
}

double KTItemTweener::horizontalShearAt(int step) const
{
    return k->animator->horizontalShearAt(STEP(step));
}

QGraphicsItem *KTItemTweener::item() const
{
    return k->animator->item();
    //return k->object;
}

QMatrix KTItemTweener::matrixAt(int step) const
{
    return k->animator->matrixAt(STEP(step));
}

QPointF KTItemTweener::posAt(int step) const
{
    return k->animator->posAt(STEP(step));
}

double KTItemTweener::rotationAt(int step) const
{
    return k->animator->rotationAt(STEP(step));
}

void KTItemTweener::setItem(QGraphicsItem *item)
{
    k->animator->setItem(item);
    //k->object = item;
}

void KTItemTweener::setPosAt(int step, const QPointF & point)
{
    VERIFY_STEP(step);
    
    k->animator->setPosAt(STEP(step), point);
    k->step(step)->setPosition(point);
}

void KTItemTweener::setRotationAt(int step, double angle)
{
    VERIFY_STEP(step);

    k->animator->setRotationAt(STEP(step), angle);
    k->step(step)->setRotation(angle);
}

void KTItemTweener::setScaleAt(int step, double sx, double sy)
{
    VERIFY_STEP(step);

    k->animator->setScaleAt(STEP(step), sx, sy);
    k->step(step)->setScale(sx, sy);
}

void KTItemTweener::setShearAt(int step, double sh, double sv)
{
    VERIFY_STEP(step);

    k->animator->setShearAt(STEP(step), sh, sv);
    k->step(step)->setScale(sh, sv);
}

void KTItemTweener::setTranslationAt(int step, double dx, double dy)
{
    VERIFY_STEP(step);

    k->animator->setTranslationAt(STEP(step), dx, dy);
    k->step(step)->setTranslation(dx, dy);
}

double KTItemTweener::verticalScaleAt(int step) const
{
    return k->animator->verticalScaleAt(STEP(step));
}

double KTItemTweener::verticalShearAt(int step) const
{
    return k->animator->verticalShearAt(STEP(step));
}

double KTItemTweener::xTranslationAt(int step) const
{
    return k->animator->xTranslationAt(STEP(step));
}

double KTItemTweener::yTranslationAt(int step) const
{
    return k->animator->yTranslationAt(STEP(step));
}

void KTItemTweener::addStep(const KTTweenerStep &step)
{
    int n = step.n();
    
    VERIFY_STEP(n);
    
    if (step.has(KTTweenerStep::Position))
        setPosAt(n, step.position());
    
    if (step.has(KTTweenerStep::Scale))
        setScaleAt(n, step.horizontalScale(), step.verticalScale());
    
    if (step.has(KTTweenerStep::Translation))
        setTranslationAt(n, step.xTranslation(), step.yTranslation());
    
    if (step.has(KTTweenerStep::Shear))
        setScaleAt(n, step.horizontalShear(), step.verticalShear());
    
    if (step.has(KTTweenerStep::Rotation))
        setRotationAt(n, step.rotation());
}

void KTItemTweener::setFrames(int frames)
{
    k->frames = frames;
}

int KTItemTweener::frames() const
{
    return k->frames;
}

void KTItemTweener::setStep(int step)
{
    VERIFY_STEP(step);
    k->animator->setStep(STEP(step));
}

void KTItemTweener::fromXml(const QString &xml)
{
    QDomDocument doc;

    if (doc.setContent(xml)) {
        QDomElement root = doc.documentElement();

        k->name = root.attribute("name");
        k->frames = root.attribute("frames").toInt();

        kFatal() << "KTItemTweener::fromXml -> " << k->name;
        
        QDomNode n = root.firstChild();
        
        while (!n.isNull()) {
               QDomElement e = n.toElement();
            
               if (!e.isNull()) {
                   if (e.tagName() == "step") {
                       QString stepDoc;
                       {
                           QTextStream ts(&stepDoc);
                           ts << n;
                       }
                    
                       KTTweenerStep *step = new KTTweenerStep(0);
                       step->fromXml(stepDoc);
                    
                       addStep(*step);
                    
                       delete step;
                   }
               }
            
               n = n.nextSibling();
        }
    }
}

QDomElement KTItemTweener::toXml(QDomDocument &doc) const
{
    kFatal() << "KTItemTweener::toXml -> " << k->name;

    QDomElement root = doc.createElement("tweening");
    root.setAttribute("name", k->name);
    root.setAttribute("frames", k->frames);
    
    foreach (KTTweenerStep *step, k->steps.values())
             root.appendChild(step->toXml(doc));
    
    return root;
}

QDomDocument KTItemTweener::tweenToXml(QString name, int frames, const QVector<KTTweenerStep *> &steps)
{
    kFatal() << "KTItemTweener::tweenToXml -> Name: " << name;

    QDomDocument doc;

    QDomElement root = doc.createElement("tweening");
    root.setAttribute("name", name);
    root.setAttribute("frames", frames);
   
    foreach (KTTweenerStep *step, steps)
             root.appendChild(step->toXml(doc));

    doc.appendChild(root);
   
    return doc;
}
