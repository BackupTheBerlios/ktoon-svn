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

#ifndef GLGRID_H
#define GLGRID_H

/*!
 * \file glgrid.h
 * \brief Include this file if you need the class GLGrid
 */

#include <GL/gl.h>

class QGLWidget;
class QPoint;
class QColor;
class QString;

/*!
 * \brief Class that handles the drawing area grid
 *
 * <b>Date of Creation: June 24 - 2004.</b>\n
 * This class contains information about the grid that is drawn into the drawing area. The grids are created whit 1x1 dimension
 * and scaled in the paintGL (drawingarea)
 */
class GLGrid
{

public:
    /*!
     * \brief Default constructor
     *
     * Constructs a GLGrid object
     * \param parent The parent widget
     * \param max_height The grid's maximum height
     * \param max_width The grid's maximum width
     * \param number_lines The number of lines
     * \param zoom_lines Indicates whether this grid will display a subgrid or not
     * \param ntsc_zone Indicates whether this grid will display an NTSC zone or not
     */
    GLGrid( QGLWidget *parent, const GLuint & max_height, const GLuint & max_width, const GLuint & number_lines, const bool & zoom_lines, const bool & ntsc_zone );
    /*!
     * \brief Default destructor
     *
     * Destroys the GLGrid object
     */
    ~GLGrid();

    /*!
     * \brief Draws the grid into the drawing area
     */
    void draw();

    /*!
     * \brief Draws a line from the point \a initial to the point \a final, with the color \a color and a line width
     *        of \a line_width
     *
     * \param initial The initial point
     * \param final The final point
     * \param color The line color
     * \param line_width The line width
     */
    void drawLine( const QPoint & initial, const QPoint & final, const QColor & color, const GLfloat & line_width );
    /*!
     * \brief Draws a line from the point (\a x1, \a y1 ) to the point (\a x2, \a y2 ), with the color \a color and a
     *        line width of \a line_width
     *
     * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
     * \param x1 The initial point x component
     * \param y1 The initial point y component
     * \param x2 The final point x component
     * \param y2 The final point y component
     * \param color The line color
     * \param line_width The line width
     */
    void drawLine( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, const QColor & color, const GLfloat & line_width );
    /*!
     * \brief Draws a line number into the grid
     *
     * \param initial The point where the number will be placed
     * \param color The number color
     * \param line_number The number represented by a string
     */
    void drawNumber( const QPoint & initial, const QColor & color, const QString & line_number );
    /*!
     * \brief Draws a line number into the grid
     *
     * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
     * \param x The x component of the point where the number will be placed
     * \param y The y component of the point where the number will be placed
     * \param z The z component of the point where the number will be placed
     * \param color The number color
     * \param line_number The number represented by a string
     */
    void drawNumber( GLdouble x, GLdouble y, GLdouble z, const QColor & color, const QString & line_number ) const;
    /*!
     * \brief Gets the grid id
     *
     * The grid id is used for calling the OpenGL display list that will draw the grid into the drawing area.
     * \return The grid id
     */
    GLuint idGrid() const;

    /*!
     * \brief Sets this grid's minimum width
     *
     * \param min_width The new minimum width
     * \sa minWidth()
     */
    void setMinWidth( const GLuint & min_width );
    /*!
     * \brief Sets this grid's minimum height
     *
     * \param min_height The new minimum height
     * \sa minHeight()
     */
    void setMinHeight( const GLuint & min_height );
    /*!
     * \brief Sets this grid's maximum width
     *
     * \param max_width The new maximum width
     * \sa maxWidth()
     */
    void setMaxWidth( const GLuint & max_width );
    /*!
     * \brief Sets this grid's maximum height
     *
     * \param max_height The new maximum height
     * \sa maxHeight()
     */
    void setMaxHeight( const GLuint & max_height );
    /*!
     * \brief Sets the number of lines to this grid
     *
     * \param number_lines The new number of lines
     * \sa numberLines()
     */
    void setNumberLines( const GLuint & number_lines );
    /*!
     * \brief Sets whether this grid will display numbers or not
     *
     * \param show_numbers The boolean value of the show_numbers property
     * \sa showNumbers()
     */
    void setShowNumbers( const bool & show_numbers );
    /*!
     * \brief Sets whether this grid will display a subgrid or not
     *
     * \param zoom_lines The boolean value of the zoom_lines property
     * \sa zoomLines()
     */
    void setZoomLines( const bool & zoom_lines );
    /*!
     * \brief Sets whether this grid will display an NTSC zone or not
     *
     * \param ntsc_zone The boolean value of the ntsc_zone property
     * \sa ntscZone()
     */
    void setNtscZone( const bool & ntsc_zone );
    /*!
     * \brief Sets a new grid color
     *
     * \param grid_color The new grid color
     */
    void setGridColor( const QColor & grid_color );
    /*!
     * \brief Sets a new NTSC color
     *
     * \param ntsc_color The new ntsc color
     */
    void setNTSCColor( const QColor & ntsc_color );

    /*!
     * \brief Gets this grid's minimum width
     *
     * \return The current minimum width
     * \sa setMinWidth()
     */
    GLuint minWidth() const;
    /*!
     * \brief Gets this grid's minimum height
     *
     * \return The current minimum height
     * \sa setMinHeight()
     */
    GLuint minHeight() const;
    /*!
     * \brief Gets this grid's maximum width
     *
     * \return The current maximum width
     * \sa setMaxWidth()
     */
    GLuint maxWidth() const;
    /*!
     * \brief Gets this grid's maximum height
     *
     * \return The current maximum height
     * \sa setMaxHeight()
     */
    GLuint maxHeight() const;
    /*!
     * \brief Gets this grid's number of lines
     *
     * \return The current number of lines
     * \sa setNumberLines()
     */
    GLuint numberLines() const;
     /*!
      * \brief Get this grid's show_numbers property
      *
      * \return <b>true</b> if this grid will display numbers, otherwise returns <b>false</b>
      * \sa setShowNumbers()
      */
    bool showNumbers() const;
     /*!
      * \brief Get this grid's zoom_lines property
      *
      * \return <b>true</b> if this grid will display a subgrid, otherwise returns <b>false</b>
      * \sa setZoomLines()
      */
    bool zoomLines() const;
     /*!
      * \brief Get this grid's ntsc_zone property
      *
      * \return <b>true</b> if this grid will display an NTSC zone, otherwise returns <b>false</b>
      * \sa setNtscZone()
      */
    bool ntscZone() const;

protected:
    GLuint id_grid;
    GLuint id_grid2;
    GLuint id_grid12;
    GLuint id_grid16;
    GLuint id_sub_grid12;
    GLuint id_sub_grid16;
    GLuint id_ntsc;
    GLuint id_numbers2;
    GLuint id_numbers12;
    GLuint id_numbers16;
    GLuint id_diagonals;
    GLuint id_fonts;

    /*!
     * \brief Constructs an OpenGL display list with instructions for drawing this grid
     */
    void buildGrid();
    /*!
     * \brief Constructs an OpenGL display list with instructions for drawing this grid
     *
     * This is an overloaded function provided only for convenience. It behaves essentially like the above function.
     * \param n_lines The number of lines
     * \param list The OpenGL list id
     */
    void buildGrid( int n_lines, GLuint list );
    /*!
     * \brief Constructs an OpenGL display list with instructions for drawing a subgrid within this grid
     *
     * \param n_lines The number of lines
     * \param list The OpenGL list id
     */
    void buildSubGrid( int n_lines, GLuint list );
    /*!
     * \brief Constructs an OpenGL display list with instructions for drawing an NTSC zone within this grid
     */
    void buildNtsc();
    /*!
     * \brief Constructs an OpenGL display list with instructions for drawing numbers within this grid
     *
     * \param n_lines The number of lines
     * \param list The OpenGL list id
     */
    void buildNumbers( int n_lines, GLuint list );
    /*!
     * \brief Constructs an OpenGL display list with instructions for drawing diagonal lines within this grid
     */
    void buildDiagonals();
    /*!
     * \brief Initializes the fonts which the grid numbers will be displayed with
     */
    void buildFonts();

private:
    QGLWidget *parent_widget;
    int min_width;
    int min_height;
    GLuint max_width;
    GLuint max_height;
    GLuint number_lines;
    bool show_numbers;
    bool zoom_lines;
    bool ntsc_zone;
    QColor grid_color, ntsc_color;

};

#endif
