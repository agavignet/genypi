# ----------------
# Module Settings
# ----------------

MODULE.NAME = PI
MODULE.PATH = ..

# -----------------
# Project Settings
# -----------------

PROJECT.ID   = pi-common
PROJECT.DIR  = pi-common
PROJECT.NAME = PiCommon
PROJECT.QT   = qml quick widgets sql multimedia

SUBDIRS +=

# ------------
# Qt Settings
# ------------


# -----------------
# Modules Settings
# -----------------

HB.LINKTYPE = staticlib

LINKED_MODULES = HB
HB += network log tools

# ---------------
# QMake Includes
# ---------------

!include( $${MODULE.PATH}/$${MODULE.NAME}.pri ): \
error( "QMake file $${MODULE.NAME}.pri not found" )
