#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "voxel.h"
#include "readFramesFromUDP.h"
#include "widgetTopDown.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // void paintEvent(QPaintEvent *event);

Q_SIGNALS:
    void newDataAvailable(const QList<XYZ>& xyzList, const QList<short>& intensityList);

public slots:
    void handleNewFrame();

// private slots:
//     void updateWidgetTopDownSlot();

private:
    Ui::MainWindow *ui;
    QPaintEvent *event;
    ReadFramesFromUDP readFramesFromUDP;
    QList<FrameSegment> decodeFrame(const QByteArray& frameBuffer);
    QList<XYZ> processSegments(const QList<FrameSegment>& decodedSegments);
    QString byteToBinary(unsigned char byte);

    QList<XYZ> xyzList;
    QList<short> intensityList;

    WidgetTopDown* widgetTopDown;
};

#endif // MAINWINDOW_H
