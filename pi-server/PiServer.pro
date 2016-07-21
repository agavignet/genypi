# ----------------
# Module Settings
# ----------------

MODULE.NAME = PI
MODULE.PATH = ..

# -----------------
# Project Settings
# -----------------

PROJECT.ID   = pi-server
PROJECT.DIR  = pi-server
PROJECT.NAME = PiServer
PROJECT.QT   = qml quick widgets
PROJECT.TYPE = app

# ------------
# Qt Settings
# ------------


# -----------------
# Modules Settings
# -----------------

HB.LINKTYPE = staticlib

LINKED_MODULES = HB
HB += network link log tools
PI += pi-common

# ---------------
# QMake Includes
# ---------------

!include( $${MODULE.PATH}/$${MODULE.NAME}.pri ): \
error( "QMake file $${MODULE.NAME}.pri not found" )
