#ifndef TEST_SELECTBOX
#define TEST_SELECTBOX

#include <game/rectangle.hpp>

class Selectbox: public Rectangle {
using Rectangle::Rectangle;
    public:
        virtual void update(State* state) {

            Mouse* mouse = state->event->mouse; 
            if (mouse->leftDragActive) {
                show();
                if (mouse->x > mouse->leftDragStartX) {
                    setX(mouse->leftDragStartX);              
                    setWidth(mouse->x - mouse->leftDragStartX);
                } else {
                    setX(mouse->x);
                    setWidth(mouse->leftDragStartX- mouse->x);
                }
                if (mouse->y > mouse->leftDragStartY) {
                    setY(mouse->leftDragStartY);              
                    setHeight(mouse->y - mouse->leftDragStartY);
                } else {
                    setY(mouse->y);
                    setHeight(mouse->leftDragStartY - mouse->y);
                }                
                // cout << "Drawing rectangle "<< getX() << " " << getY() << " " << getWidth() << " " << getHeight() << "\n";
            } else {
                hide();
            }
        }
};
#endif
