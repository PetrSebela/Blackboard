# Splines
- for now, splines are represented by lines connecting spline control points
- splines are used for smoothing out lines when zoomed in


# Selection tool
- while hovering over selectable object, change cursor appropriately and show select box around selectable part
- drag clicking on empty surface causes select box to appear and alowing user to select multiple objects 
- adding shift adds newly selected objects to previous selection
- selection area does not have to cover entire object, intersection is suffitient
- encapsulates all selected objects under one which alows whole selection to be moved

## Selecting splines
1. if at least one line segment intersects at some side of selection box, then spline should be selected
2. if distance between at least one point and some side of selection box is less than spline thickness, then the spline should be selected