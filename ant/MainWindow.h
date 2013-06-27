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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QTextEdit>

#include "GridWidget.h"
#include "Ant.h"
#include "GameDefines.h"
#include "AntAdapter.h"

class TQMainWindow : public QWidget
  {
  Q_OBJECT
    
  public:
  TQMainWindow(QWidget *parent = 0);
  ~TQMainWindow();

  void keyPressEvent(QKeyEvent * event);

  public slots:
  void DoLoadTreeAI();
  void DoLoadRandomAI();
  void DoLoadNeuralAI();
  void DoLoadPolishFunctionAI();

  private:
  TQGridWidget * mapWidget;
  TQGridWidget * lastMoveWidget;
  TQGridWidget * curMoveWidget;

  QTextEdit * displayAIEdit;

  //TQGridWidget * normMoveWidget;

  TQAntAdapter * ant;
  TGrid * map;

  QLabel * scoreLabel;

  void UpdateAll();

  void PrepareMap();
  };

#endif // MAINWINDOW_H
