#ifndef GAME_CLIP_H
#define GAME_CLIP_H

class Frame;
class Image;

/**
 * @brief The Clip class encapsulates a collection of frames from a sprite sheet.
 *
 * This class is used to group a set of frames that can be played as an animation. Each clip
 * includes the frame data, frame count and frame pause time.
 */
class Clip {
public:
    // Frame data
    Frame* frames;

    // Total frames in the clip
    int frameCount;

    // Time delay between frames
    int framePause;

    // Functions declaration
    Clip(Image* image, int frameWidth, int frameHeight, int startCell, int startRow, int frameCount=1, int framePause=60, bool flipX=false, bool flipY=false, bool readVertically=false);
    int getFrameCount();
    Frame* getFrame(int frameIndex);
    int getFramePause();
    virtual ~Clip();
};

#endif // GAME_CLIP_H
