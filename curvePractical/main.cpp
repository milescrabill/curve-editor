//
//  main.cpp
//  curvePractical
//
//  Created by Miles Crabill on 9/18/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

/*
    Implemented features:
    - Pressing SPACE makes the next object selected instead of the current one, repeatedly pressing SPACE cycles over all curves. [5 pts]
        - working
    - When no keys are pressed and the user presses the mouse button, the object at the mouse cursor (if there is any) becomes selected. [15 pts if this works approximately, (*) 25 pts if this works exactly]
        - 15 points currently
        - working
    - As long as the mouse button remains pressed, mouse movements drag the selected object. When the button is released, the object remains selected. [15 pts]
        - dragging works for all control points, work for all curves (hermite and catmull-rom wiggle a bit)
    - When an object is selected, the user may hold down 'A' to add control points to the selected object by clicking. [5 pts]
        - working
    - Support removing control points of the selected curve, by holding 'D' and clicking them. [10 pts]
        - working
    - Support dragging control points of the selected curve. [10 pts]
        - working
        - note: for catmull-rom, cannot drag tangent points (as it should be) but can select them
    - When an object is selected, pressing the 'F' key should turn the curve into a polygon that fills the curve. [5 pts if it works for convex shapes, (*) 20 pts if it works for non-self-intersecting curves, (*) 25 points if it works for any curve]
        - 5 points currently
        - working for convex
     - Support cubic Hermite interpolation splines, created by clicking control points while holding key 'U'. Derivatives at control points should be manually editable. Extra controls should be displayed to allow the user to adjust derivatives. [(*) 35 pts]
        - working
     - Support Catmull-Rom splines, created by clicking control points while holding key 'R'. [25 pts]
        - working?


    Total: 5 + 15 + 15 + 5 + 10 + 10 + 5 + 35 + 25 (or so)
*/

// compilation on OSX:
// cc -I. -framework GLUT -framework OpenGL -framework Cocoa -Wno-deprecated -Wno-c++11-extensions -lstdc++ main.cpp -o curve

#include <iostream>
#include <vector>
#include <GLUT/Glut.h>
#include <math.h>
#include "float2.h"
#include "circle.h"
#include "curve.h"
#include "wonkycurve.h"
#include "bezier.h"
#include "lagrange.h"
#include "sinewave.h"
#include "polyline.h"
#include "hermite.h"
#include "catmullrom.h"

// pixel width and height of the window, assigned later
int width, height;

// global values
float lineWidth = 2.0f;
float pointSize = 4.0f;

// colors
const GLfloat red[] = {1.0f, 0.0f, 0.0f};
const GLfloat green[] = {0.0f, 1.0f, 0.0f};
const GLfloat purple[] = {0.33f, 0.1f, 0.55f};
const GLfloat blue[] = {0.0f, 0.0f, 1.0f};
const GLfloat yellow[] = {0.89f, 0.81f, 0.34f};
const GLfloat fuchsia[] = {1.0f, 0.0f, 1.0f};
const GLfloat randColor[] = {rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX};

// debug flags
const bool debug = false;
const bool debugMouse = false;

std::vector<Freeform*> curves;

enum mode {
    polyline,
    lagrange,
    bezier,
    hermite,
    catmullrom,
    removeControlPoints,
    addControlPoints,
    move,
};

// for checking the type of curve, uses a virtual method
const int BezierType = 0;
const int LagrangeType = 1;
const int PolylineType = 2;
const int HermiteType = 3;
const int CatmullRomType = 4;

mode currentMode = move;
bool drawing = false;
bool filling = false;

// selected curve
// defaults to -1 to signify no selected curve
int selected = -1;

Freeform *selectedCurve;
bool draggingControlPoint = false;

// dragging curves
bool draggingCurve = false;
float2 startOfDrag;

// currently moving point index within selected curve
int currentPointIndex;

int mouseOverControlPoint(float2 p) {
    if (selectedCurve == NULL) {
        return -1;
    }

    int closestPoint = -1;
    if (int(curves.size()) == 0) {
        return closestPoint;
    }
    double diffX = fabs(selectedCurve->getControlPoint(0).x - p.x);
    double diffY = fabs(selectedCurve->getControlPoint(0).y - p.y);

    std::vector<int> closePoints = selectedCurve->getClosestControlPoints(p);

    // loops over control points of curve, finds closest within an epsilon
    for (int j = 0; j < closePoints.size(); j++) {
        float2 f;
        f = selectedCurve->getControlPoint(closePoints[j]);
        double newDiffX = fabs(f.x - p.x);
        double newDiffY = fabs(f.y - p.y);
        if (newDiffX - 0.005f < diffX && newDiffY - 0.005f < diffY) {
            closestPoint = closePoints[j];
            if (debug) {
                printf("closestPoint curve: %d, control point: %d\n", selected, closestPoint);
            }
            diffX = newDiffX;
            diffY = newDiffY;
        }
    }
    return closestPoint;
}

int mouseOverCurve(float2 p) {
    int closestCurve = -1;
    double diffX = fabs(curves[0]->lookupPoint(0).x - p.x);
    double diffY = fabs(curves[0]->lookupPoint(0).y - p.y);
    for (int i = 0; i < curves.size(); i++) {
        std::vector<float2> closestPoints = curves[i]->isPointOnCurve(p);
        for (int j = 0; j < closestPoints.size(); j++) {
            float2 f = curves[i]->lookupPoint(j);
            double newDiffX = fabs(f.x - p.x);
            double newDiffY = fabs(f.y - p.y);

            if (newDiffX - 0.005f < diffX && newDiffY - 0.005f < diffY) {
                closestCurve = i;
                diffX = newDiffX;
                diffY = newDiffY;
            }
        }
    }
    if (debug) {
        printf("point is on curve %d\n", closestCurve);
    }
    return closestCurve;
}

float2 convertMouse(int x, int y) {
    float newX = 2 * (x / (width * 1.0f)) - 1;
    float newY = 2 * (1 - y / (height * 1.0f)) - 1;

    if (debugMouse) {
        printf("converted x: %f, converted y: %f\n", newX, newY);
    }
    return float2(newX, newY);
}

void onMouseMove(int x, int y) {
    if (debugMouse) {
        printf("mouse x: %d, mouse y: %d\n", x, y);
    }

    if (currentMode == move) {
        float2 f = convertMouse(x, y);
        if (draggingControlPoint) {
            // special case for catmall rom, where we append tangents to
            // end of control points, but don't want them moveable by click
            if (currentPointIndex < selectedCurve->numberOfControlPoints() || selectedCurve->curveType() == HermiteType) {
                selectedCurve->setControlPoint(currentPointIndex, f);
            }
            selectedCurve->populatePoints();
        }
        if (draggingCurve) {
            int n = selectedCurve->numberOfControlPoints();
            int type = selectedCurve->curveType();
            if (type == HermiteType || type == CatmullRomType) {
                n *= 2;
            }
            for (int i = 0; i < n; i++) {
                float2 p = selectedCurve->getControlPoint(i);
                p += f - startOfDrag;
                selectedCurve->setControlPoint(i, p);
                selectedCurve->populatePoints();
            }
            startOfDrag = f;
        }
    }
}

void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (debugMouse) {
            printf("mouse x: %d, mouse y: %d\n", x, y);
        }

        float2 f = convertMouse(x, y);

        switch (currentMode) {
            case addControlPoints: {
                if (curves.size() > 0) {
                    selectedCurve->addControlPoint(f);
                    selectedCurve->populatePoints();
                }
                break;
            }
            case removeControlPoints: {
                if (curves.size() > 0) {
                    currentPointIndex = mouseOverControlPoint(f);
                    selectedCurve->removeControlPoint(currentPointIndex);
                    selectedCurve->populatePoints();
                    currentPointIndex = -1;
                }
                break;
            }
            case polyline: {
                if (drawing && curves.size() > 0) {
                    selectedCurve->addControlPoint(f);
                    selectedCurve->populatePoints();
                } else {
                    drawing = true;
                    curves.push_back(new Polyline());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    selectedCurve->addControlPoint(f);
                }
                break;
            }
            case bezier: {
                if (drawing && curves.size() > 0) {
                    selectedCurve->addControlPoint(f);
                    selectedCurve->populatePoints();
                } else {
                    drawing = true;
                    curves.push_back(new BezierCurve());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    selectedCurve->addControlPoint(f);
                }
                break;
            }
            case lagrange: {
                if (drawing && curves.size() > 0) {
                    selectedCurve->addControlPoint(f);
                    selectedCurve->populatePoints();
                } else {
                    drawing = true;
                    curves.push_back(new LagrangeCurve());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    selectedCurve->addControlPoint(f);
                }
                break;
            }
            case hermite: {
                if (drawing && curves.size() > 0) {
                    selectedCurve->addControlPoint(f);
                    selectedCurve->populatePoints();
                } else {
                    drawing = true;
                    curves.push_back(new Hermite());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    selectedCurve->addControlPoint(f);
                }
                break;
            }
            case catmullrom: {
                if (drawing && curves.size() > 0) {
                    selectedCurve->addControlPoint(f);
                    selectedCurve->populatePoints();
                } else {
                    drawing = true;
                    curves.push_back(new CatmullRom());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    selectedCurve->addControlPoint(f);
                }
                break;
            }
            case move: {
                if (!drawing && curves.size() > 0) {
                    currentPointIndex = mouseOverControlPoint(f);
                    if (currentPointIndex == -1) {
                        int curve = mouseOverCurve(f);
                        if (curve != -1) {
                            selected = curve;
                            selectedCurve = curves[curve];
                            draggingCurve = true;
                            startOfDrag = f;
                        } else {
                            selectedCurve = NULL;
                        }
                    } else {
                        draggingControlPoint = true;
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && !drawing) {
        currentPointIndex = -1;
        draggingControlPoint = false;
        draggingCurve = false;
    }
}

void drawCurve(Freeform *curve) {
    Hermite *h = dynamic_cast<Hermite*>(curve);
    switch (curve->curveType()) {
        case PolylineType:
            glColor3fv(purple);
            break;
        case BezierType:
            glColor3fv(red);
            break;
        case LagrangeType:
            glColor3fv(green);
            break;
        case CatmullRomType: {
            glColor3fv(fuchsia);
            break;
        }
        case HermiteType:
            h->linesBetweenTangentAndControlPoints();
            h->drawTangentPoints();
            glColor3fv(yellow);
            break;
        default:
            break;
    }

    if (curve == selectedCurve && !draggingControlPoint) {
        glColor3fv(blue);
        glLineWidth(2 * lineWidth);
        glPointSize(1.5 * pointSize);
        if (filling) {
            curve->fill();
        }
    } else {
        glLineWidth(lineWidth);
        glPointSize(pointSize);
    }

    curve->draw();
    curve->drawControlPoints();

}

void onDisplay() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    int time = glutGet(GLUT_ELAPSED_TIME);

    for (int i = 0; i < curves.size(); i++) {
        drawCurve(curves[i]);
    }

    if (draggingControlPoint) {
        float2 p = selectedCurve->getControlPoint(currentPointIndex);
        glPointSize(3.0f * pointSize);
        glBegin(GL_POINTS);
        glColor3fv(blue);
        glVertex2f(p.x, p.y);
        glEnd();
        glPointSize(pointSize);
    }

    glutSwapBuffers();
}

void onReshape(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void onKeyboard(unsigned char key, int mouseX, int mouseY) {
    switch(key) {
        case 'p': {
            if (debug && currentMode != polyline) {
                printf("polyline selected\n");
            }
            currentMode = polyline;
            break;
        }
        case 'b': {
            if (debug && currentMode != bezier) {
                printf("bezier selected\n");
            }
            currentMode = bezier;
            break;
        }
        case 'l': {
            if (debug && currentMode != lagrange) {
                printf("lagrange selected\n");
            }
            currentMode = lagrange;
            break;
        }
        case 'u': {
            if (debug && currentMode != hermite) {
                printf("hermite selected\n");
            }
            currentMode = hermite;
            break;
        }
        case 'd': {
            if (debug && currentMode != removeControlPoints) {
                printf("removeControlPoints selected\n");
            }
            currentMode = removeControlPoints;
            break;
        }
        case 'a': {
            if (debug && currentMode != addControlPoints) {
                printf("addControlPoints selected\n");
            }
            currentMode = addControlPoints;
            break;
        }
        case 'r': {
            if (debug && currentMode != catmullrom) {
                printf("catmullrom selected\n");
            }
            currentMode = catmullrom;
            break;
        }
        // this worked, but not with the ability to click points / curves
//        case 'q': {
//            if (curves.size() > 0) {
//                selectedCurve->rotationAngle++;
//            }
//            break;
//        }
//        case 'e': {
//            if (curves.size() > 0) {
//                selectedCurve->rotationAngle--;
//            }
//            break;
//        }
    }
}

void onKeyboardUp(unsigned char key, int mouseX, int mouseY) {
    if (key == 'c') {
        for (int i = 0; i < curves.size(); i++) {
            curves[i]->clearControlPoints();
        }
    } else if (key == 'f') {
        filling = !filling;
    } else if (key == 32) {
        selected = (selected + 1) % curves.size();
        selectedCurve = curves[selected];
        if (debug) {
            printf("Selected: %d\n", selected);
        }
    } else {
        currentMode = move;
        drawing = false;
    }
}

void onIdle() {
    // checks for and deletes finished curves with <2 points
    if (!drawing) {
        for (int i = 0; i < curves.size(); i++) {
            if (curves[i]->numberOfControlPoints() <2) {
                curves.erase(curves.begin() + selected);
                selected = 0;
                selectedCurve = curves[selected];
            }
        }
    }

    glutPostRedisplay();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    width = height = 512;
    glutInitWindowSize(width, height);
    glutCreateWindow("curves");

    glMatrixMode(GL_PROJECTION);
    glOrtho(0, width, 0, height, 0, 0);

    glColor3fv(red);

    glLineWidth(lineWidth);
    glPointSize(pointSize);

    // randomize seed
    srand(int(time(NULL)));

    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMouseMove);
    glutIdleFunc(onIdle);
    
    glutMainLoop();
    return 0;
}
