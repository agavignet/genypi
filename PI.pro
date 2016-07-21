# -----------------
# Project Settings
# -----------------

PROJECT.NAME = PI
PROJECT.TYPE = subdirs

SUBDIRS =   \
            pi-common/PiCommon.pro \
            pi-client/PiClient.pro \
            pi-server/PiServer.pro

# ---------------
# QMake Includes
# ---------------

!include( $${PROJECT.NAME}.pri ): \
error( "QMake file $${PROJECT.NAME}.pri not found" )
