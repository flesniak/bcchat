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

#include "bcchat_messagefield.h"

message_field::message_field(QWidget *parent)
{
}

void message_field::keyPressEvent(QKeyEvent *event)
{
if((event->key() == Qt::Key_Return) & (event->modifiers() == Qt::NoModifier))
  emit(submit());
else
  QTextEdit::keyPressEvent(event);
}

message_field_button::message_field_button(QWidget *parent, QTextEdit *target_message_field) : QPushButton(parent)
{
edit=target_message_field;
setText(tr("Formatting"));

menu = new QMenu(this);
bold = menu->addAction(tr("Bold"));
bold->setCheckable(true);
italic = menu->addAction(tr("Italic"));
italic->setCheckable(true);
underline = menu->addAction(tr("Underlined"));
underline->setCheckable(true);

setMenu(menu);

connect(bold,SIGNAL(toggled(bool)),this,SLOT(toggle_bold(bool)));
connect(italic,SIGNAL(toggled(bool)),this,SLOT(toggle_italic(bool)));
connect(underline,SIGNAL(toggled(bool)),this,SLOT(toggle_underline(bool)));
}

void message_field_button::toggle_bold(bool value)
{
if(value)
 edit->setFontWeight(QFont::Bold);
else
 edit->setFontWeight(QFont::Normal);
edit->setFocus(Qt::OtherFocusReason);
}

void message_field_button::toggle_italic(bool value)
{
edit->setFontItalic(value);
edit->setFocus(Qt::OtherFocusReason);
}

void message_field_button::toggle_underline(bool value)
{
edit->setFontUnderline(value);
edit->setFocus(Qt::OtherFocusReason);
}
