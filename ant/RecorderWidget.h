/*
 * Copyright (C) 2012 Riccardo Monica
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

#ifndef RECORDERWIDGET_H
#define RECORDERWIDGET_H

#include <QWidget>
#include <QLabel>

#include <string>

class TQRecorderWidget : public QWidget
  {
  Q_OBJECT
  public:
  explicit TQRecorderWidget(QWidget *parent = 0);

  // set the file header
  void SetHeaderString(QString head) {headerString = head; }
    
  signals:
    
  public slots:
  void DoClear();
  void DoBrowse();
  void DoRecordNewLine(QString line);

  private:
  QLabel * fileNameLabel;
  QString currentFileName;

  QString headerString;
  };

#endif // RECORDERWIDGET_H
