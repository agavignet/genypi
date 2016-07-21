# ----------------
# Module Settings
# ----------------

MODULE.NAME = PI
MODULE.PATH = ..

# -----------------
# Project Settings
# -----------------

PROJECT.ID   = pi-client
PROJECT.DIR  = pi-client
PROJECT.NAME = PiClient
PROJECT.QT   = qml quick widgets webview quickcontrols2
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

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
