curve-editor
============

AIT Fall 2014 Graphics Curve Practical + Curve Editor Project
This is a homework assignment that I submitted for the Computer Graphics course at AIT, Aquincum Institute of Technology.

Assignment text:
"Write a small vector graphics editor!

You need to support:
- polylines
- Lagrange and Bezier curves

The program must be able to display any (reasonable) number of such objects. The color of the objects depends on their type, but none of them is blue. Zero or one object can be selected. The selected object is displayed in blue, and with double width.

The user can create objects by pressing down the 'P', 'L', or 'B' key --- Polyline, Lagrange, or Bezier, respectively. Control points can be added by mouse clicks as long as the key is held down. Releasing the key deletes the selected object if it has less then two control points. The object being created is always the selected one, and is drawn as such. The control points of the selected curve are also drawn.

Implement the following features to earn points (starred (*) items require extra research over presented material, see notes below):
- Pressing SPACE makes the next object selected instead of the current one, repeatedly pressing SPACE cycles over all curves. [5 pts]
- When no keys are pressed and the user presses the mouse button, the object at the mouse cursor (if there is any) becomes selected. [15 pts if this works approximately, (*) 25 pts if this works exactly]
- As long as the mouse button remains pressed, mouse movements drag the selected object. When the button is released, the object remains selected. [15 pts]
- When an object is selected, the user may hold down 'A' to add control points to the selected object by clicking. [5 pts]
- Support removing control points of the selected curve, by holding 'D' and clicking them. [10 pts]
- When an object is selected, pressing the 'F' key should turn the curve into a polygon that fills the curve. [5 pts if it works for convex shapes, (*) 20 pts if it works for non-self-intersecting curves, (*) 25 points if it works for any curve]
- Support Catmull-Clark subdivision curves, created by clicking control points while holding key 'C'. [20 pts]
- Support Catmull-Rom splines, created by clicking control points while holding key 'R'. [25 pts]
- Support cubic Hermite interpolation splines, created by clicking control points while holding key 'U'. Derivatives at control points should be manually editable. Extra controls should be displayed to allow the user to adjust derivatives. [(*) 35 pts]
- Support uniform cubic B-splines, created by clicking control points while holding key 'S'. [(*) 30 pts]
- Support dragging control points of the selected curve. [10 pts]
- Display the current number of curves in the corner using Bezier curves to form the numbers (steal control points from http://jackf.net/bezier-clock/) [20 pts]
- Display an animated Bezier clock (steal concept from http://jackf.net/bezier-clock/) [30 pts]

You need
55 pts for a D,
65 pts for a C,
75 pts for a B,
85 pts for an A."
