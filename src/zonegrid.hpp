#ifndef ZONEGRID_H
#define ZONEGRID_H

#include "ofMain.h"


class ZoneGrid
{
public:
    ZoneGrid(ofVec3f dim);

    // Draw choregraphy zone
    void drawZone();

    // Draw grid in the "ground"
    void drawGrid();

    // Adds graduation to the grid
    void drawGraduation();

    // Draw vertical axes in the zone
    void drawAxes();

    // Modify axis to be displayed
    void modifyAxis(int key);

    // Get zone dimension
    ofVec3f dimension();

private:
    ofVec3f _dim;
    bool x_axis, y_axis, z_axis,_grad;
};

#endif // ZONEGRID_H
