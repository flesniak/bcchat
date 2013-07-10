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

#include "bcchat_opts.h"

bcchat_opts::bcchat_opts(QWidget *parent)
{
setWindowTitle(tr("Options"));

QLabel *nick_label = new QLabel(this);
nick_label->setText(tr("Nickname:"));

nick_edit = new QLineEdit(this);
nick_edit->setMaxLength(255);

okay_button = new QPushButton(this);
okay_button->setText(tr("OK"));

QLabel *port_label = new QLabel(this);
port_label->setText("Port:");
port_spin = new QSpinBox(this);
port_spin->setRange(1025,65536);

QHBoxLayout *port_layout = new QHBoxLayout;
port_layout->addWidget(port_label);
port_layout->addWidget(port_spin);

QVBoxLayout *mainlayout = new QVBoxLayout(this);
mainlayout->addWidget(nick_label);
mainlayout->addWidget(nick_edit);
mainlayout->addLayout(port_layout);
mainlayout->addWidget(okay_button);

connect(okay_button,SIGNAL(clicked()),this,SLOT(close()));
}
