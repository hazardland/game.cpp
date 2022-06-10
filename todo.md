. Fetch mouse.x mouse.y

    -- On mouse down
        drag.startX = input.x
        drag.startY = input.y

    -- On mouse up
        if drag.active
         
            ended=True
            active=False
        
        else if -- !drag.active && 
           !drag.ended && 
           drag.startX>-1 && 
           drag.startY>-1 && 
           (drag.startX!=input.x || drag.startY!=input.y)
        drag.active=True
        
        else
            click = true
            drag.strartX = -1
            drag.startY = -1
    
    XX -- On mouse move
    XX    if drag.ended:
    XX        startX=-1
    XX        startY=-1

    input.drag(active=False, ended=False, startX, startY)

+ Draw FPS
. Draw Map
. Implement Camera
. How to use SDL_SetPaletteColors(surface->format->palette, newColors, startIndex, endIndex)
. Make better Rectangle class with fill and border
