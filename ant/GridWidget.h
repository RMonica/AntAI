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

#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QGridLayout>

#include <vector>

#include "Grid.h"
#include "Ant.h"

class TQGridWidget : public QWidget
  {
  Q_OBJECT
  public:
  explicit TQGridWidget(QWidget *parent = 0);
  ~TQGridWidget();

  void DisplayGrid(const TGrid & grid);
  void DisplayAnt(uint x, uint y, uint range);
  void DisplayDir(TDirection dir);

  void paintEvent(QPaintEvent *);

  // set if the ant must be displayed or not
  void SetDisplayAnt(bool display) {hasAnt = display; }

  // pixels
  static const uint CELLWIDTH = 30;
  static const uint CELLHEIGHT = 30;
    
  signals:
    
  public slots:

  private:
  TGrid lastGrid;
  uint lastAntX,lastAntY,lastAntRange;
  TDirection lastDir;
  bool hasAnt;

  void _draw();
  };

#endif // GRIDWIDGET_H
