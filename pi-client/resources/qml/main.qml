import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Universal 2.0

import PI 1.0
import "."

ApplicationWindow {
    id: root
    title: qsTr("Pi Client")
    width: 300
    height: 500
    visible: true

    Universal.theme: Universal.Dark
    Universal.accent: Universal.Green

    Background {
        anchors.fill: parent
    }

    NotificationHandler {
        id: notificationArea
    }

    StackView {
        id: mainStack
        anchors.fill: parent

        Component {
            id: startStack
            StartStack {
                notificationHandler: notificationArea
            }
        }

        Component {
            id: connectingScreen
            ConnectingScreen {}
        }

        Component {
            id: loginScreen
            LoginScreen {}
        }

        Component {
            id: authenticatingScreen
            AuthenticatingScreen {}
        }

        Component {
            id: webViewScreen
            WebViewScreen {}
        }

        Component {
            id: appStack
            AppStack {
                notificationHandler: notificationArea
            }
        }

        states: [
            State {
                name: "disconnected"
                when: cppModel.me.status === ClientPiUser.PIUSER_DISCONNECTED
            },
            State {
                name: "connecting"
                when: cppModel.me.status === ClientPiUser.PIUSER_CONNECTING
            },
            State {
                name: "connected"
                when: cppModel.me.status === ClientPiUser.PIUSER_CONNECTED
            },
            State {
                name: "authenticating"
                when: cppModel.me.status === ClientPiUser.PIUSER_AUTHENTICATING && !cppModel.browserNeeded
            },
            State {
                name: "authenticating_in_browser"
                when: cppModel.me.status === ClientPiUser.PIUSER_AUTHENTICATING && cppModel.browserNeeded
            },
            State {
                name: "authenticated"
                when: cppModel.me.status === ClientPiUser.PIUSER_AUTHENTICATED
            }
        ]

        transitions: [

            Transition {
                to: "disconnected"

                ScriptAction {
                    script: {
                        var screen = startStack.createObject(mainStack);
                        mainStack.replace( screen );
                    }
                }
            },
            Transition {
                to: "connecting"

                ScriptAction {
                    script: {
                        var screen = connectingScreen.createObject(mainStack);
                        mainStack.replace( screen );
                    }
                }
            },
            Transition {
                to: "connected"

                ScriptAction {
                    script: {
                        var screen = loginScreen.createObject(mainStack);
                        mainStack.replace( screen );
                    }
                }
            },
            Transition {
                to: "authenticating"

                ScriptAction {
                    script: {
                        var screen = authenticatingScreen.createObject(mainStack);
                        mainStack.replace( screen );
                    }
                }
            },
            Transition {
                to: "authenticating_in_browser"

                ScriptAction {
                    script: {
                        var screen = webViewScreen.createObject(mainStack);
                        mainStack.replace( screen );
                    }
                }
            },
            Transition {
                to: "authenticated"

                ScriptAction {
                    script: {
                        var screen = appStack.createObject(mainStack);
                        mainStack.replace( screen );
                    }
                }
            }
        ]
    }
}
