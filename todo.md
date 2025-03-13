. Restore cameraFollow from git commit history
. Fix diagonal movment (Shrink if diagal movement detected both vectors with 40%)
. Alt + Enter = Toggle Fullscreen
. Mouse scroll = Zoom in / Zoom out from mouse position
. Character movement A W S D
. F5 - Regenerate
. Arrow Keys - Scroll camera with cooldown
. Camera zoom cooldown config

-------------------------------


Unit now has
position - Which descirbes position of an object in the map, position is x,y,w,h collisions are calculated based on this
selectPosition - Means which region is effective during unit selection, it is relative to units position
renderPosition - Means from where are we rendering the unit sprite, relative to units worl map position

position is stored inside map cells grid[x][y][layer][unit]

unit: object
    
    unit.hitbox - is not needed because objects x, y, width, height is hitbox itself
    unit.selectbox
    unit.viewbox
    unit.position

    unit is rendered


    object 
        [object does not require hitbox selectbox and viewbox because is just rendered in getViewRect()]
        should be simply renderable
        has x, y, width, height
        has id if accessed



unit


object.hitbox
object.selecbox
object.view
object.position

object.renderArea

object.width
object.height
object.x
object.y

object.view.x
object.view.y
object.view.width
object.view.height
object.view.parent_x
object.view.parent_y
object.view.parent_width
object.view.parent_height
object.view.width_ratio
object.view.height_ratio


object.selecbox.x
object.

What I think now can be done is a Server class and Client class
Then I will need to implement TCP/IP connection between two games running

Server class should be deployable separately

The massive world problem in game framework:
--------------------------------------------------------
    So what I have is scene.objects and three major cycles:
        1. scene.update() - Updates all objects (Calculates new state, like position etc, etc)
        2. scene.render() - Renders all objects that are visible
        3. Also I have minimap and minimap.update() also checks all objects and draws on minimap

    If the world objects are few everything works perfectly but if I add millions of objects, in other words if the number of objects scales the fps is starting to drop

    I need to come up with some way to iterate less in a massive array of objects

    I have
    object.getX()
    object.getY()
    object.getWidth()
    object.getHeight()
    object.getRenderX()
    object.getRenderY()
    object.getRenderWidth()
    object.getRenderHeight()
    or
    object.getCollisionArea() - x, y, w, h - This is position + Collision rectangle
    object.getRenderArea() - x, y, w, h - This is render area and it might differ from position
    object.getSelectionArea()

    If I store objects in world[x][y] where x,y is a square with predetermined width, I can quickly find if this object is on screen or on minimap, like I add object to world[object.getX()][object.getY()]

But first problem is Collisions and RenderRect and Position?

. Select objects
. Center camera
. Should objects have id?
. Where is global object storage? in Scene
. Object destroy should remove objects
. Select objects
. Delete objects
. Create objects
. Change terrain
. Collision rectangle, Selection rectangle, render frame

So what I do have now is object->position
And I render body into position rectangle
But what I should have is a getRenderRectangle()

getCollisionRect() - Gets collision rect 
getRenderRect() - Get render rect to check where are we rendering
getPositionRect() - Get position rectangle
getSelectionRect() - Get the selection rectangle


ახლა მაქვს getPoisition()
რომელსაც იყენებს თვითონ ობიექტი რომ გაიგოს უნდა დაიხატოს თუ არ

უნდა გადავარჩიო რომელი მჭირდება მოკლედ და გავიაზრო რომელი რას აკეთებს
ეს იდეები მოვიდა ფაქტორიოდან

ადრეც ვხვდებოდი რომ რაც რენდერდება სხვა არის და პოზიცია შეიძლება პატარა იყოს
მინიმუმ ეს ორი უნდა განვასხვავო
პოზიცია არის უფრო პერსონაჟის ფეხები, რუკასთან შეხების ადგილი

რატომ განსხვავდება Collision-ი პოზიციისგან?

Old
-------------
. How to use SDL_SetPaletteColors(surface->format->palette, newColors, startIndex, endIndex)
