#ifndef GAME_CLIP_H
#define GAME_CLIP_H

using namespace std;
#include <iostream>

#include <game/image.h>
#include <game/frame.h>

/**
 * @brief Stores frame data which are later animated from animation using sprite
 * 
 */
class Clip {
    public:
        Frame* frames;
        int frameCount;
        int pause = 60;
        // int width;
        // int height;

        Clip(Image* image,
                  int width,
                  int height,
                  int cell,
                  int row,
                  int count=1,
                  int pause=60,
                  bool flipX=false,
                  bool flipY=false,
                  bool readVertically=false) {

            // cout << "Creating clip\n";

            this->pause = pause;
            frameCount = count;
            frames = new Frame[count];
            int cellCount = image->getWidth()/width;
            int rowCount = image->getHeight()/height;
            for (size_t i = 0; i < count; i++)
            {
                frames[i].rect.x = (cell-1) * width;
                frames[i].rect.y = (row-1) * height;
                frames[i].rect.w = width;
                frames[i].rect.h = height;
                frames[i].flipX = flipX;
                frames[i].flipY = flipY;

                // cout << image << " Frame "<< i << " vertical: " <<  readVertically << " x:" << frames[i].rect.x << " y:" << frames[i].rect.y << " w:" << frames[i].rect.w << " h:" << frames[i].rect.h << "\n";

                if (i+1==count) {
                    break;
                }

                if (readVertically) {
                    if (row+1 > rowCount) {
                        row = 1;
                        cell += 1;
                    }
                    row += 1;
                }
                else {
                    if (cell+1 > cellCount) {
                        cell = 1;
                        row += 1;
                    }
                    cell += 1;
                }
            }
        }
        int getFrameCount() {
            return frameCount;
        }
        Frame* getFrame(int frame){
            //cout << frame << " ";
            return &frames[frame];
        }
        int getPause() {
            return pause;
        }
        ~Clip() {
            // cout << "Clip is being destroyed";
        }
};

#endif
