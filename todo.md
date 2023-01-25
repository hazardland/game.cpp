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
