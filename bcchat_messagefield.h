/************************************************************************
 * Copyright (C) 2010 Fabian Lesniak <f.lesniak@gmx.de>                 *
 *                                                                      *
 * This file is part of BCChat.                                         *
 *                                                                      *
 * BCChat is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * BCChat is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with BCChat.  If not, see <http://www.gnu.org/licenses/>.      *
 ***********************************************************************/

#include <QtGui>

class message_field : public QTextEdit
{
Q_OBJECT

  public:
    message_field(QWidget *parent = 0);
  protected:
    virtual void keyPressEvent(QKeyEvent *event);
  signals:
    void submit();
};

class message_field_button : public QPushButton
{
Q_OBJECT

  public:
    message_field_button(QWidget *parent, QTextEdit *target_message_field);
  private:
    QTextEdit *edit;
    QMenu *menu;
    QAction *bold;
    QAction *italic;
    QAction *underline;
  public slots:
    void toggle_bold(bool value);
    void toggle_italic(bool value);
    void toggle_underline(bool value);
};
