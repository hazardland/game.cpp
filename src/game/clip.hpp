#ifndef GAME_CLIP
#define GAME_CLIP

using namespace std;

#include <iostream>

#include <game/image.hpp>
#include <game/frame.hpp>

/**
 * @brief Stores frame data which are later animated from animation using sprite
 * 
 */
class Clip {
    public:
        Frame* frames;
        int frameCount;
        int framePause = 60;
        // int width;
        // int height;

        Clip(Image* image,
                  int frameWidth,
                  int frameHeight,
                  int startCell,
                  int startRow,
                  int frameCount=1,
                  int framePause=60,
                  bool flipX=false,
                  bool flipY=false,
                  bool readVertically=false) {

            // cout << "Creating clip\n";

            this->framePause = framePause;
            this->frameCount = frameCount;
            frames = new Frame[frameCount];
            int cellCount = image->getWidth()/frameWidth;
            int rowCount = image->getHeight()/frameHeight;
            int cell = startCell;
            int row = startRow;
            for (size_t i = 0; i < frameCount; i++)
            {
                frames[i].rect.x = (cell-1) * frameWidth;
                frames[i].rect.y = (row-1) * frameHeight;
                frames[i].rect.w = frameWidth;
                frames[i].rect.h = frameHeight;
                frames[i].flipX = flipX;
                frames[i].flipY = flipY;
                frames[i].cacheFlip();

                // cout << image << " Frame "<< i << " vertical: " <<  readVertically << " x:" << frames[i].rect.x << " y:" << frames[i].rect.y << " w:" << frames[i].rect.w << " h:" << frames[i].rect.h << "\n";

                if (i+1==frameCount) {
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
        Frame* getFrame(int frameIndex){
            //cout << frame << " ";
            return &frames[frameIndex];
        }
        int getFramePause() {
            return framePause;
        }
        ~Clip() {
            // cout << "Clip is being destroyed";
        }
};

#endif
