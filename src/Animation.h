#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{  
    public:
        unsigned int m_index;
        unsigned int m_numFrames;
        unsigned int m_animationSpeed;
        Animation();
        Animation(unsigned int index, unsigned int numFrames, unsigned int animationSpeed);
};

#endif
