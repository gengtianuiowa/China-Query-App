# China-Query-App
Final updated on 2018.12
A useful simple GIS system which is able to visualize Chinese map (point layer and base map) by text data.
Users are able to execute bi-directional queries on map:
1. From map to attribute: a pop-up window of point attribute is shown by clicking the point on the map. This is achieved by using grid index: cutting the space into 3 * 4 grids. When clicking on map, judge which grid you are clicking in first, then compare which point you are clicking by searching in the grid. This will reduce the number of distance comparisons in the algorithm to get attributes.
2. From attribute to map: by inputting the attribute of point, the point which has this attribute will be enlarged and marked red to display on map.</br>
Watch my Youtube Channel for more details: https://www.youtube.com/watch?v=IoUfgVeRRHs&t=59s. (sorry, in this video the explaination is in Chinese)
