#ifndef VOXEL_H
#define VOXEL_H

#include<QString>

struct FrameSegment {
    QString intensity;
    QString coordinate;
};
struct TPRQ {
    int theta;
    int phi;
    int radius;
    int quadrant;
};
struct XYZ {
    int x;
    int y;
    int z;
};

#endif // VOXEL_H
