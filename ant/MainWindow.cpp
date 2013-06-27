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

#include "MainWindow.h"

#include <QGridLayout>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QApplication>
#include <QPushButton>
#include <QFileDialog>

#include "RecorderWidget.h"
#include "Arff.h"
#include "AntTreeAI.h"
#include "AntNeuralAI.h"
#include "AntPolishFunctionAI.h"

TQMainWindow::TQMainWindow(QWidget *parent):QWidget(parent)
  {
  srand(time(0)); // randomize

  mapWidget = new TQGridWidget(this);
  lastMoveWidget = new TQGridWidget(this);
  curMoveWidget = new TQGridWidget(this);
  scoreLabel = new QLabel(this);

  QVBoxLayout * lastMoveLayout = new QVBoxLayout();
  lastMoveLayout->addWidget(new QLabel("Last move:",this));
  lastMoveLayout->addWidget(lastMoveWidget);

  QVBoxLayout * curMoveLayout = new QVBoxLayout();
  curMoveLayout->addWidget(new QLabel("Next move:",this));
  curMoveLayout->addWidget(curMoveWidget);

  TQRecorderWidget * recorderWidget = new TQRecorderWidget(this);

  QVBoxLayout * aiLoaderLayout = new QVBoxLayout();
  QPushButton * aiRandomButton = new QPushButton("Load random AI",this);
  connect(aiRandomButton,SIGNAL(clicked()),this,SLOT(DoLoadRandomAI()));
  aiRandomButton->setFocusPolicy(Qt::NoFocus);
  aiLoaderLayout->addWidget(aiRandomButton);
  QPushButton * aiTreeButton = new QPushButton("Load tree AI...",this);
  connect(aiTreeButton,SIGNAL(clicked()),this,SLOT(DoLoadTreeAI()));
  aiTreeButton->setFocusPolicy(Qt::NoFocus);
  aiLoaderLayout->addWidget(aiTreeButton);
  QPushButton * aiNeuralButton = new QPushButton("Load neural AI...",this);
  connect(aiNeuralButton,SIGNAL(clicked()),this,SLOT(DoLoadNeuralAI()));
  aiNeuralButton->setFocusPolicy(Qt::NoFocus);
  aiLoaderLayout->addWidget(aiNeuralButton);
  QPushButton * aiPolishFunctionButton = new QPushButton("Load prefix function AI...",this);
  connect(aiPolishFunctionButton,SIGNAL(clicked()),this,SLOT(DoLoadPolishFunctionAI()));
  aiPolishFunctionButton->setFocusPolicy(Qt::NoFocus);
  aiLoaderLayout->addWidget(aiPolishFunctionButton);

  displayAIEdit = new QTextEdit(this);
  displayAIEdit->setFocusPolicy(Qt::NoFocus);

  QGridLayout * layout = new QGridLayout(this);
  setLayout(layout);
  layout->addWidget(mapWidget,0,0,2,1);
  layout->addLayout(lastMoveLayout,1,1,1,1);
  layout->addLayout(aiLoaderLayout,1,2,1,1);
  layout->addLayout(curMoveLayout,1,3,1,1);
  layout->addWidget(scoreLabel,2,0);
  layout->addWidget(recorderWidget,0,1,1,4);
  layout->addWidget(displayAIEdit,3,0,1,5);

  map = new TGrid(GAME_MAP_SIZE_X,GAME_MAP_SIZE_Y,0);
  ant = new TQAntAdapter(this,0,0,GAME_ANT_VISUAL_RANGE,*map);
  lastMoveWidget->DisplayAnt(ant->GetVisualRange(),ant->GetVisualRange(),ant->GetVisualRange());
  curMoveWidget->DisplayAnt(ant->GetVisualRange(),ant->GetVisualRange(),ant->GetVisualRange());
  //normMoveWidget->DisplayAnt(ant->GetVisualRange(),ant->GetVisualRange(),ant->GetVisualRange());

  recorderWidget->SetHeaderString(QString(NArff::GenerateHeader(*ant).c_str()));
  connect(ant,SIGNAL(antMoved(QString)),recorderWidget,SLOT(DoRecordNewLine(QString)));

  displayAIEdit->setText(ant->GetAIInfo().c_str());

  PrepareMap();
  }

TQMainWindow::~TQMainWindow()
  {
  delete ant;
  delete map;
  }

void TQMainWindow::keyPressEvent(QKeyEvent * event)
  {
  switch (event->key())
    {
    case Qt::Key_Right:
      ant->DoMove(DIR_EAST);
      break;
    case Qt::Key_Up:
      ant->DoMove(DIR_NORTH);
      break;
    case Qt::Key_Down:
      ant->DoMove(DIR_SOUTH);
      break;
    case Qt::Key_Left:
      ant->DoMove(DIR_WEST);
      break;
    case ' ':
      ant->DoMoveAuto();
      break;
    case 'N':
    case 'n':
      PrepareMap();
      break;
    case 'Q':
    case 'q':
      QApplication::quit();
      break;
    }

  QWidget::keyPressEvent(event);
  UpdateAll();
  }

void TQMainWindow::UpdateAll()
  {
  mapWidget->DisplayGrid(*map);
  mapWidget->DisplayAnt(ant->GetX(),ant->GetY(),ant->GetVisualRange());
  lastMoveWidget->DisplayGrid(ant->GetLastMove());
  lastMoveWidget->DisplayDir(ant->GetLastMove().GetChoice());
  curMoveWidget->DisplayGrid(ant->GetNeighborhood());
  curMoveWidget->DisplayDir(ant->Predict());

  /*
  TMove normalized = ant->GetLastMove().Normalize();
  normMoveWidget->DisplayGrid(normalized);
  normMoveWidget->DisplayDir(normalized.GetChoice());
  */

  scoreLabel->setText("Score: " + QString().setNum(ant->GetPoints()) + " Moves: " +
    QString().setNum(ant->GetMoves()));
  }

void TQMainWindow::PrepareMap()
  {
  uint x;
  uint y;


  for (uint i = 0; i < GAME_MAP_SIZE_Y; i++)
    for (uint h = 0; h < GAME_MAP_SIZE_X; h++)
      (*map)[i][h] = 0;

  for (uint i = 0; i < GAME_FOOD_PIECES; i++)
    {
    x = rand() % GAME_MAP_SIZE_X;
    y = rand() % GAME_MAP_SIZE_Y;
    (*map)[y][x]++;
    }

  x = rand() % GAME_MAP_SIZE_X;
  y = rand() % GAME_MAP_SIZE_Y;
  // do 20 attempts to find an empty spot
  for (uint i = 0; i < 20; i++)
    if ((*map)[y][x] != 0)
      {
      x = rand() % GAME_MAP_SIZE_X;
      y = rand() % GAME_MAP_SIZE_Y;
      }
      else
        break;

  ant->ResetPosition(x,y);

  UpdateAll();
  }

void TQMainWindow::DoLoadTreeAI()
  {
  QString str = QFileDialog::getOpenFileName(this,"Load tree AI");
  if (str == "") // cancel pressed
    return;

  ant->SetAI(new TAntTreeAI(str.toAscii().data()));
  displayAIEdit->setText(ant->GetAIInfo().c_str());

  UpdateAll();
  }

void TQMainWindow::DoLoadNeuralAI()
  {
  QString str = QFileDialog::getOpenFileName(this,"Load neural AI");
  if (str == "") // cancel pressed
    return;

  ant->SetAI(new TAntNeuralAI(str.toAscii().data()));
  displayAIEdit->setText(ant->GetAIInfo().c_str());

  UpdateAll();
  }

void TQMainWindow::DoLoadRandomAI()
  {
  ant->SetAI(new TRandomAntAI());
  displayAIEdit->setText(ant->GetAIInfo().c_str());

  UpdateAll();
  }

void TQMainWindow::DoLoadPolishFunctionAI()
  {
  QString str = QFileDialog::getOpenFileName(this,"Load polish notation function AI");
  if (str == "") // cancel pressed
    return;

  ant->SetAI(new TAntPolishFunctionAI(str.toAscii().data()));
  displayAIEdit->setText(ant->GetAIInfo().c_str());

  UpdateAll();
  }
