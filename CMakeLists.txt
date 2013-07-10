#************************************************************************
#* Copyright (C) 2010 Fabian Lesniak <f.lesniak@gmx.de>                 *
#*                                                                      *
#* This file is part of BCChat.                                         *
#*                                                                      *
#* BCChat is free software: you can redistribute it and/or modify       *
#* it under the terms of the GNU General Public License as published by *
#* the Free Software Foundation, either version 3 of the License, or    *
#* (at your option) any later version.                                  *
#*                                                                      *
#* BCChat is distributed in the hope that it will be useful,            *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of       *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
#* GNU General Public License for more details.                         *
#*                                                                      *
#* You should have received a copy of the GNU General Public License    *
#* along with BCChat.  If not, see <http://www.gnu.org/licenses/>.      *
#***********************************************************************/

cmake_minimum_required( VERSION 2.4 )
project( bcchat )

find_package( Qt4 COMPONENTS QtGui QtNetwork REQUIRED )

SET( bcchat_SRCS main.cpp bcchat.cpp bcchat_opts.cpp bcchat_stat.cpp bcchat_messagefield.cpp )
SET( bcchat_HDRS bcchat.h bcchat_opts.h bcchat_stat.h bcchat_messagefield.h )
SET( bcchat_TS_FILES de.ts )

include( ${QT_USE_FILE} bcchat_HDRS )

option( UPDATE_TRANSLATIONS "Updates TS-Files" )
IF( UPDATE_TRANSLATIONS )
  QT4_CREATE_TRANSLATION( bcchat_QM_FILES ${bcchat_SRCS} ${bcchat_TS_FILES} )
ELSE( UPDATE_TRANSLATIONS )
  QT4_ADD_TRANSLATION( bcchat_QM_FILES ${bcchat_TS_FILES} )
ENDIF( UPDATE_TRANSLATIONS )
add_custom_target( translation_target DEPENDS ${bcchat_QM_FILES} )

QT4_WRAP_CPP( bcchat_MOC_SRCS ${bcchat_HDRS} )
QT4_ADD_RESOURCES( bcchat_RESRCS resources.qrc )

add_definitions( -O2 )

IF( MINGW )
  add_executable( bcchat WIN32 ${bcchat_SRCS} ${bcchat_MOC_SRCS} ${bcchat_RESRCS} ${bcchat_QM_FILES} )
ELSE( MINGW )
  add_executable( bcchat ${bcchat_SRCS} ${bcchat_MOC_SRCS} ${bcchat_RESRCS} ${bcchat_QM_FILES} )
ENDIF( MINGW )

target_link_libraries( bcchat ${QT_LIBRARIES} )
