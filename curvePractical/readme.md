Miles Crabill

Implemented features:
- Pressing SPACE makes the next object selected instead of the current one, repeatedly pressing SPACE cycles over all curves. [5 pts]
    - working
- When no keys are pressed and the user presses the mouse button, the object at the mouse cursor (if there is any) becomes selected. [15 pts if this works approximately, (*) 25 pts if this works exactly]
    - 15 points currently
    - working
- As long as the mouse button remains pressed, mouse movements drag the selected object. When the button is released, the object remains selected. [15 pts]
    - dragging works for all control points, work for all curves (hermite and catmull-rom wiggle a bit)
        - except polyline
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
    - working