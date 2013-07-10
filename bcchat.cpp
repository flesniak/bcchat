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

#include "bcchat.h"

// ########## MAIN WINDOW CONSTRUCTOR ##########

bcchat::bcchat()
{
setWindowTitle(tr("BCChat - Broadcastchat"));

//Network stuff
port = 54545;

socket = new QUdpSocket(this);
if(!socket->bind(port)) {
  QMessageBox::critical(this,tr("Error!"),tr("Unable to open port ")+QString::number(port)+tr(". Program already running?"));
  exit(9); }

//Stat window
stat_win = new bcchat_stat(this);

//Messages field
messages = new QTextEdit(this);
messages->setReadOnly(true);
//Clients field
client_list = new QListWidget(this);
client_list->setMaximumWidth(180);
//Message field
message = new message_field(this);
message->setMaximumHeight(80);
message->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
message->setFocus(Qt::OtherFocusReason);
//Buttons
message_button = new message_field_button(this,message);

//Nickname stuff
QString localuser;
#ifdef Q_WS_WIN
localuser=getenv("username");
#else
localuser=getenv("USER");
#endif
if(localuser.isEmpty())
  localuser=tr("Unnamed");
nickname = localuser+QString::fromUtf8("@")+QHostInfo::localHostName();

//Button
send_button = new QPushButton(this);
send_button->setText(tr("Send Message"));

//Labels
QLabel *msg_label = new QLabel(this);
msg_label->setText(tr("Message:"));
QLabel *clients_label = new QLabel(this);
clients_label->setText(tr("Chat buddies"));
nick_label = new QLabel(this);
nick_label->setText(tr("Nickname: ")+nickname);

//Menubar
QMenuBar *menubar = new QMenuBar(this);
QMenu *chat_menu = menubar->addMenu(tr("C&hat"));
QAction *options_action = chat_menu->addAction(QIcon::fromTheme("configure"),tr("O&ptions"));
QAction *quit_action = chat_menu->addAction(QIcon::fromTheme("application-exit"),tr("Q&uit"));
quit_action->setShortcuts(QKeySequence::Quit);
QMenu *extras_menu = menubar->addMenu(tr("E&xtras"));
QAction *stats_action = extras_menu->addAction(tr("D&isplay stats"));
QAction *info_action = extras_menu->addAction(QIcon::fromTheme("help-about"),tr("A&bout"));

QGridLayout *layout = new QGridLayout(this);
layout->setMenuBar(menubar);
layout->addWidget(messages,0,0,2,3);
layout->addWidget(clients_label,0,3);
layout->addWidget(client_list,1,3);
layout->addWidget(msg_label,2,0);
layout->addWidget(message_button,2,1);
layout->addWidget(send_button,2,2);
layout->addWidget(nick_label,2,3);
layout->addWidget(message,3,0,3,4);
layout->setRowStretch(1,1);
layout->setColumnStretch(0,1);

connect(socket,SIGNAL(readyRead()),this, SLOT(receive_message()));
connect(message,SIGNAL(textChanged()),this,SLOT(check_send_button()));
connect(message,SIGNAL(submit()),this,SLOT(send_message()));
connect(send_button,SIGNAL(clicked()),this,SLOT(send_message()));
connect(options_action,SIGNAL(triggered()),this,SLOT(show_options_dlg()));
connect(quit_action,SIGNAL(triggered()),this,SLOT(close()));
connect(stats_action,SIGNAL(triggered()),stat_win,SLOT(show()));
connect(info_action,SIGNAL(triggered()),this,SLOT(display_info()));

send_packet(0);
}

bcchat::~bcchat()
{
send_packet(3,ipmap.keys());
}

/* Message Types (pretty similar to packet magicks)
0: Print Nickname, prepend date & Print Message
1: Print Client going online
2: Print Client changing nick
3: Print Client going offline */

void bcchat::add_message(char type, QHostAddress address, QString text)
{
QTextCharFormat format;        
switch(type)
  {
  case 0 : format.setFontUnderline(1);
           messages->setCurrentCharFormat(format);
           messages->append(QTime::currentTime().toString("hh:mm:ss")+" "+ipmap[address]+":");
           format.setFontUnderline(0);
           messages->setCurrentCharFormat(format);
           messages->append(text);
           break;
  case 1 : format.setFontItalic(1);
           messages->setCurrentCharFormat(format);
           messages->append(ipmap[address]+tr(" joined."));
           break;
  case 2 : format.setFontItalic(1);
           messages->setCurrentCharFormat(format);
           messages->append(ipmap[address]+tr(" renamed ")+text+".");
           break;
  case 3 : format.setFontItalic(1);
           messages->setCurrentCharFormat(format);
           messages->append(ipmap[address]+tr(" left the chat."));
           break;
  }
}

void bcchat::client_add(char mode, QHostAddress address, QString name)
{
switch(mode)
  {
  case 1 : if(!ipmap.contains(address)) {
            ipmap[address]=name;
            client_list->addItem(name);
            }
           break;
  case 2 : if(!ipmap.contains(address))
            client_add(1,address,name);
           else {
            QList<QListWidgetItem*> found = client_list->findItems(ipmap[address],Qt::MatchExactly);
            if(!found.isEmpty())
              found[0]->setText(name);
            ipmap[address]=name;
            }
           break;
  case 3 : if(ipmap.contains(address)) {
             QList<QListWidgetItem*> found = client_list->findItems(ipmap[address],Qt::MatchExactly);
             if(!found.isEmpty())
               delete found[0];
               }
             ipmap.remove(address);
           break;
  }
}

// ########## NETWORK FUNCTIONS ##########

/* CHAT PACKET MAGICKS:
0: Sender wants to know who's out there, sends its nickname
1: Sender reports its nickname
2: Sender changes nick
3: Sender leaves
4: Message */

void bcchat::receive_message()
{
while(socket->hasPendingDatagrams())
  {
  QByteArray data;
  data.resize(socket->pendingDatagramSize());
  QHostAddress sender;
  socket->readDatagram(data.data(), data.size(), &sender);
  switch(data[0])
   {
   case 0 : send_packet(1,sender);
            client_add(1,sender,QString::fromUtf8(data.mid(1,-1)));
            add_message(1,sender);
            break;
   case 1 : client_add(1,sender,QString::fromUtf8(data.mid(1,-1)));
            break;
   case 2 : add_message(2,sender,QString::fromUtf8(data.mid(2,-1)));
            client_add(2,sender,QString::fromUtf8(data.mid(2,-1)));
            break;
   case 3 : add_message(3,sender);
            client_add(3,sender);
            break;
   case 4 : add_message(0,sender,QString::fromUtf8(data.mid(1,-1)));
            stat_win->msg_recv++;
            break;
   }
  stat_win->recv++;
  stat_win->size_recv += data.size();
  }
}

void bcchat::send_packet(char mode, QList<QHostAddress> targets, QString value)
{
if(targets.isEmpty())
  targets.append(QHostAddress(QHostAddress::Broadcast));
QByteArray data;
data.resize(1);
switch(mode)
  {
  case 0 : data[0]=0;
           data.append(nickname.toUtf8());
           break;
  case 1 : data[0]=1;
           data.append(nickname.toUtf8());
           break;
  case 2 : data[0]=2;
           data.append(value.toUtf8());
           nickname=value;
           break;
  case 3 : data[0]=3;
           break;
  case 4 : data[0]=4;
           data.append(value.toUtf8());
           stat_win->msg_sent++;
           break;
  }
for(int tid=0;tid<targets.size();tid++) {
 socket->writeDatagram(data.data(),data.size(),targets[tid],port);
 stat_win->sent++;
 stat_win->size_sent += data.size();
 }
}

void bcchat::send_packet(char mode, QHostAddress target, QString value)
{
QList<QHostAddress> targets;
targets.append(target);
send_packet(mode,targets,value);
}

// ########## SOME SLOTS ##########

void bcchat::send_message(QHostAddress target)
{
if(message->toPlainText().length() > 0)
  {
  QList<QHostAddress> target_list;
  if(target != QHostAddress())
    target_list.append(target);
  else
    target_list = ipmap.keys();
  send_packet(4,target_list,message->toHtml());
  message->setText("");
  }
}

void bcchat::show_options_dlg()
{
bcchat_opts *odlg = new bcchat_opts(this);
odlg->nick_edit->setText(nickname);
odlg->port_spin->setValue(port);
odlg->exec();
//Update nickname
if((odlg->nick_edit->text() != "") & (odlg->nick_edit->text() != nickname))
  {
  send_packet(2,QList<QHostAddress>(),odlg->nick_edit->text());
  nick_label->setText(tr("Nickname: ")+odlg->nick_edit->text());
  }
//Update port
if(odlg->port_spin->value() != port)
  {
  delete socket;
  socket = new QUdpSocket;
  if(!socket->bind(odlg->port_spin->value())) {
    QMessageBox::critical(this,tr("Error!"),tr("Unable to open port ")+QString::number(port)+tr(". Using previous port."));
    socket->bind(port);
    }
  else
    port=odlg->port_spin->value();
  connect(socket,SIGNAL(readyRead()),this, SLOT(receive_message()));
  }
}

void bcchat::check_send_button()
{
if(message->toPlainText() == "")
  send_button->setEnabled(false);
else
  send_button->setEnabled(true);
}

void bcchat::display_info()
{
QMessageBox msg;
msg.setIconPixmap(QPixmap(":icon-96.png"));
msg.setWindowTitle(tr("About BCChat ..."));
msg.setText(tr("BCChat - a broadcast chat application, Version ")+version+tr("\n\nThis program sends and receives messages using UDP on port 54545 (default). No server is needed.\n\n\251 2010 Fabian Lesniak\nThis program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License."));
msg.setWindowModality(Qt::WindowModal);
msg.exec();
}
