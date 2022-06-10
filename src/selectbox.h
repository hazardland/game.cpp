#ifndef WAR2_SELECTBOX_H
#define WAR2_SELECTBOX_H

#include <game/rectangle.h>

class Selectbox: public Rectangle {
using Rectangle::Rectangle;
    public:
        virtual void update(uint32_t delta, Input* input) {

            if (input->mouse->leftDragActive) {
                show();
                if (input->mouse->x > input->mouse->leftDragStartX) {
                    setX(input->mouse->leftDragStartX);              
                    setWidth(input->mouse->x - input->mouse->leftDragStartX);
                } else {
                    setX(input->mouse->x);
                    setWidth(input->mouse->leftDragStartX- input->mouse->x);
                }
                if (input->mouse->y > input->mouse->leftDragStartY) {
                    setY(input->mouse->leftDragStartY);              
                    setHeight(input->mouse->y - input->mouse->leftDragStartY);
                } else {
                    setY(input->mouse->y);
                    setHeight(input->mouse->leftDragStartY- input->mouse->y);
                }                
                // cout << "Drawing rectangle "<< getX() << " " << getY() << " " << getWidth() << " " << getHeight() << "\n";
            } else {
                hide();
            }
        }
};
#endif
