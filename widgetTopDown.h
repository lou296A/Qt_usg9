// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WIDGETTOPDOWN_H
#define WIDGETTOPDOWN_H

#include <QWidget>
#include <QPaintEvent>
#include "voxel.h"


class WidgetTopDown : public QWidget
{
    Q_OBJECT

public:
    WidgetTopDown(QWidget *parent);
    void paintEvent(QPaintEvent *event);
    void paint(QPainter *painter, QPaintEvent *event, const QList<XYZ>& xyzList, const QList<short>& intensityList);

public slots:
    void setData(const QList<XYZ>& newXYZList, const QList<short>& newIntensityList);

protected:

private:
    QBrush background;
    QBrush voxelBrush;
    QList<XYZ> xyzList;
    QList<short> intensityList;
};

#endif
