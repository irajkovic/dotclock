#pragma once

#include <chrono>
#include <thread>
#include <vector>

#include "face.hpp"

namespace Faces
{

class Runner
{
    std::vector<std::unique_ptr<Faces::Face>> &mFaces;
    Face &mSeparator;

    public:
    Runner(std::vector<std::unique_ptr<Faces::Face>> &faces, Face &separator)
        : mFaces(faces), mSeparator(separator)
    {
    }

    void run()
    {
        for (;;) {
            for (auto &face : mFaces) {

                animate(&mSeparator);
                animate(face.get());

                std::this_thread::sleep_for(face->transitionSleep());
            }
        }
    }

    void animate(Face *face)
    {
        face->prepare();
        while (face->run()) {
            std::this_thread::sleep_for(face->animationSleep());
        }
    }
};

} /* namespace Faces */
