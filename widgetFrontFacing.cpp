// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "voxel.h"
#include "widgetFrontFacing.h"
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QTextStream>

WidgetFrontFacing::WidgetFrontFacing(QWidget *parent)
    : QWidget(parent)
{
}


void WidgetFrontFacing::setData(const QList<XYZ>& newXYZList, const QList<short>& newIntensityList) {
    // Safely update data and trigger a repaint
    xyzList = newXYZList;
    intensityList = newIntensityList;
    update();
}


void WidgetFrontFacing::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(&painter, event, xyzList, intensityList);
}


void WidgetFrontFacing::paint(QPainter *painter, QPaintEvent *event, const QList<XYZ>& xyzList, const QList<short>& intensityList)
{
    QSize widgetSize = this->size();
    qDebug() << "width" << widgetSize.width();
    qDebug() << "height" << widgetSize.height();

    painter->fillRect(event->rect(), background);
    painter->save();

    QPen voxelEdgePen(Qt::white, 0);
    painter->setPen(voxelEdgePen);
    short max_intensity = -32768;
    short min_intensity = 32767;
    for (int i = 0; i < xyzList.size(); ++i) {
        if (intensityList[i] > max_intensity) max_intensity = intensityList[i];
        if (intensityList[i] < min_intensity) min_intensity = intensityList[i];
    }


    // qDebug() << "max intensity" << max_intensity;
    // qDebug() << "min intensity" << min_intensity;

    for (int i = 0; i < xyzList.size(); ++i) {
        const XYZ& xyz = xyzList[i];
        short intensity = intensityList[i];
        QColor color = QColor(0, 0, 0);
        QBrush voxelBrush(color);
        painter->setBrush(voxelBrush);

        // Draw the voxel
        if (intensity > 0) {
            painter->drawRect(QRectF(QPointF(8*xyz.x, 8*xyz.y), QSizeF(4, 4)));
        }
    }

    painter->restore();
}
