#pragma once

#include <chrono>

namespace Faces
{

/**
 * The Face is an abstract base class that is inherited by the actual clock
 * Face implementations.
 */
class Face
{
    public:
    /**
     * Virtual destructor is a must for polymorphic base class.
     */
    virtual ~Face() = default;

    /**
     * Should be called once before clock face animation loop is started, so
     * that clock face can prepare for drawing.
     */
    virtual void prepare() = 0;

    /**
     * Every call of this method renders one "frame" of the clock animation.
     * The method should be called in a loop until it returns false, which
     * signifies that one cycle of animation is done.
     *
     * @retval true  Animation is in progress, user should keep calling the
     *               method.
     * @retval false Animation is done, if called again, it will restart.
     */
    virtual bool run() = 0;

    /**
     * Returns Time before two animation frames.
     *
     * @return The chrono::duration to wait.
     */
    virtual std::chrono::duration<int, std::milli> animationSleep()
    {
        return std::chrono::duration<int, std::milli>(50);
    }

    /**
     * Returns Time before two animation cycles (the end of this animation and
     * the beginning of the next one). Used to keep the rendered content
     * on the physical display for some time before running the next animation.
     *
     * @return The chrono::duration to wait.
     */
    virtual std::chrono::duration<int, std::milli> transitionSleep()
    {
        return std::chrono::duration<int, std::milli>(2000);
    }
};

} // namespace Faces
