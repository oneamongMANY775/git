import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle
{
    width: 600
    height: 400

    Loader {
        id: loader
        anchors.fill: parent
        source: "final_view.qml"
        antialiasing: true
        asynchronous: true
        visible: true
    }
}

