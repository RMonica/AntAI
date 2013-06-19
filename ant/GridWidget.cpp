/*
 * Copyright (C) 2012-2013 Riccardo Monica
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GridWidget.h"

#include <QPainter>

TQGridWidget::TQGridWidget(QWidget *parent):QWidget(parent)
  {
  lastAntX = 0;
  lastAntY = 0;
  lastAntRange = 1;
  hasAnt = true;
  lastDir = DIR_NONE;
  }

TQGridWidget::~TQGridWidget()
  {
  }

void TQGridWidget::DisplayGrid(const TGrid & grid)
  {
  lastGrid = grid;
  setMinimumSize(grid.GetWidth() * CELLWIDTH,grid.GetHeight() * CELLHEIGHT);

  update();
  }

void TQGridWidget::DisplayAnt(uint x, uint y, uint range)
  {
  lastAntX = x;
  lastAntY = y;
  lastAntRange = range;

  update();
  }

void TQGridWidget::DisplayDir(TDirection dir)
  {
  lastDir = dir;

  update();
  }

void TQGridWidget::_draw()
  {
  QPainter painter(this);

  uint width = lastGrid.GetWidth();
  uint height = lastGrid.GetHeight();
  painter.fillRect(0,0,width * CELLWIDTH,height * CELLHEIGHT,Qt::cyan);

  // display the board
  for (uint i = 0; i < height; i++)
    for (uint h = 0; h < width; h++)
      {
      sint value = lastGrid[i][h];

      // change color of the cell if needed
      // dangerous cell
      if (value < 0)
        painter.fillRect(h * CELLWIDTH,i * CELLHEIGHT,CELLWIDTH,CELLHEIGHT,Qt::red);

      // food cell
      if (value > 0)
        painter.fillRect(h * CELLWIDTH,i * CELLHEIGHT,CELLWIDTH,CELLHEIGHT,Qt::yellow);
      }

  // draw the ant if any
  if (hasAnt)
    {
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(lastAntX * CELLWIDTH + CELLWIDTH / 4,lastAntY * CELLHEIGHT + CELLWIDTH / 4,
      CELLWIDTH / 2,CELLHEIGHT / 2);

    // visual range
    sint minx = sint(lastAntX) - lastAntRange;
    sint miny = sint(lastAntY) - lastAntRange;
    sint maxx = sint(lastAntX) + lastAntRange;
    sint maxy = sint(lastAntY) + lastAntRange;
    // boundaries
    if (minx < 0)
      minx = 0;
    if (miny < 0)
      miny = 0;
    if (maxx >= sint(width))
      maxx = width-1;
    if (maxy >= sint(height))
      maxy = height-1;
    minx *= CELLWIDTH;
    maxx *= CELLWIDTH;
    miny *= CELLHEIGHT;
    maxy *= CELLHEIGHT;

    painter.drawRect(minx,miny,maxx-minx+CELLWIDTH-1,maxy-miny+CELLHEIGHT-1);
    }

  // show a green circle on the edge of last direction
  if (lastDir >= 0 && lastDir < MAX_DIR)
    {
    painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
    sint diffx = DIRECTION_X_MOD[lastDir];
    sint diffy = DIRECTION_Y_MOD[lastDir];

    // move to the center of the cell, then add diff
    sint posx = lastAntX * CELLWIDTH + (CELLWIDTH / 2) + diffx * (CELLWIDTH / 2);
    sint posy = lastAntY * CELLHEIGHT + (CELLHEIGHT / 2) + diffy * (CELLHEIGHT / 2);

    painter.drawEllipse(posx - CELLWIDTH / 4 - 1,posy - CELLHEIGHT / 4 - 1,
      CELLWIDTH / 2,CELLHEIGHT / 2);
    }

  painter.setBrush(Qt::NoBrush);
  // display the numbers above all
  for (uint i = 0; i < height; i++)
    for (uint h = 0; h < width; h++)
      {
      sint value = lastGrid[i][h];

      // write the value of the cell
      painter.drawText(h * CELLWIDTH,i * CELLHEIGHT,CELLWIDTH,CELLHEIGHT,Qt::AlignHCenter | Qt::AlignVCenter,
        QString().setNum(value));
      }
  }

void TQGridWidget::paintEvent(QPaintEvent * event)
  {
  _draw();

  QWidget::paintEvent(event);
  }
