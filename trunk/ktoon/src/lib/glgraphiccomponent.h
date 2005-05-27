/***************************************************************************
 *   Copyright (C) 2004 by Simena Dinas                                    *
 *   dsimena@toonka.com                                                    *
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

#ifndef GLGRAPHICCOMPONENT_H
#define GLGRAPHICCOMPONENT_H

/**
 * @file glgraphiccomponent.h
 * @brief Include this file if you need the class GLGraphicComponent
 */

#include <qgl.h>
#include <qcolor.h>
#include <qdom.h>

#include "color.h"
#include "brush.h"

/**
 * @brief Base class for all graphic components
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains information about a generic graphic component. It declares some virtual functions
 * to reimplement them into its subclasses.
 */
class GLGraphicComponent
{

public:
     //! Constants for defining the kind of drawing area's graphics
     enum GraphicKind
     {
         GC_BRUSH = 1, /**< A graphic that was created by the brush tool */
	 GC_PENCIL = 2, /**< A graphic that was created by the pencil tool */
	 GC_PEN = 3, /**< A graphic that was created by the pen tool */
	 GC_LINE = 4, /**< A graphic that was created by the line tool */
	 GC_RECTANGLE = 5, /**< A graphic that was created by the rectangle tool */
	 GC_ELLIPSE = 6, /**< A graphic that was created by the ellipse tool */
	 GC_BITMAP = 7, /**< A graphic that represents a bitmap */
	 GC_IMAGE = 8 /**< A graphic that represents an image imported from outside */
     };

     //! Static variable to define names for identifying graphics when a selection operation is performed
     static GLuint selection_name;

     /**
      * @brief Default constructor
      *
      * Constructs a GLGraphicComponent object
      * @param parent The parent widget
      * @param origin The origin point
      * @param color The color of this graphic component
      * @param width The brush which this graphic component was drawn with (to calculate the outline width)
      */
     GLGraphicComponent( QGLWidget *parent, const QPoint & origin, const Color & color, const Brush & width );
     /**
      * @brief Default destructor
      *
      * Destroys the GLGraphicComponent object
      */
     virtual ~GLGraphicComponent();

     /**
      * @brief Gets the graphic component id
      *
      * The graphic component id is used for calling the OpenGL display list that will draw the graphic component
      * into the drawing area.
      * @return The graphic component id
      */
     GLuint idGraphicComponent() const;

     //Set methods
     /**
      * @brief Sets a new origin point to this graphic component
      *
      * @param origin The origin point
      * @sa originPoint()
      */
     void setOriginPoint( const QPoint & origin );
     /**
      * @brief Sets a new outline color to this graphic component
      *
      * @param color The new outline color
      * @sa outlineColor()
      */
     void setOutlineColor( const Color & color );
     /**
      * @brief Sets a new width to the outline of this graphic component, according to the argument brush
      *
      * @param width The brush which this graphic component was drawn with
      * @sa widthPoint()
      */
     void setWidthPoint( const Brush & width );
     /**
      * @brief Sets whether this graphic component will contain a fill or not
      *
      * @param fill The boolean value of the fill property
      * @sa fill()
      */
     void setFill( bool fill );
     /**
      * @brief Sets an OpenGL stipple pattern to the contour of this graphic component
      *
      * It internally applies this pattern to the underlying glLine() funcion.
      * @param pattern The stipple pattern
      * @sa stipplePattern()
      */
     void setStipplePattern( GLushort pattern );
     /**
      * @brief Sets an OpenGL stipple factor to the contour of this graphic component
      *
      * It internally applies this factor to the underlying glLine() funcion.
      * @param factor The stipple factor
      * @sa stippleFactor()
      */
     void setStippleFactor( GLushort factor );
     /**
      * @brief Sets the Z coordinate for each graphic component
      *
      * @param z The z value
      * @sa getZ()
     */
     void setZ( GLfloat z );
     /**
      * @brief Sets the rotation angle for each graphic component
      *
      * @param angle_rotation The angle rotation value
      * @sa rotationAngle()
     */
     void setRotationAngle( GLfloat angle_rotation );
     /**
      * @brief Sets the top left point of the rectangle that fits outside this graphic component
      *
      * @param top_left The new top left point
      * @sa topLeft()
      */
     void setTopLeft( const QPoint & top_left );
     /**
      * @brief Sets the bottom right point of the rectangle that fits outside this graphic component
      *
      * @param bottom_right The new bottom right point
      * @sa bottomRight()
      */
     void setBottomRight( const QPoint & bottom_right );
     /**
      * @brief Sets the horizontal flip
      *
      * @sa horizontalFlip()
      */
     virtual void toggleHorizontalFlip();
     /**
      * @brief Sets the vertical flip
      *
      * @sa verticalFlip()
      */
     virtual void toggleVerticalFlip();
     /**
      * @brief Sets the spin point of this graphic component
      *
      * @param spin_point The new spin point
      * @sa spinPoint()
      */
     void setSpinPoint( const QPoint & spin_point );
     /**
      * @brief Calculates the top left point of the rectangle that fits outside this graphic component
      */
     virtual void calculateTopLeft() = 0;
     /**
      * @brief Calculates the bottom right point of the rectangle that fits outside this graphic component
      */
     virtual void calculateBottomRight() = 0;
     /*
      * @brief Draws this graphic component into the drawing area
      *
      * It simply calls a OpenGL display list, passing the graphic component id. 
      */
     virtual void draw();
     /**
      * @brief Translates this graphic component to a relative position
      *
      * The relative position is calculated from the difference between the x and y components of the argument points.
      * @param origin The initial translation point
      * @param end The final translation point
      */
     virtual void translateGraphic( const QPoint & origin, const QPoint & end );
     /**
      * @brief Translates this graphic component to a relative position
      *
      * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
      * The relative position is calculated from the difference between the x and y components of the current origin
      * position and the arguments.
      * @param x_translate The x component of the final translation point
      * @param y_translate The y component of the final translation point
      */
     virtual void translateGraphic( int x_translate, int y_translate );
     /**
      * @brief Rotates this graphic component to a relative position
      *
      * The new position is calculated between the spin point and the rotation angle.
      * @param spin The spin point of rotation
      * @param angle The rotation angle
      */
     void rotateGraphic( const QPoint & spin, float angle );
     /**
      * @brief Rotates this graphic component to a relative position
      *
      * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
      * The new position is calculated between the spin point and the rotation angle
      * @param x_rotate The x component of the spin point
      * @param y_rotate The y component of the spin point
      * @param angle The rotation angle
      */
     void rotateGraphic( float x_rotate, float y_rotate, float angle );
    /**
      * @brief flips this graphic component to a relative position
      *
      * The new position is calculated with the flip point.
      * @param flip The spin point of flip
      */
     void flipGraphic( const QPoint & flip );
     /**
      * @brief flips this graphic component to a relative position
      *
      * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
      * The new position is calculated with the flip point.
      * @param x_flip The x component of the flip point
      * @param y_flip The y component of the flip point
      */
     void flipGraphic( float x_flip, float y_flip );
     /**
      * @brief This function performs all operations of the current graphic related to the CONTOUR_SELECTION tool in the mouse press event
      */
     void contourSelectionPressOption();
     /**
      * @brief This function performs all operations of the current graphic related to the FILL tool in the mouse press event
      *
      * @param fill_color The fill color
      */
     void fillPressOption( const Color & fill_color );
     /**
      * @brief This function performs all operations of the current graphic related to the REMOVE_FILL tool in the mouse press event
      */
     void removeFillPressOption();
     /**
      * @brief This function performs all operations of the current graphic related to the CONTOUR_SELECTION tool in the mouse move event
      *
      * @param contour_selection_point The point of the movement with the CONTOUR_SELECTION tool
      */
    void contourSelectionMoveOption( const QPoint & contour_selection_point );
     /**
      * @brief This function performs all operations of the current graphic related to the BRUSH tool in the mouse move event
      *
      * @param brush_point The point of the movement with the BRUSH tool
      */
    void brushMoveOption( const QPoint & brush_point );
     /**
      * @brief This function performs all operations of the current graphic related to the PENCIL tool in the mouse move event
      *
      * @param pencil_point The point of the movement with the PENCIL tool
      */
    void pencilMoveOption( const QPoint & pencil_point );
     /**
      * @brief This function performs all operations of the current graphic related to the PEN tool in the mouse move event
      *
      * @param pen_point The point of the movement with the PENCIL tool
      */
    void penMoveOption( const QPoint & pen_point );
     /**
      * @brief This function performs all operations of the current graphic related to the Line tool in the mouse move event
      *
      * @param line_point The point of the movement with the PENCIL tool
      */
    void lineMoveOption( const QPoint & line_point );
     /**
      * @brief This function performs all operations of the current graphic related to the RECTANGLE tool in the mouse move event
      *
      * @param rectangle_point The point of the movement with the PENCIL tool
      */
    void rectangleMoveOption( const QPoint & rectangle_point );
     /**
      * @brief This function performs all operations of the current graphic related to the ELLIPSE tool in the mouse move event
      *
      * @param ellipse_point The point of the movement with the PENCIL tool
      */
    void ellipseMoveOption( const QPoint & ellipse_point );
     /**
      * @brief This function makes a fast line implementation.
      *
      * It is neccesary for non accelerated cards or the width of line implementation hardware mode is limited.
      * @param origin The origin point
      * @param end The end point
      * @param line_width The brush which this graphic component was drawn with (to calculate the outline width)
      * @param outline_color The color of this graphic component
      * @sa lineImpl()
      */
    void lineImplFast( const QPoint & origin, const QPoint & end, int line_width, const Color & outline_color );
     /**
      * @brief This function makes a line implementation with a rectangle, where width is the rectangle base.
      *
      * It is neccesary for non accelerated cards or the width of line implementation hardware mode is limited.
      * @param origin The origin point
      * @param end The end point
      * @param line_width The brush which this graphic component was drawn with (to calculate the outline width)
      * @param stipple_factor The stipple factor value
      * @param outline_color The color of this graphic component
      * @sa lineImplFast()
      */
    void lineImpl( const QPoint & origin, const QPoint & end, int line_width, int stipple_factor, const Color & outline_color );

     //Get methods
     /**
      * @brief Gets the parent widget
      *
      * @return A pointer to the parent widget
      */
     QGLWidget *parentWidget() const;
      /**
      * @brief Gets this graphic component's origin point
      *
      * @return The origin point.
      * @sa setOutlineColor()
      */
     QPoint originPoint() const;
     /**
      * @brief Gets this graphic component's outline color
      *
      * @return The outline color. Note that it is of type Color, not QColor
      * @sa setOutlineColor()
      */
     const Color & outlineColor() const;
     /**
      * @brief Gets the brush which this graphic component was drawn with
      *
      * @return The current brush
      * @sa setWidthPoint()
      */
     const Brush & widthPoint() const;
     /**
      * @brief Gets this graphic component's kind
      *
      * @return The graphic kind. See GraphicKind
      * @sa setKindGraphic()
      */
     int kindGraphic() const;
     /**
      * @brief Get this graphic component's fill property
      *
      * @return <b>true</b> if this graphic component will be filled or it's filled, otherwise returns <b>false</b>
      * @sa setFill()
      */
     bool fill() const;
     /**
      * @brief Gets the OpenGL stipple pattern of this graphic component's contour
      *
      * @return The current stipple pattern
      * @sa setStipplePattern
      */
     GLushort stipplePattern() const;
     /**
      * @brief Gets the OpenGL stipple factor of this graphic component's contour
      *
      * @return The current stipple factor
      * @sa setStippleFactor
      */
     GLushort stippleFactor() const;
     /**
      * @brief Gets the Z coordinate of this graphic component
      *
      * @return the z value
      * @sa setZ()
     */
     GLfloat getZ() const;
     /**
      * @brief Gets the rotation angle of this graphic component
      *
      * @return the rotation_angle value
      * @sa setRotationAngle()
     */
     GLfloat rotationAngle() const;
     /**
      * @brief Gets the top left point of the rectangle that fits outside this graphic component
      *
      * @return The top left point
      * @sa setTopLeft()
      */
     QPoint topLeft() const;
     /**
      * @brief Gets the bottom right point of the rectangle that fits outside this graphic component
      *
      * @return The bottom right point
      * @sa setBottomRight()
      */
     QPoint bottomRight() const;
      /**
      * @brief Gets the horizontal flip
      *
      * @return The horizontal flip
      * @sa setHorizontalFlip()
      */
     bool horizontalFlip();
      /**
      * @brief Gets the vertical flip
      *
      * @return The vertical flip
      * @sa setVerticalFlip()
      */
     bool verticalFlip();
     /**
      * @brief Gets the graphic component selection name
      *
      * The selection name is useful for identifying a single object during a selection operation.
      * @return The graphic component selection name
      */
     GLuint selectionName() const;
      /**
      * @brief Gets the trasnlate point of this graphic component
      *
      * @return The translate point
      * @sa translateGraphic()
      */
     QPoint getTranslate();
      /**
      * @brief Gets the spin point of this graphic component
      *
      * @return The spin point
      * @sa setSpinPoint()
      */
     QPoint spinPoint();
      /**
      * @brief Gets whether this graphic component is included within the selection rectangle
      *
      * @param mouse_press The input event of the mousePressEvent operation
      * @param mouse_release The input event of the mouseReleaseEvent operation
      * @return <b>true</b> if the this graphic component is included, otherwise returns <b>false</b>
      */
     bool graphicIncluded( QPoint mouse_press, QPoint mouse_release );
      /**
      * @brief Gets the translation value of this graphic
      *
      * @return The x translation value
      */
     int translationX();
       /**
      * @brief Gets the translation value of this graphic
      *
      * @return The y translation value
      */
     int translationY();
    /**
      * @brief Gets the clipboard representation of this graphic component
      *
      * @return A string that represents this graphic component's properties
      */
     virtual QString clipboardGraphic() = 0;

     /**
      * @brief Creates an XML tag for this object
      *
      * @param doc The DOM Document
      * @return The DOM Element associated
      */
     virtual QDomElement createXML( QDomDocument &doc ) = 0;
     /**
      * @brief Event for graphic key press control
      *
      * @param key_event The input event
      */
     void keyPressEvent( QKeyEvent *key_event );
     /**
      * @brief Event for graphic key release control
      *
      * @param key_event The input event
      */
     void keyReleaseEvent( QKeyEvent *key_event );

protected:
     QPoint origin;
     Color color;
     Brush width;
     int kind_graphic;
     GLuint id_graphic_component;
     bool fill_graphic;
     GLushort stipple_pattern;
     GLushort stipple_factor;
     QPoint top_left, bottom_right, spin_point;
     QPoint translate;
     GLfloat z;
     GLfloat rotation_angle;
     bool horizontal_flip, vertical_flip;
     GLuint local_selection_name;

     QGLWidget *parent_widget;

     //Variable related of the key press event
     bool shift_pressed;
     bool control_pressed;
     bool included;

     /**
      * @brief Sets this graphic to be of the kind \a kind
      *
      * @param kind The graphic kind. See GraphicKind
      * @sa kindGraphic()
      */
     void setKindGraphic( int kind );
     /**
      * @brief Constructs an OpenGL display list with instructions for drawing this graphic component
      */
     virtual void buildList() = 0;

};

#endif
