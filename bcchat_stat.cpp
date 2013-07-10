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

#include "bcchat_stat.h"

bcchat_stat::bcchat_stat(QWidget *parent)
{
setWindowTitle(tr("Statistics"));

sent = 0;
recv = 0;
msg_sent = 0;
msg_recv = 0;
size_sent = 0;
size_recv = 0;

QLabel *sent_prelabel = new QLabel(this);
QLabel *recv_prelabel = new QLabel(this);
QLabel *sent_msg_prelabel = new QLabel(this);
QLabel *recv_msg_prelabel = new QLabel(this);
QLabel *sent_size_prelabel = new QLabel(this);
QLabel *recv_size_prelabel = new QLabel(this);

sent_label = new QLabel(this);
recv_label = new QLabel(this);
sent_msg_label = new QLabel(this);
recv_msg_label = new QLabel(this);
sent_size_label = new QLabel(this);
recv_size_label = new QLabel(this);

/*sent_prelabel->setText(tr("Packets sent:"));
recv_prelabel->setText(tr("Packets received:"));
sent_msg_prelabel->setText(tr("thereof messages:"));
recv_msg_prelabel->setText(tr("thereof messages:"));
sent_size_prelabel->setText(tr("Data sent:"));
recv_size_prelabel->setText(tr("Data received:"));*/

close_button = new QPushButton(this);
close_button->setText(tr("Close"));

/*QVBoxLayout *prelabels = new QVBoxLayout;
prelabels->addWidget(sent_prelabel);
prelabels->addWidget(sent_msg_prelabel);
prelabels->addWidget(recv_prelabel);
prelabels->addWidget(recv_msg_prelabel);
prelabels->addWidget(sent_size_prelabel);
prelabels->addWidget(recv_size_prelabel);

QVBoxLayout *labels = new QVBoxLayout;
labels->addWidget(sent_label);
labels->addWidget(sent_msg_label);
labels->addWidget(recv_label);
labels->addWidget(recv_msg_label);
labels->addWidget(sent_size_label);
labels->addWidget(recv_size_label);

QHBoxLayout *labelbox = new QHBoxLayout;
labelbox->addLayout(prelabels);
labelbox->addLayout(labels);

QVBoxLayout *mainlayout = new QVBoxLayout(this);
mainlayout->addLayout(labelbox);
mainlayout->addWidget(close_button);*/

QFormLayout *form = new QFormLayout;
form->addRow(tr("Packets sent:"),sent_label);
form->addRow(tr("thereof messages:"),sent_msg_label);
form->addRow(tr("Packets received:"),recv_label);
form->addRow(tr("thereof messages:"),recv_msg_label);
form->addRow(tr("Data sent:"),sent_size_label);
form->addRow(tr("Data received:"),recv_size_label);

QVBoxLayout *mainlayout = new QVBoxLayout(this);
mainlayout->addLayout(form);
mainlayout->addWidget(close_button);

ut = new stat_update_thread;

connect(close_button,SIGNAL(clicked()),this,SLOT(close()));
connect(ut,SIGNAL(update()),this,SLOT(update_labels()));
}

void bcchat_stat::show()
{
ut->start();
QWidget::show();
}

void bcchat_stat::close()
{
ut->quit();
QWidget::close();
}

void bcchat_stat::update_labels()
{
sent_label->setText(QString::number(sent));
recv_label->setText(QString::number(recv));
sent_msg_label->setText(QString::number(msg_sent));
recv_msg_label->setText(QString::number(msg_recv));
sent_size_label->setText(QString::number(size_sent)+" B");
recv_size_label->setText(QString::number(size_recv)+" B");
}

void stat_update_thread::run()
{
while(1)
  {
  emit update();
  sleep(1);
  }
}
