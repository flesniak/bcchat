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
#include <QtNetwork>

#include "bcchat_opts.h"
#include "bcchat_stat.h"
#include "bcchat_messagefield.h"

const char version[] = "2.1";

class bcchat : public QWidget
{
Q_OBJECT

  public:
    bcchat();
    ~bcchat();
  private:
    QUdpSocket *socket;
    QTextEdit *messages;
    QListWidget *client_list;
    message_field *message;
    message_field_button *message_button;
    QPushButton *send_button;
    QPushButton *options_button;
    QPushButton *stat_button;
    QLabel *nick_label;
    bcchat_stat *stat_win;
    QString nickname;
    QHash<QHostAddress,QString> ipmap;
    int port; //int instead of short for QSpinBox compat
    void send_packet(char mode, QList<QHostAddress> targets = QList<QHostAddress>(), QString value = QString());
    void send_packet(char mode, QHostAddress target, QString value = QString());
    void client_add(char mode, QHostAddress address, QString name = QString());
    void add_message(char type, QHostAddress address, QString text = QString());
  public slots:
    void receive_message();
    void send_message(QHostAddress target = QHostAddress());
    void check_send_button();
    void show_options_dlg();
    void display_info();
};
