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

class stat_update_thread : public QThread
{
Q_OBJECT

  public:
    void run();
  signals:
    void update();
};

class bcchat_stat : public QWidget
{
Q_OBJECT

  public:
    bcchat_stat(QWidget *parent = 0);
    quint32 sent, recv, msg_sent, msg_recv;
    quint64 size_sent, size_recv;
    stat_update_thread *ut;
  private:
    QPushButton *close_button;
    QLabel *sent_label;
    QLabel *recv_label;
    QLabel *sent_msg_label;
    QLabel *recv_msg_label;
    QLabel *sent_size_label;
    QLabel *recv_size_label;
  public slots:
    virtual void show();
    virtual void close();
    void update_labels();
};
