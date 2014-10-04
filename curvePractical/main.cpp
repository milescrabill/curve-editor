//
//  main.cpp
//  curvePractical
//
//  Created by Miles Crabill on 9/18/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#include <iostream>
#include <vector>
#include <GLUT/Glut.h>
#include <math.h>
#include "float2.h"
#include "circle.h"
#include "curve.h"
#include "wonkycurve.h"
#include "bezier.h"
#include "lagrangecurve.h"

// pixel width and height of the window, assigned later
int width, height;

// global values
float lineWidth = 2.0f;
float pointSize = 3.0f;

// colors
const GLfloat red[] = {1.0f, 0.0f, 0.0f};
const GLfloat green[] = {0.0f, 1.0f, 0.0f};
const GLfloat blue[] = {0.0f, 0.0f, 1.0f};
const GLfloat randColor[] = {rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX};

// debug flags
const bool debug = true;
const bool debugMouse = false;

std::vector<Freeform*> curves;

enum mode {
    polyline,
    lagrange,
    bezier,
    removeControlPoints,
    addControlPoints,
    move,
};

mode currentMode = move;
bool drawing = false;

// selected curve
int selected = 0;
Curve* selectedCurve;
bool draggingControlPoint = false;

// dragging curves
bool draggingCurve = false;
float2 startOfDrag;

// currently moving point
std::pair<int, int> currentCurvePoint = std::make_tuple(-1, -1);

void drawPoint(float2 f) {
    glBegin(GL_POINTS);
    glVertex2f(f.x, f.y);
    glEnd();
}

std::pair<int, int> mouseOverControlPoint(float2 p) {
    std::pair<int, int> closestCurvePoint = std::make_tuple(-1, -1);
    double diffX = fabs(curves[0]->getControlPoint(0).x - p.x);
    double diffY = fabs(curves[0]->getControlPoint(0).y - p.y);
    for (int i = 0; i < curves.size(); i++) {
        std::vector<int> closePoints = curves[i]->getClosestControlPoints(p);
        for (int j = 0; j < closePoints.size(); j++) {
            float2 f = curves[i]->getControlPoint(closePoints[j]);
            double newDiffX = fabs(f.x - p.x);
            double newDiffY = fabs(f.y - p.y);

            if (newDiffX - 0.005f < diffX && newDiffY - 0.005f < diffY) {
                closestCurvePoint = std::make_tuple(i, closePoints[j]);

                if (debug) {
                    printf("closestPoint curve: %d, control point: %d\n", closestCurvePoint.first, closestCurvePoint.second);
                }

                diffX = newDiffX;
                diffY = newDiffY;
            }
        }
    }

    return closestCurvePoint;
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

        int curve = currentCurvePoint.first;
        int point = currentCurvePoint.second;
        if (!(curve == -1 || point == -1)) {
            curves[curve]->setControlPoint(point, f);
            curves[curve]->populatePoints();
        }

        if (draggingCurve) {
            for (int i = 0; i < curves[selected]->numberOfControlPoints(); i++) {
                float2 p = curves[selected]->getControlPoint(i);
                p.x += f.x - startOfDrag.x ;
                p.y += f.y - startOfDrag.y;
                curves[selected]->setControlPoint(i, p);
                curves[selected]->populatePoints();
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
            case addControlPoints:
                curves[selected]->addControlPoint(f);
                break;
            case bezier: {
                if (drawing) {
                    curves[selected]->addControlPoint(f);
                    curves[selected]->populatePoints();
                } else {
                    drawing = true;
                    curves.push_back(new BezierCurve());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    curves[selected]->addControlPoint(f);
                }
                break;
            }
            case lagrange: {
                if (drawing) {
                    curves[selected]->addControlPoint(f);
                } else {
                    drawing = true;
                    curves.push_back(new LagrangeCurve());
                    selectedCurve = curves.back();
                    selected = int(curves.size()) - 1;
                    curves[selected]->addControlPoint(f);
                }
                break;
            }
            case move: {
                if (!drawing) {
                    currentCurvePoint = mouseOverControlPoint(f);
                    if (currentCurvePoint.first == -1 && currentCurvePoint.second == -1) {
                        int curve = mouseOverCurve(f);
                        if (curve != -1) {
                            selected = curve;
                            selectedCurve = curves[curve];
                            draggingCurve = true;
                            startOfDrag = f;
                        }
                    } else {
                        draggingControlPoint = true;
                    }
                }
            }
            default: {
                break;
            }
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && !drawing) {
        currentCurvePoint = std::make_pair(-1, -1);
        draggingControlPoint = false;
        draggingCurve = false;
    }
}

void onDisplay() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    int time = glutGet(GLUT_ELAPSED_TIME);

    for (int i = 0; i < curves.size(); i++) {

        glLineWidth(lineWidth);
        glColor3fv(red);
        if (selected == i && !draggingControlPoint) {
            glColor3fv(blue);
            glLineWidth(2 * lineWidth);
//          curves[i]->drawTangent(time * 0.0001 - floor(time * 0.0001));
        }

        curves[i]->draw();
        curves[i]->drawControlPoints();
    }

    if (draggingControlPoint) {
        glPointSize(3.0f * pointSize);
        float2 p = curves[currentCurvePoint.first]->getControlPoint(currentCurvePoint.second);
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
    if (key == 'p') {
        if (debug && currentMode != polyline) {
            printf("polyline selected\n");
        }
        currentMode = polyline;
    } else if (key == 'b') {
        if (debug && currentMode != bezier) {
            printf("bezier selected\n");
        }
        currentMode = bezier;
    } else if (key == 'l') {
        if (debug && currentMode != lagrange) {
            printf("lagrange selected\n");
        }
        currentMode = lagrange;
    } else if (key == 'd') {
        if (debug && currentMode != removeControlPoints) {
            printf("removeControlPoint selected\n");
        }
        currentMode = removeControlPoints;
    }
}

void onKeyboardUp(unsigned char key, int mouseX, int mouseY) {
    if (key == 'c') {
        for (int i = 0; i < curves.size(); i++) {
            curves[i]->clearControlPoints();
        }
    } else if (key == 32) {
        selected = (selected + 1) % curves.size();
        if (debug) {
            printf("Selected: %d\n", selected);
        }
    } else if (key == 'p') {
        currentMode = move;
        drawing = false;
        if (debug) {
            printf("polyline deselected\n");
        }
    } else if (key == 'b') {
        currentMode = move;
        drawing = false;
        if (debug) {
            printf("bezier deselected\n");
        }
    } else if (key == 'l') {
        currentMode = move;
        drawing = false;
        if (debug) {
            printf("lagrange deselected\n");
        }
    } else if (key == 'd') {
        currentMode = move;
        drawing = false;
        if (debug) {
            printf("removeControlPoints deselected\n");
        }
    }
}

void onIdle() {
    if (!drawing) {
        for (int i = 0; i < curves.size(); i++) {
            if (curves[i]->numberOfControlPoints() <= 2) {
                curves.erase(curves.begin() + selected);
                selected = int(curves.size()) - 1;
                selectedCurve = curves.back();
            }
        }
    }

    glutPostRedisplay();
}

int main(int argc, char * argv[])
{
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
    srand(time(NULL));

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
