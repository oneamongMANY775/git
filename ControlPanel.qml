import QtQuick
import QtQuick.Layouts

ColumnLayout {
    property alias openGLButton: openGLButton
    signal refreshRateChanged(variant rate);
    signal signalSourceChanged(string source, int sampleCount);

    spacing: 8
    Layout.fillHeight: true

    Text {
        text: "Scope"
        font.pointSize: 18
        color: "white"
    }

    MultiButton {
        id: openGLButton
        text: "OpenGL: "
        items: ["false", "true"]
        currentSelection: openGLSupported ? 1 : 0
        onSelectionChanged: openGlChanged(currentSelection == 1);
        enabled: openGLSupported
    }



    MultiButton {
        id: sampleCountButton
        text: "Samples: "
        items: ["128", "1024", "10000"]
        currentSelection: 1
        onSelectionChanged: signalSourceChanged(
                                signalSourceButton.items[signalSourceButton.currentSelection],
                                selection);
    }

    MultiButton {
        text: "Refresh rate: "
        items: ["1", "24", "60"]
        currentSelection: 2
        onSelectionChanged: refreshRateChanged(items[currentSelection]);
    }
}
