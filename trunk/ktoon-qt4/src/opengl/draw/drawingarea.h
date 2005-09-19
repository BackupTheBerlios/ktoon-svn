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

#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

/**
 * @file drawingarea.h
 * @brief Include this file if you need the class DrawingArea
 */

#include <qmatrix.h>
#include <q3valuelist.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <Q3PopupMenu>
#include <Q3PtrList>
#include <QCloseEvent>
#include <iostream>
#include <qobject.h>

#include "glcontrol.h"
#include "glgraphiccomponent.h"
#include "glline.h"
#include "glellipse.h"
#include "glrectangle.h"
#include "glgrid.h"
#include "glpen.h"
#include "glpencil.h"
#include "glbrush.h"
#include "glimage.h"
#include "gldrawing.h"
#include "ktcolor.h"
#include "gltext.h"

class Q3PopupMenu;
class GLDrawing;
class Color;

/**
 * @brief Class that handles the dialog box with an area where it is possible to make illustrations
 *
 * <b>Date of Creation: May 25 - 2004.</b>\n
 * This is the most important widget of the illustration mode. Here, the user may create any type of drawing
 * with the help of the tools within the dialog boxes.
 */

class DrawingArea : public GLControl
{
    Q_OBJECT

    public:
        /**
        * @brief Default Constructor
        *
        * Constructs a DrawingArea object
        * @param parent The parent widget
        * @param grandparent The grandparent widget
        * @param name This object name. It is passed to the GLControl constructor
        * @param f Flags passed to the GLControl constructor
        */
        DrawingArea( QWidget* parent, const char* name = 0, Qt::WFlags f = 0 );
        /**
        * @brief Default destructor
        *
        * Destroys the DrawingArea object
        */
        ~DrawingArea();
    
        //Set methods
        /**
        * @brief Sets a new x component to the drawing area rotation
        *
        * @param x The new x component
        * @sa rotationX()
        */
        void setRotationX( const GLfloat & x );
        /**
        * @brief Sets a new y component to the drawing area rotation
        *
        * @param y The new y component
        * @sa rotationY()
        */
        void setRotationY( const GLfloat & y );
        /**
        * @brief Sets a new z component to the drawing area rotation
        *
        * @param z The new z component
        * @sa rotationZ()
        */
        void setRotationZ( const GLfloat & z) ;
        /**
        * @brief Sets a new x component to the drawing area translation
        *
        * @param x The new x component
        * @sa translationX()
        */
        void setTranslationX( const GLfloat & x );
        /**
        * @brief Sets a new y component to the drawing area translation
        *
        * @param y The new y component
        * @sa translationY()
        */
        void setTranslationY( const GLfloat & y );
        /**
        * @brief Sets a new z component to the drawing area translation
        *
        * @param z The new z component
        * @sa translationZ()
        */
        void setTranslationZ( const GLfloat & z );
        /**
        * @brief Sets a new x component to the drawing area scalation
        *
        * @param x The new x component
        * @sa scaleX()
        */
        void setScaleX( const GLfloat & x );
        /**
        * @brief Sets a new y component to the drawing area scalation
        *
        * @param y The new y component
        * @sa scaleY()
        */
        void setScaleY( const GLfloat & y );
        /**
        * @brief Sets a new z component to the drawing area scalation
        *
        * @param z The new z component
        * @sa scaleZ()
        */
        void setScaleZ( const GLfloat & z );
    
        /**
        * @brief Sets a new maximum height to this widget
        *
        * @param max_vert The new maximum height
        * @sa maxVertical()
        */
        void setMaxVertical( const GLuint & max_vert );
        /**
        * @brief Sets a new maximum width to this widget
        *
        * @param max_hor The new maximum width
        * @sa maxHorizontal()
        */
        void setMaxHorizontal( const GLuint & max_hor );
        /**
        * @brief Sets the number of lines to the drawing area's grid
        *
        * @param num_lines The new number of lines
        * @sa numberLines()
        */
        void setNumberLines( const GLuint & num_lines );
        /**
        * @brief Sets the vertical margin value to separate the grid from the top
        *
        * @param marg_vert The new vertical margin
        * @sa verticalMargin()
        */
        void setVerticalMargin( const GLuint & marg_vert );
        /**
        * @brief Sets the horizontal margin value to separate the grid from the left
        *
        * @param marg_hor The new horizontal margin
        * @sa horizontalMargin()
        */
        void setHorizontalMargin( const GLuint & marg_hor );
        /**
        * @brief Sets a new height to this widget
        *
        * @param h The new height
        * @sa height()
        */
        void setHeight( const GLuint & h );
        /**
        * @brief Sets a new width to this widget
        *
        * @param w The new width
        * @sa width()
        */
        void setWidth( const GLuint & w );
        /**
        * @brief Sets whether this drawing area's grid will display a subgrid or not
        *
        * @param zoom_lines The boolean value of the zoom_lines property
        * @sa zoomLines()
        */
        void setZoomLines( const bool & zoom_lines );
        /**
        * @brief Sets whether this drawing area's grid will display an NTSC zone or not
        *
        * @param ntsc_zone The boolean value of the ntsc_zone property
        * @sa ntscZone()
        */
        void setNtscZone( const bool & ntsc_zone );
        /**
        * @brief Sets whether this drawing area's will display all the layer drawings at the current keyframe level
        *
        * @param light_table The boolean value of the light table property
        * @sa lightTable()
        */
        void setLightTable( const bool & light_table );
        /**
        * @brief Calculates the position of the grid
        */
        void gridPosition();
        /**
        * @brief Creates a brush object from the string representation after a paste operation
        *
        * @param brush The string that describes the brush representation
        */
        void pasteBrush( QString brush );
        /**
        * @brief Creates a pencil object from the string representation after a paste operation
        *
        * @param pencil The string that describes the pencil representation
        */
        void pastePencil( QString pencil );
        /**
        * @brief Creates a pen object from the string representation after a paste operation
        *
        * @param pen The string that describes the pen representation
        */
        void pastePen( QString pen );
        /**
        * @brief Creates a line object from the string representation after a paste operation
        *
        * @param line The string that describes the line representation
        */
        void pasteLine( QString line );
        /**
        * @brief Creates a rectangle object from the string representation after a paste operation
        *
        * @param rectangle The string that describes the rectangle representation
        */
        void pasteRectangle( QString rectangle );
        /**
        * @brief Creates a ellipse object from the string representation after a paste operation
        *
        * @param ellipse The string that describes the ellipse representation
        */
        void pasteEllipse( QString ellipse );
        /**
        * @brief Adds a graphic component to this drawing area
        *
        * @param graphic_component The graphic that is going to be added
        * @param update The boolean value of the update or not
        */
        void addGraphicComponent( GLGraphicComponent * graphic_component, bool update = true );
        /**
        * @brief Sets whether this Document is modified or not
        *
        * @param modify The boolean value of the modify state
        */
        void modifyDocument( bool modify = true );
        /**
        * @brief Loads the specified image into the drawing area
        *
        * @param file_name The image file name
        */
        void loadImage( const QString &file_name );
        /**
        * @brief Loads the specified image sequence into the drawing area
        *
        * @param file_name The image sequence file name
        */
        void loadImageSequence( const QString &file_name );
    
        //Get methods
        /**
        * @brief Gets the x component from the drawing area rotation
        *
        * @return x The current x component
        * @sa setRotationX()
        */
        GLfloat rotationX() const;
        /**
        * @brief Gets the y component from the drawing area rotation
        *
        * @return y The current y component
        * @sa setRotationY()
        */
        GLfloat rotationY() const;
        /**
        * @brief Gets the z component from the drawing area rotation
        *
        * @return z The current z component
        * @sa setRotationZ()
        */
        GLfloat rotationZ() const;
        /**
        * @brief Gets the x component from the drawing area translation
        *
        * @return x The current x component
        * @sa setTranslationX()
        */
        GLfloat translationX() const;
        /**
        * @brief Gets the y component from the drawing area translation
        *
        * @return y The current y component
        * @sa setTranslationY()
        */
        GLfloat translationY() const;
        /**
        * @brief Gets the z component from the drawing area translation
        *
        * @return z The current z component
        * @sa setTranslationZ()
        */
        GLfloat translationZ() const;
        /**
        * @brief Gets the x component from the drawing area scalation
        *
        * @return x The current x component
        * @sa setScaleX()
        */
        GLfloat scaleX() const;
        /**
        * @brief Gets the y component from the drawing area scalation
        *
        * @return y The current y component
        * @sa setScaleY()
        */
        GLfloat scaleY() const;
        /**
        * @brief Gets the z component from the drawing area scalation
        *
        * @return z The current z component
        * @sa setScaleZ()
        */
        GLfloat scaleZ() const;
        /**
        * @brief Gets this widget's maximum height
        *
        * @return The current maximum height of the grid
        * @sa setMaxVertical()
        */
        GLuint maxVertical() const;
        /**
        * @brief Gets this widget's maximum width  of the grid
        *
        * @return The current maximum width
        * @sa setMaxHorizontal()
        */
        GLuint maxHorizontal() const;
        /**
        * @brief Gets the drawing area's grid number of lines
        *
        * @return The current number of lines
        * @sa setNumberLines()
        */
        GLuint numberLines() const;
        /**
        * @brief Gets the vertical margin value
        *
        * @return The current vertical margin of the grid
        * @sa setVerticalMargin()
        */
        GLuint verticalMargin() const;
        /**
        * @brief Gets the horizontal margin value
        *
        * @return The current horizontal margin of the grid
        * @sa setHorizontalMargin()
        */
        GLuint horizontalMargin() const;
        /**
        * @brief Gets this widget's height
        *
        * @return The current height of the drawing area
        * @sa setHeight()
        */
        GLuint height() const;
        /**
        * @brief Gets this widget's width
        *
        * @return The current width of the drawing area
        * @sa setWidth()
        */
        GLuint width() const;
        /**
        * @brief Gets the drawing area grid's zoom_lines property
        *
        * @return <b>true</b> if this grid will display a subgrid, otherwise returns <b>false</b>
        * @sa setZoomLines()
        */
        bool zoomLines() const;
        /**
        * @brief Gets the drawing area grid's ntsc_zone property
        *
        * @return <b>true</b> if this grid will display an NTSC zone, otherwise returns <b>false</b>
        * @sa setNtscZone()
        */
        bool ntscZone() const;
        /**
        * @brief Gets the drawing area's light table property
        *
        * @return <b>true</b> if this drawing area will display all the layer drawings at the current keyframe level, otherwise returns <b>false</b>
        * @sa setLightTable()
        */
        bool lightTable() const;
        /**
        * @brief Gets the current graphic component
        *
        * @return A pointer to the current graphic
        */
        GLGraphicComponent *currentGraphic() const;
        /**
        * @brief Gets this drawing area's graphic list
        *
        * @return The currently displayed list of graphics
        */
        Q3PtrList<GLGraphicComponent> graphicList() const;
		 
	bool isModified();

    protected:
        /**
        * @brief Initializes the OpenGL rendering context
        *
        * For a detailed description see the QGLWidget documentation.
        */
        void initializeGL();
        /**
        * @brief This function is called whenever this widget has been resized
        *
        * For a detailed description see the QGLWidget documentation.
        * @param w The new width
        * @param h The new height
        */
        void resizeGL( int w, int h );
        /**
        * @brief This function is called whenever this widget needs to be painted
        *
        * For a detailed description see the QGLWidget documentation.
        */
        void paintGL();
        /**
        * @brief Takes the frontmost graphic component and selects it
        *
        * @param hits Number of clicked objects
        * @param buffer The selection buffer
        */
        void processHits ( GLint hits, GLuint buffer[] );
        /**
        * @brief Allows to select graphic components by means of a mouse event
        *
        * @param mouse_event The input event
        */
        void drawSelected( QMouseEvent *mouse_event );
        /**
        * @brief Allows to select graphic components contained into two mouse events
        *
        * @overload
        * @param mouse_press The input event of the mousePressEvent operation
        * @param mouse_release The input event of the mouseReleaseEvent operation
        */
        void drawSelected( QPoint mouse_press, QPoint mouse_release );
        /**
        * @brief Draws previous Onion Skins into the DrawingArea, according to the current onion skin value
        */
        void drawOnionSkinsAndLightTable();
        /**
        * @brief Event for dialog box mouse press control
        *
        * Reimplemented from GLControl.
        * @param mouse_event The input event
        */
        void mousePressEvent( QMouseEvent *mouse_event );
        /**
        * @brief Event for dialog box mouse move control
        *
        * Reimplemented from GLControl.
        * @param mouse_event The input event
        */
        void mouseMoveEvent( QMouseEvent *mouse_event );
        /**
        * @brief Event for dialog box mouse release control
        *
        * Reimplemented from GLControl.
        * @param mouse_event The input event
        */
        void mouseReleaseEvent( QMouseEvent *mouse_event );
        /**
        * @brief Event for dialog mouse double click control
        *
        * Reimplemented from GLControl.
        * @param mouse_event The input event
        */
        void mouseDoubleClickEvent( QMouseEvent *mouse_event );
        /**
        * @brief Event for dialog box key press control
        *
        * Reimplemented from QWidget.
        * @param key_event The input event
        */
        void keyPressEvent( QKeyEvent *key_event );
        /**
        * @brief Event for dialog box key release control
        *
        * Reimplemented from QWidget.
        * @param key_event The input event
        */
        void keyReleaseEvent( QKeyEvent *key_event );
        /**
        * @brief Event for dialog box closing control
        *
        * Reimplemented from QWidget.
        * @param close_event The input event
        */
        void closeEvent( QCloseEvent *close_event );
        /**
        * @brief Event for dialog box mouse wheel control
        *
        * Reimplemented from QWidget.
        * @param wheel_event The input event
        */
        void wheelEvent( QWheelEvent *wheel_event );
        /**
        * @brief Captures the color of the graphic at the point \a pick_point of this drawing area
        *
        * If there is no graphic at the point \a pick_point , this function returns the screen color at that point.
        * @param pick_point A pick point into the drawing area
        * @return The color of the graphic at that point
        */
	KTColor *grabColor( const QPoint & pick_point );
        /**
        * @brief Load the Texture to select the current graphic in selection mode
        *
        * @param filename The path of the texture file
        * @return The texture's display list identifier
        */
        GLuint loadTexture( const char *filename );
        /**
        * @brief Creates the display list for the arrows when a graphic has been selected
        *
        * @param id_texture The texture's display list identifier
        * @return The display list identifier of the rectangle that draws the textures
        */
        GLuint createSelectionArrows( GLuint id_texture );
        /**
        * @brief Marks a selected graphic with arrows
        */
        void markSelected();
        /**
        * @brief Marks a set of selected graphics with a rectangle
        *
        * @overload
        * @param mouse_press The input event of the mousePressEvent operation
        * @param mouse_release The input event of the mouseReleaseEvent operation
        */
        void markSelected( QPoint mouse_press, QPoint mouse_release );
        /**
        * @brief Creates the matrix inverse related to the accummulated transformations into the OpenGL's projection matrix
        *
        * It calculates the matrix inverse with the Gauss-Jordan Elimination method
        */
        void invertMatrix();
        /**
        * @brief converts the OpenGL's projection matrix ( is a vector of 16 positions ) in a matrix of 4*4 position
        *
        * It converts a vector of 16 elements in a matrix transpose of 4x4 elements. This operation is needed by invertMatrix()
        */
        void convertToMatrix();
        /**
        * @brief Maps a point to the actual coordinates
        *
        * It takes a point and map it to the inverse transpose matrix
        * @param point_to_map The point to map
        * @return The mapped point
        */
        QPoint mapPointToMatrix( const QPoint & point_to_map );
    
        //Variables related to the current keyframe
        Q3PtrList<GLGraphicComponent> graphic_list;
        Q3PtrList<GLGraphicComponent> current_graphic_list;
        
        Q3PtrList<GLGraphicComponent> history_graphic_list;
        
        GLGraphicComponent *current_graphic;
    
        int current_cursor;
	KTColor *current_outline_color;
	KTColor *current_fill_color;
        Brush *current_brush;
    
        //Variables related to the previous onion skin
        Q3PtrList<GLGraphicComponent> previous_onion_skin_1;
        Q3PtrList<GLGraphicComponent> previous_onion_skin_2;
        Q3PtrList<GLGraphicComponent> previous_onion_skin_3;
    
        //Variables related to the next onion skin
        Q3PtrList<GLGraphicComponent> next_onion_skin_1;
        Q3PtrList<GLGraphicComponent> next_onion_skin_2;
        Q3PtrList<GLGraphicComponent> next_onion_skin_3;
    
        //Variable related to the light Table
        Q3PtrList<GLGraphicComponent> light_table_list;
    
        //Variable related to the copy/paste operations.
        QStringList clipboard;
    
        //Variables related to the texture
        QRgb *texture;
        GLuint id_texture;
        GLuint id_spin;
        GLuint id_arrow_texture[9];
        GLuint id_spin_texture;
        GLuint id_arrow_names[9];
    
    
    private:
        //Variables related to the grid, that is used as a guide to draw.
        GLuint max_vertical;
        GLuint max_horizontal;
        GLuint vertical_margin;
        GLuint horizontal_margin;
        GLuint number_lines;
        bool zoom_lines;
        bool front_grid;
        bool ntsc_zone;
        bool light_table;
    
        //Variable related to closing documents
        bool modified;
    
        //Variable that indicates that a pen drawing has not been finished
        bool bezier;
    
        //Variable that indicates that all drawings into the drawing area have been selected
        bool selected_all;
    
        //Variables related to the temporary activation of the hand tool
        bool using_hand;
        int previous_cursor;
    
        //Variables related to the window handling
        GLuint h;
        GLuint w;
    
        //The grid
        GLGrid *grid;
    
        //Variables related to the OpenGL's transformations
        GLfloat rotation_x, rotation_y, rotation_z;
        GLfloat translation_x, translation_y, translation_z;
        GLfloat scale_x, scale_y, scale_z;
    
        //Variable for handling the selection buffer
        GLuint selectionBuffer[1024];
    
        //Variable to activate the selection marks
        bool selected_graphic;
    
        //Matrices for OpenGL transformation Variables
        QMatrix m_invert;
        GLdouble matrix[16];
        GLdouble mmatrix[4][4];
    
        //Variable for the right click menu
        Q3PopupMenu *right_click_menu;
    
        Q3ValueList<int> id_images;
    
        //Variable to handle the shift press event
        bool shift_pressed;

    signals:
        /**
        * This signal is emitted when the DrawingArea is selected.
        */
        void selected();
        /**
        * This signal is emitted when the DrawingArea is closed.
        */
        void closed();
        /**
        * This signal is emitted when a color has been picked from the screen.
        * @param c The picked color
        */
	void colorGrabbed( KTColor *c );
        /**
        * This signal is emitted when paintGL is called
        */
        void updated();
	
        /**
        * This signal is emitted when the graphic list is not empty
        * @param state It is <b>true</b> if the current keyframe has graphics, otherwise  it is <b>false</b>
        */
        void wasDrawn( bool state );
			     
	void useTool(int);
    	
	void mousePos(QPoint);
    
    public slots:
        /**
        * @brief Changes the outline color when the current outline color was changed from the color palette
        */
        void slotChangeOutlineColor();
        /**
        * @brief Changes the fill color when the current fill color was changed from the color palette
        */
        void slotChangeFillColor();
        /**
        * @brief Changes the brush minimum thickness when the it was changed from the brushes dialog
        */
        void slotChangeMinThicknessBrush();
        /**
        * @brief Changes the brush maximum thickness when the it was changed from the brushes dialog
        */
        void slotChangeMaxThicknessBrush();
        /**
        * @brief Changes the brush smoothness when the it was changed from the brushes dialog
        */
        void slotChangeSmoothnessBrush();
        /**
        * @brief Performs a "Zoom In" on the drawing area
        */
        void slotZoomIn();
        /**
        * @brief Performs a "Zoom Out" on the drawing area
        */
        void slotZoomOut();
        /**
        * @brief Shows the drawing area without a grid
        */
        void slotNoGrid();
        /**
        * @brief Shows a 12 Field Grid
        */
        void slotSeeGrid12();
        /**
        * @brief Shows a 16 Field Grid
        */
        void slotSeeGrid16();
        /**
        * @brief Shows a Grid with a 3 Field subgrid
        */
        void slotSeeSubgrid();
        /**
        * @brief Sends the grid to the front or to the back of the drawing area
        */
        void slotFrontBackGrid();
        /**
        * @brief Shows or hides the guidelines for NTSC zone
        */
        void slotSeeNTSC();
        /**
        * @brief Activates or deactivates the light table
        */
        void slotLightTable();
        /**
        * @brief Rotates the scene 30 degrees clockwise
        */
        void slotRotateClockwise();
        /**
        * @brief Rotates the scene 30 degrees counterclockwise
        */
        void slotRotateCounterClockwise();
        /**
        * @brief Restores any previous rotations performed to the scene
        */
        void slotRestoreRotation();
        /**
        * @brief Fits the drawing area into the application in a way that it is possible to view the whole of it
        */
        void slotFit();
        /**
        * @brief Undoes the last action
        */
        void slotUndo();
        /**
        * @brief Redoes an action (after executing an "undo" action)
        */
        void slotRedo();
        /**
        * @brief Cuts the selection and puts it onto the clipboard
        */
        void slotCut();
        /**
        * @brief Copies the selection and puts it onto the clipboard
        */
        void slotCopy();
        /**
        * @brief Pastes the clipboard into the drawing area
        */
        void slotPaste();
        /**
        * @brief Pastes the clipboard into the drawing area, in the location where the copied object was
        */
        void slotPasteInPlace();
        /**
        * @brief Deletes the selected objects
        */
        void slotDelete();
        /**
        * @brief selects all objects into the drawing area
        */
        void slotSelectAll();
        /**
        * @brief Updates the drawing area according to the selected frame
        */
        void slotSelectFrame();
        /**
        * @brief Disable previous Onion Skins
        */
        void slotNoPreviousOnionSkin();
        /**
        * @brief See only the previous Onion Skin
        */
        void slotPreviousOnionSkin();
        /**
        * @brief See the previous two Onion Skins
        */
        void slotPrevious2OnionSkin();
        /**
        * @brief See the previous three Onion Skins
        */
        void slotPrevious3OnionSkin();
        /**
        * @brief Disable next Onion Skins
        */
        void slotNoNextOnionSkin();
        /**
        * @brief See only the next Onion Skin
        */
        void slotNextOnionSkin();
        /**
        * @brief See the next two Onion Skins
        */
        void slotNext2OnionSkin();
        /**
        * @brief See the next three Onion Skins
        */
        void slotNext3OnionSkin();
        /**
        * @brief Makes a group from the selected objects of the drawing area
        */
        void slotGroup();
        /**
        * @brief Ungroups the previous grouped elements
        */
        void slotUngroup();
        /**
        * @brief Puts the selected object over all the objects in the drawing area
        */
        void slotBringToFront();
        /**
        * @brief Sends the selected object to the drawing area's background
        */
        void slotSendToBack();
        /**
        * @brief Moves the selected object one step forward into the drawing area
        */
        void slotOneStepForward();
        /**
        * @brief Moves the selected object one step backward into the drawing area
        */
        void slotOneStepBackward();
        /**
        * @brief Aligns the selection to the drawing area's left border
        */
        void slotAlignLeft();
        /**
        * @brief Centers the selection vertically into the drawing area
        */
        void slotCenterVertically();
        /**
        * @brief Aligns the selection to the drawing area's right border
        */
        void slotAlignRight();
        /**
        * @brief Aligns the selection to the drawing area's top border
        */
        void slotAlignTop();
        /**
        * @brief Centers the selection horizontally into the drawing area
        */
        void slotCenterHorizontally();
        /**
        * @brief Aligns the selection to the drawing area's bottom border
        */
        void slotAlignBottom();
        /**
        * @brief Reflects the selection in the drawing area - horizontal axis
        */
        void slotFlipHorizontally();
        /**
        * @brief Reflects the selection in the drawing area - vertical axis
        */
        void slotFlipVertically();
        /**
        * @brief Rotates the selected elements 90 degrees to the right
        */
        void slotRotateCW90();
        /**
        * @brief Rotates the selected elements 90 degrees to the left
        */
        void slotRotateCCW90();
        /**
        * @brief Rotates the selected elements 180 degrees
        */
        void slotRotate180();
        /**
        * @brief Activates a special Tool for transformations
        */
        void slotPerspectiveSelection();
        /**
        * @brief Shows a dialog box to set graphic component properties
        */
        void slotProperties();
        /**
        * @brief Adds the current graphic component to the object library
        */
        void slotAddToLibrary();
        /**
        * @brief Modifies the Grid color
        */
        void slotSetGridColor();
        /**
        * @brief Modifies the NTSC color
        */
        void slotSetNTSCColor();

};

#endif
