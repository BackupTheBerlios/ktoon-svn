/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                   *
 *                                                                         *
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

#include "ktpaintarea.h"

#include "ddebug.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QPolygon>
#include <QApplication>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QClipboard>
#include <QMenu>

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"

#include "ktprojectresponse.h"

#include "ktpaintareaevent.h"
#include "ktpaintarearotator.h"
#include "ktimagedevice.h"

#include "dcore/dconfig.h"
#include "dgui/dapplication.h"

#include <cmath>

#include "kttextitem.h"

#include "librarydialog.h"
#include "ktlibraryobject.h"
#include "ktrequestbuilder.h"

#include "dosd.h"


#ifdef QT_OPENGL_LIB

#include <QGLWidget>
#include <QGLFramebufferObject>

class GLDevice : public QGLWidget
{
	public:
		GLDevice() : QGLWidget()
		{
			makeCurrent();
		}
		~GLDevice() {};
		
	protected:
		void initializeGL()
		{
			glDisable(GL_DEPTH_TEST);
		}
};

#endif

#include "ktscene.h"
#include "ktproject.h"

struct KTPaintArea::Private
{
	QGraphicsRectItem *grid;
	KTToolPlugin *tool;
	
	bool isDrawing;
	
	KTBrushManager *brushManager;
	KTInputDeviceInformation *inputInformation;
	
	KTProject *project;
	
	QRectF drawingRect;
	
	int currentSceneIndex;
	
	bool drawGrid;
	double angle;
	
	KTPaintAreaRotator *rotator;
	
	QStringList copiesXml;
};



KTPaintArea::KTPaintArea(KTProject *project, QWidget * parent) : QGraphicsView(parent), d(new Private)
{
	d->grid = 0;
	d->tool = 0;
	d->isDrawing = false;
	d->project = project;
	d->currentSceneIndex = 0;
	d->drawGrid = false;
	d->angle = 0;
	
	d->brushManager = new KTBrushManager(this);
	d->rotator = new KTPaintAreaRotator(this, this);
	d->inputInformation = new KTInputDeviceInformation(this);
	
	d->drawingRect = QRectF(QPointF(0,0), QSizeF( 500, 400 ) ); // FIXME: configurable
	
	setScene(0);
	setCurrentScene( 0 );
	if ( scene() )
	{
		qobject_cast<KTScene *>(scene())->setCurrentFrame( 0, 0 );
	}
	
	centerDrawingArea();
	
	setUseOpenGL( false );
	setInteractive ( true );
	
	restoreState();
}

void KTPaintArea::saveState()
{
	DConfig *config = dApp->config( "PaintArea" );
	config->setValue("RenderHints", int(renderHints()));
	
}

void KTPaintArea::restoreState()
{
	DConfig *config = dApp->config( "PaintArea" );
	
	int renderHints = config->value("RenderHints", int(this->renderHints())).toInt();
	setRenderHints(QPainter::RenderHints(renderHints));
	
}

KTPaintArea::~KTPaintArea()
{
	saveState();
	delete d;
}

void KTPaintArea::setCurrentScene(int index)
{
	KTScene *sscene = d->project->scene(index);
	if ( sscene )
	{
		d->currentSceneIndex = index;
		
		sscene->clean();
		
		sscene->setItemIndexMethod(QGraphicsScene::NoIndex);
		setScene(sscene);
		
		setBackgroundBrush(Qt::gray);
		
		sscene->setSceneRect( d->drawingRect );
		
		if ( KTScene *prev = dynamic_cast<KTScene *>(scene()) )
		{
			sscene->setCurrentFrame( prev->currentFrameIndex(), prev->currentLayerIndex());
		}
		else
		{
			if ( sscene->layers().count() > 0 )
			{
				sscene->setCurrentFrame(0, 0);
			}
		}
		
		if ( d->tool )
		{
			d->tool->init( this );
		}
		
		sscene->drawCurrentPhotogram();
	}
	else
	{
		setDragMode(QGraphicsView::NoDrag);
		setScene(0);
	}
}

void KTPaintArea::setAntialiasing(bool use)
{
#ifdef QT_OPENGL_LIB
	if ( QGLWidget *gl = dynamic_cast<QGLWidget *>(viewport() ) )
	{
		gl->setUpdatesEnabled(false); // works better
		gl->setFormat(QGLFormat(QGL::SampleBuffers | QGL::HasOverlay /*| QGL::DirectRendering | QGL::AccumBuffer | QGL::Rgba */));
		gl->setUpdatesEnabled(true);
	}
#endif
	
	setRenderHint(QPainter::Antialiasing, use);
	setRenderHint(QPainter::TextAntialiasing, use);
}

void KTPaintArea::setUseOpenGL(bool opengl)
{
	D_FUNCINFO << opengl;
	
	QCursor cursor(Qt::ArrowCursor);
	if ( viewport() )
	{
		cursor = viewport()->cursor();
	}
	
#ifdef QT_OPENGL_LIB
	if ( opengl )
	{
		setViewport(new GLDevice());
	}
	else
	{
		setViewport( new KTImageDevice() );
	}
#else
	Q_UNUSED(opengl);
	dWarning() << tr("OpenGL isn't supported");
#endif
	
	// to restore the cursor.
	if ( viewport() )
	{
		viewport()->setCursor(cursor);
		viewport()->setAcceptDrops(true);
	}
}

void KTPaintArea::setDrawGrid(bool draw)
{
	d->drawGrid = draw;
// 	resetCachedContent();
	viewport()->update();
}

void KTPaintArea::setTool(KTToolPlugin *tool )
{
	if ( !scene() ) return;
	
	if ( d->tool )
	{
		d->tool->aboutToChangeTool();
		disconnect(d->tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
	}
	
	d->tool = tool;
	d->tool->init(this);
	
	connect(d->tool,SIGNAL(requested(const KTProjectRequest *)), this, SIGNAL(requestTriggered( const KTProjectRequest* )));
}

bool KTPaintArea::drawGrid() const
{
	return d->drawGrid;
}

void KTPaintArea::mousePressEvent ( QMouseEvent * event )
{
	if ( !canPaint() ) return;
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	
	delete eventMapped;
	
	if ( event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		d->isDrawing = false;
	}
	else if (d->tool )
	{
		KTScene *sscene = qobject_cast<KTScene *>(scene());
		
		if ( event->buttons() == Qt::LeftButton && !sscene->currentFrame()->isLocked() )
		{
			QGraphicsView::mousePressEvent(event);
			
			d->tool->begin();
			
			d->isDrawing = true;
			d->tool->press(d->inputInformation, d->brushManager, sscene, this );
		}
		else if ( event->buttons() == Qt::RightButton )
		{
			QMenu *menu = new QMenu(tr("Drawing area"));
			
			menu->addAction(dApp->findGlobalAction("undo"));
			menu->addAction(dApp->findGlobalAction("redo"));
			
			menu->addSeparator();
			
			QAction *cut = menu->addAction(tr("Cut"), this, SLOT(cutItems()));
			
			QAction *copy = menu->addAction(tr("Copy"), this, SLOT(copyItems()));
			
			QAction *paste = menu->addAction(tr("Paste"), this, SLOT(pasteItems()));
			
			menu->addSeparator();
			QAction *del = menu->addAction(tr("Delete"), this, SLOT(deleteItems()));
			
			menu->addSeparator();
			
			/*QAction *addToLib = */menu->addAction(tr("Add to library..."), this, SLOT(addSelectedItemsToLibrary()));
			
			if ( scene()->selectedItems().isEmpty() )
			{
				del->setEnabled(false);
				cut->setEnabled(false);
				copy->setEnabled(false);
			}
			
			if ( d->copiesXml.isEmpty() )
			{
				paste->setEnabled(false);
			}
			
			if ( QMenu *toolMenu = d->tool->menu() )
			{
				menu->addSeparator();
				menu->addMenu(toolMenu);
			}
			
			menu->exec(event->globalPos());
			
// 			QGraphicsView::mousePressEvent(event);
		}
	}
}


void KTPaintArea::mouseDoubleClickEvent( QMouseEvent *event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	
	if (d->tool)
	{
		d->tool->doubleClick( d->inputInformation,  qobject_cast<KTScene *>(scene()), this );
	}
	
	delete eventMapped;
}

void KTPaintArea::mouseMoveEvent ( QMouseEvent * event )
{
	if ( !canPaint()) return;
	
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	// Rotate
	if(  !d->isDrawing && event->buttons() == Qt::LeftButton &&  (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier)))
	{
		setUpdatesEnabled(false);
		
		
		setDragMode (QGraphicsView::NoDrag);
		
		QPointF p1 = event->pos();
		QPointF p2 = d->drawingRect.center();
		
		QPointF dp = p1 - p2;
		
		if(dp.x() != 0)
		{
			double a =  atan(dp.y() / dp.x())*(180/M_PI);
			if(dp.x() < 0)
			{
				a += 180;
			}
			d->rotator->rotateTo( (int)a );
		}
		
		setUpdatesEnabled(true);
	}
	else if (d->tool && d->isDrawing )
	{
		d->tool->move(d->inputInformation, d->brushManager,  qobject_cast<KTScene *>(scene()), this );
	}
	
	emit cursorPosition( mapToScene( event->pos() ) );
	
	delete eventMapped;
	
	QGraphicsView::mouseMoveEvent(event);
}

void KTPaintArea::mouseReleaseEvent(QMouseEvent *event)
{
	if ( !canPaint()) return;
	QMouseEvent *eventMapped = mapMouseEvent( event );
	
	d->inputInformation->updateFromMouseEvent( eventMapped );
	
	if ( d->tool && d->isDrawing )
	{
		KTScene *currentScene = qobject_cast<KTScene *>(scene());
		d->tool->release(d->inputInformation, d->brushManager,  currentScene, this );
		
		
		d->tool->end();
	}
	
	delete eventMapped;
	
	d->isDrawing = false;
	
	QGraphicsView::mouseReleaseEvent(event);
}

void KTPaintArea::tabletEvent ( QTabletEvent * event )
{
	d->inputInformation->updateFromTabletEvent( event );
	
	QGraphicsView::tabletEvent(event );
}


QMouseEvent *KTPaintArea::mapMouseEvent(QMouseEvent *event) const
{
	QMouseEvent *e = new QMouseEvent(event->type(), this->mapToScene(event->pos()).toPoint(), this->mapToScene( event->globalPos() ).toPoint(), event->button(), event->buttons(), event->modifiers () );
	
	return e;
}

void KTPaintArea::frameResponse(KTFrameResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			setCurrentScene( event->sceneIndex() );
			
			if ( !scene() ) return;
			
			KTScene *sscene = dynamic_cast<KTScene *>(scene());
			
			sscene->setCurrentFrame(event->layerIndex(), event->frameIndex());
			sscene->drawPhotogram(event->frameIndex());
			
			if ( event->action() == KTProjectRequest::Select )
			{
				if ( d->tool ) d->tool->init( this );
			}
			
			dDebug("paintarea") << "frame: " << event->frameIndex() << " " << "layer: " << event->layerIndex();
		}
		break;
		case KTProjectRequest::Lock:
		{
			KTScene *sscene = dynamic_cast<KTScene *>(scene());
			
			if ( sscene->currentFrameIndex() == event->frameIndex() )
			{
				viewport()->update();
			}
		}
		break;
		default:
		{
		}
		break;
	}
	
	if ( d->tool )
	{
		d->tool->init( this );
	}
}

void KTPaintArea::layerResponse(KTLayerResponse *event)
{
	if ( !scene() ) return;
	
	KTScene *sscene = dynamic_cast<KTScene *>(scene());
	if( event->action() == KTProjectRequest::View)
	{
		sscene->layers()[event->layerIndex()]->setVisible(event->arg().toBool());
	}
	
	if ( event->action() != KTProjectRequest::Add ||  event->action() != KTProjectRequest::Remove )
	{
		qobject_cast<KTScene *>(scene())->drawCurrentPhotogram();
		viewport()->update(scene()->sceneRect().toRect() );
	}
	
	
	
}

void KTPaintArea::sceneResponse(KTSceneResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			
			setCurrentScene( event->sceneIndex() );
			
		}
		break;
		case KTProjectRequest::Remove:
		{
			if ( event->sceneIndex() == d->currentSceneIndex )
			{
				setCurrentScene( d->currentSceneIndex-1 );
			}
		}
		break;
		default: break;
	}
}


void KTPaintArea::itemResponse(KTItemResponse *event)
{
	switch(event->action())
	{
// 		case KTProjectRequest::Add:
// 		{
// 		}
// 		break;
// 		case KTProjectRequest::Remove:
// 		{
// 			
// 		}
// 		break;
		case KTProjectRequest::Transform:
		{
			viewport()->update();
		}
		break;
		default:
		{
			qobject_cast<KTScene *>(scene())->drawCurrentPhotogram();
			viewport()->update(scene()->sceneRect().toRect() );
		}
	}
	
	if ( d->tool )
	{
		d->tool->itemResponse( event );
		d->tool->init(this);
	}
}

void KTPaintArea::projectResponse(KTProjectResponse *)
{
}

void KTPaintArea::libraryResponse(KTLibraryResponse *request)
{
	Q_UNUSED(request);
}

void KTPaintArea::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsView::drawBackground(painter, rect);
	
	painter->save();
	
	bool hasAntialiasing = painter->renderHints() & QPainter::Antialiasing;
	
	painter->setRenderHint(QPainter::Antialiasing, false);
	
	painter->setPen( QPen(QColor(0,0,0,180), 3) );
	painter->fillRect( d->drawingRect, Qt::white );
	painter->drawRect( d->drawingRect );
	
	if ( d->drawGrid )
	{
		int sx = (int)painter->matrix().m11();
		int sy = (int)painter->matrix().m22();
		
		painter->resetMatrix();
		painter->scale(sx, sy);
		painter->setPen( QPen(QColor(0,0,180, 50), 1) );
		
		for(int i = 0; i < qMax(width(), height()); i+= 10 )
		{
			painter->drawLine(i, 0, i, height() );
			painter->drawLine(0, i, width(), i );
		}
	}
	
	painter->setRenderHint(QPainter::Antialiasing, hasAntialiasing);
	
	painter->restore();
}

void KTPaintArea::drawForeground( QPainter *painter, const QRectF &rect )
{
	if ( KTScene *sscene = qobject_cast<KTScene *>(scene()) )
	{
		if ( KTFrame *frame = sscene->currentFrame() )
		{
			if ( frame->isLocked() )
			{
				painter->fillRect(rect, QColor(201,201,201, 200));
				
				painter->setFont(QFont("Arial", 30) );
				QFontMetricsF fm(painter->font());
				QString text = tr("Locked");
				
				painter->drawText(QPointF(sscene->sceneRect().topRight().x() - fm.width(text), (sscene->sceneRect().topRight().y() + fm.height()) / 2), text);
			}
		}
	}
}

bool KTPaintArea::canPaint() const
{
	if ( ! scene() ) return false;
	
	if ( KTScene *sscene = dynamic_cast<KTScene *>(scene()) )
	{
		if (sscene->currentFrameIndex() >= 0 && sscene->currentLayerIndex() >= 0) return true;
	}
	
	return false;
}

void KTPaintArea::centerDrawingArea()
{
// 	ensureVisible(d->drawingRect, (width() - d->drawingRect.width()) /2, (height() - d->drawingRect.height()) /2  );
	centerOn(d->drawingRect.center());
}


void KTPaintArea::deleteItems()
{
// 	D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems ();
	
	if(!selecteds.empty())
	{
		QString strItems= "";
		
		KTScene* currentScene = static_cast<KTScene*>(scene());
// 		int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->indexOf(item), KTProjectRequest::Remove );
				emit requestTriggered(&event);
			}
			
			/*foreach(QGraphicsItem *item, selecteds)
			{
				int indexOfItem = currentScene->currentFrame()->indexOf(item);
				if(indexOfItem != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(indexOfItem);
						firstItem = indexOfItem;
					}
					else
					{
						strItems += " , "+ QString::number(indexOfItem);
					}
				}
			}
			strItems+= ")";
			if(strItems != ")")
			{
				KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, KTProjectRequest::Remove, strItems );
				
				dDebug(2) << "Deleting frame: " << currentScene->currentFrameIndex() << "\n" << "Items: " << strItems ;
				emit requestTriggered(&event);
		}*/
		}
	}
}

void KTPaintArea::groupItems()
{
// 	D_FUNCINFO;
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
		QString strItems= "";
		
		KTScene* currentScene = static_cast<KTScene*>(scene());
		int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				if(currentScene->currentFrame()->indexOf(item) != -1)
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->indexOf(item)) ;
						firstItem = currentScene->currentFrame()->indexOf(item);
					}
					else
					{
						strItems += " , "+ QString::number(currentScene->currentFrame()->indexOf(item));
					}
				}
			}
			strItems+= ")";
		}
		if(strItems != ")")
		{
			KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, KTProjectRequest::Group, strItems );
			emit requestTriggered(&event);
		}
	}
}

void KTPaintArea::ungroupItems()
{
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
// 		QString strItems= "";
		
		KTScene* currentScene = static_cast<KTScene*>(scene());
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->indexOf(item), KTProjectRequest::Ungroup);
				emit requestTriggered(&event);
			}
		}
		/*int firstItem = -1;
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				if(currentScene->currentFrame()->indexOf(item) != -1 && qgraphicsited->cast<QGraphicsItemGroup *>(item) )
				{
					if(strItems.isEmpty())
					{
						strItems +="("+ QString::number(currentScene->currentFrame()->indexOf(item)) ;
						firstItem = currentScene->currentFrame()->indexOf(item);
					}
					else
					{
						strItems += " , "+ QString::number(currentScene->currentFrame()->indexOf(item));
					}
				}
			}
			strItems+= ")";
		}
		if(strItems != ")")
		{
			KTProjectRequest event = KTRequestBuilder::createItemRequest( currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), firstItem, KTProjectRequest::Ungroup, strItems );
			emit requestTriggered(&event);
	}*/
	}
}

void KTPaintArea::copyItems()
{
	D_FUNCINFO;
	d->copiesXml.clear();
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	if(!selecteds.isEmpty())
	{
		KTScene* currentScene = static_cast<KTScene*>(scene());
// 		int firstItem = -1;
		
		if(currentScene)
		{
			foreach(QGraphicsItem *item, selecteds)
			{
				QDomDocument orig;
				orig.appendChild(dynamic_cast<KTAbstractSerializable *>(item)->toXml( orig ));
				d->copiesXml << orig.toString();
				
				// Paint it to clipbard
				QPixmap toPixmap(item->boundingRect().size().toSize());
				toPixmap.fill(Qt::transparent);
				
				QPainter painter(&toPixmap);
				painter.setRenderHint(QPainter::Antialiasing);
				
				QStyleOptionGraphicsItem opt;
				
				opt.state = QStyle::State_None;
				
				if (item->isEnabled())
					opt.state |= QStyle::State_Enabled;
				if (item->hasFocus())
					opt.state |= QStyle::State_HasFocus;
				if (item == currentScene->mouseGrabberItem())
					opt.state |= QStyle::State_Sunken;
				
				opt.exposedRect = item->boundingRect();
				opt.levelOfDetail = 1;
				opt.matrix = item->sceneMatrix();
				opt.palette = palette();
				
				item->paint(&painter, &opt, this);
				
				painter.end();
				
				QApplication::clipboard()->setPixmap(toPixmap);
			}
		}
	}
	
}

void KTPaintArea::pasteItems()
{
	D_FUNCINFO;
	KTScene* currentScene = static_cast<KTScene*>(scene());
	
	foreach(QString xml, d->copiesXml)
	{
		KTProjectRequest event = KTRequestBuilder::createItemRequest(currentScene->index(), currentScene->currentLayerIndex(), currentScene->currentFrameIndex(), currentScene->currentFrame()->graphics().count(), KTProjectRequest::Add, xml);
		
		emit requestTriggered(&event);
	}
}

void KTPaintArea::cutItems()
{
	D_FUNCINFO;
	copyItems();
	deleteItems();
}

void KTPaintArea::wheelEvent(QWheelEvent *event)
{
	if(event->modifiers () == Qt::ControlModifier)
	{
		scaleView(pow(( double)2, -event->delta() / 240.0));
	}
	else
	{
		QGraphicsView::wheelEvent(event);
	}
}

void KTPaintArea::keyPressEvent(QKeyEvent *event)
{
	if ( d->tool )
	{
		d->tool->keyPressEvent(event);
		
		if ( event->isAccepted() )
		{
			return;
		}
	}
	
	QGraphicsView::keyPressEvent(event);
}

void KTPaintArea::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}

void KTPaintArea::setRotationAngle(int angle)
{
	rotate(angle - d->angle );
	d->angle = angle;
}

KTBrushManager *KTPaintArea::brushManager() const
{
	return d->brushManager;
}

void KTPaintArea::setNextFramesOnionSkinCount(int n)
{
	if ( KTScene* currentScene = static_cast<KTScene*>(scene()) )
	{
		currentScene->setNextOnionSkinCount( n );
	}
}

void KTPaintArea::setPreviousFramesOnionSkinCount(int n)
{
	if ( KTScene* currentScene = static_cast<KTScene*>(scene()) )
	{
		currentScene->setPreviousOnionSkinCount( n );
	}
}

void KTPaintArea::addSelectedItemsToLibrary()
{
	dDebug("paintarea") << "Adding to library";
	
	QList<QGraphicsItem *> selecteds = scene()->selectedItems();
	
	if ( selecteds.isEmpty() )
	{
		DOsd::self()->display(tr("No items selected"), DOsd::Error);
		return;
	}
	
	LibraryDialog dialog;
	foreach (QGraphicsItem *item, selecteds )
	{
		dialog.addItem( item );
	}
	
	if ( dialog.exec() != QDialog::Accepted )
	{
		return;
	}
	
	foreach (QGraphicsItem *item, selecteds )
	{
		if( KTAbstractSerializable *itemSerializable = dynamic_cast<KTAbstractSerializable *>(item) )
		{
			QString symName = dialog.symbolName( item );
			
			QDomDocument doc;
			doc.appendChild(itemSerializable->toXml(doc));
			
			
			KTProjectRequest request = KTRequestBuilder::createLibraryRequest(KTProjectRequest::Add, symName,  doc.toString().toLocal8Bit(), KTLibraryObject::Item);
			
			emit requestTriggered(&request);
		}
	}
}


