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

#include "RecorderWidget.h"

#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

TQRecorderWidget::TQRecorderWidget(QWidget *parent):QWidget(parent)
  {
  QGridLayout * layout = new QGridLayout(this);
  setLayout(layout);

  QLabel * recorderLabel = new QLabel("Recorder",this);
  layout->addWidget(recorderLabel,0,0,1,2);

  fileNameLabel = new QLabel(this);
  layout->addWidget(fileNameLabel,1,0,1,2);

  QPushButton * clearButton = new QPushButton(this);
  clearButton->setText("Clear");
  clearButton->setFocusPolicy(Qt::NoFocus);
  connect(clearButton,SIGNAL(clicked()),this,SLOT(DoClear()));
  layout->addWidget(clearButton,2,0);

  QPushButton * browseButton = new QPushButton(this);
  browseButton->setText("Browse...");
  browseButton->setFocusPolicy(Qt::NoFocus);
  connect(browseButton,SIGNAL(clicked()),this,SLOT(DoBrowse()));
  layout->addWidget(browseButton,2,1);
  }

void TQRecorderWidget::DoClear()
  {
  if (currentFileName == "")
    return; // file name not set

  QFile * cfile = new QFile(currentFileName,this);
  if (!cfile->open(QIODevice::WriteOnly) || !cfile->isWritable())
    {
    delete cfile;
    QMessageBox::warning(this,"Error","Couldn't open file for writing:\n"+currentFileName);
    return;
    }

  cfile->write(headerString.toAscii());

  delete cfile;
  }

void TQRecorderWidget::DoBrowse()
  {
  QString str = QFileDialog::getSaveFileName(this,"Append to file");
  if (str == "") // cancel pressed
    return;
  currentFileName = str;
  fileNameLabel->setText(str);
  }

void TQRecorderWidget::DoRecordNewLine(QString line)
  {
  if (currentFileName == "")
    return; // file name not set

  QFile * cfile = new QFile(currentFileName,this);
  if (!cfile->open(QIODevice::Append) || !cfile->isWritable())
    {
    delete cfile;
    QMessageBox::warning(this,"Error","Couldn't open file for writing:\n"+currentFileName);
    return;
    }

  cfile->write((line + "\n").toAscii());

  delete cfile;
  }
