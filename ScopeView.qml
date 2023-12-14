// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15

ChartView {
    id: chartView

    property bool openGL: openGLSupported

    // Import the SerialDataSource QML file
    SerialDataSource {
        id: serialSource // Assign an ID to the SerialDataSource instance
    }

    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark

    onOpenGLChanged: {
        if (openGLSupported) {
            var series = chartView.series("signal")
            if (series)
                series.useOpenGL = openGL;
        }
    }

    ValueAxis {
        id: axisY
        min: -1
        max: 4
    }

    ValueAxis {
        id: axisX
        min: 0
        max: 1024
    }

    LineSeries {
        id: lineSeries
        name: "signal"
        axisX: axisX
        axisY: axisY
        useOpenGL: chartView.openGL
    }

    Timer {
        id: refreshTimer
        interval: 1 / 60 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            var seriesData = serialSource.parseReceivedData(); // Get data from SerialDataSource
            if (seriesData.length > 0) {
                lineSeries.clear(); // Clear existing data
                for (var i = 0; i < seriesData.length; ++i) {
                    lineSeries.append(seriesData[i]); // Append received data
                }
            }
        }
    }

    function createAxis(min, max) {
        // The following creates a ValueAxis object that can be then set as a x or y axis for a series
        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: "
                                  + min + "; max: " + max + " }", chartView);
    }

    function setAnimations(enabled) {
        if (enabled)
            chartView.animationOptions = ChartView.SeriesAnimations;
        else
            chartView.animationOptions = ChartView.NoAnimation;
    }

    function changeRefreshRate(rate) {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }
}
