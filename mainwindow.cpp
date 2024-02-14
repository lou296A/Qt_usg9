#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgetTopDown.h"
#include "widgetFrontFacing.h"
#include "readFramesFromUDP.h"
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    WidgetTopDown *widgetTopDown = new WidgetTopDown(this);
    WidgetFrontFacing *widgetFrontFacing = new WidgetFrontFacing(this);
    connect(&readFramesFromUDP, &ReadFramesFromUDP::newFrameAvailable, this, &MainWindow::handleNewFrame);
    connect(this, &MainWindow::newDataAvailable, widgetTopDown, &WidgetTopDown::setData);
    connect(this, &MainWindow::newDataAvailable, widgetFrontFacing, &WidgetFrontFacing::setData);

    ui->gridLayout->addWidget(widgetTopDown, 5, 0);
    ui->gridLayout->addWidget(widgetFrontFacing, 5, 3);
    QPixmap pix("C:/Users/silin/Desktop/qt_usg9/usg9_ui_v0/qt_pic/nus_logo.jpg");
    ui->label_logo->setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatio));
}


MainWindow::~MainWindow()
{
    delete ui;
}



// To add LF/HF switching
void MainWindow::handleNewFrame() {
    // Retrieve one frame from queue
    QByteArray frame;
    if (!readFramesFromUDP.HFramesUDPBuffer.isEmpty()) {
        frame = readFramesFromUDP.HFramesUDPBuffer.dequeue();
    } else if (!readFramesFromUDP.LFramesUDPBuffer.isEmpty()) {
        frame = readFramesFromUDP.LFramesUDPBuffer.dequeue();
    }

    // Decode one frame into voxel_pos / voxel_value list
    QList<FrameSegment> decodedSegments = decodeFrame(frame);
    xyzList = processSegments(decodedSegments);
    for (const FrameSegment& segment : decodedSegments) {
        short intensity = segment.intensity.toShort(nullptr, 2);
        intensityList.append(intensity);
    }

    qDebug() << "list size" << xyzList.size();
    emit newDataAvailable(xyzList, intensityList);
}

QList<FrameSegment> MainWindow::decodeFrame(const QByteArray& frameBuffer) {
    QList<FrameSegment> decodedSegments;
    QString binaryData;

    for (char byte : frameBuffer) {
        binaryData += byte;
    }

    // Iterate through the binary string in segments of 35 bits
    for (int i = 0; i < binaryData.length(); i += 35) {
        if (i + 35 > binaryData.length()) {
            break; // Handle the case for any remaining bits that don't make up a full segment
        }
        QString segment = binaryData.mid(i, 35);
        FrameSegment frameSegment;
        frameSegment.coordinate = segment.left(19); // First 19 bits for the coordinate
        frameSegment.intensity = segment.mid(19, 16); // Next 16 bits for the intensity
        decodedSegments.append(frameSegment);
    }

    return decodedSegments;
}

TPRQ parseTPRQ(const QString& coordinate) {
    TPRQ tprq;
    tprq.radius = coordinate.mid(0, 7).toInt(nullptr, 2);
    tprq.phi = coordinate.mid(7, 5).toInt(nullptr, 2);
    tprq.theta = coordinate.mid(12, 5).toInt(nullptr, 2);
    tprq.quadrant = coordinate.mid(17, 2).toInt(nullptr, 2);
    return tprq;
}

XYZ convertToXYZ(TPRQ tprq) {
    // Apply quadrant logic
    if (tprq.quadrant == 1) {
        tprq.theta = 63 - tprq.theta;
    } else if (tprq.quadrant == 2) {
        tprq.phi = 63 - tprq.phi;
    } else if (tprq.quadrant == 3) {
        tprq.theta = 63 - tprq.theta;
        tprq.phi = 63 - tprq.phi;
    }

    XYZ xyz;
    xyz.x = tprq.theta;
    xyz.y = tprq.phi;
    xyz.z = tprq.radius;
    return xyz;
}

QList<XYZ> MainWindow::processSegments(const QList<FrameSegment>& decodedSegments) {
    QList<XYZ> xyzList;
    for (const FrameSegment& segment : decodedSegments) {
        TPRQ tprq = parseTPRQ(segment.coordinate);
        XYZ xyz = convertToXYZ(tprq);
        xyzList.append(xyz);
    }
    return xyzList;
}
