//
// Created by Drew Ingebretsen on 9/11/14.
// Copyright (c) 2014 CS6400. All rights reserved.
//


#include <functional>
#include <vector>
#include <algorithm>

#ifndef __Animation_H_
#define __Animation_H_


template<typename Keyframeable>
class Animateable {

public:

    typedef std::function<Keyframeable(Keyframeable value, double time)> AnimatorFunction;

    AnimatorFunction animatorFunction;

    void addFrame(double time, Keyframeable property){
        frames.push_back(std::make_pair(time,property));
        std::sort(frames.begin(),frames.end(),
                [](Frame a, Frame b)
                {return a.first < b.first;});
    }

    const Keyframeable operator()(int time) const {
        return getValueAtTime((double)time);
    }

    const Keyframeable operator()(double time) const {
        return getValueAtTime(time);
    }


    const Keyframeable getValueAtTime(double time) const {
        if(time < frames.front().first)
        {
            return frames.front().second;
        }
        auto lastFrame = std::find_if(frames.begin(),frames.end(),[&](Frame a)
        {
            return a.first > time;
        });
        if(lastFrame == frames.end())
        {
            return frames.back().second;
        }
        auto firstFrame = lastFrame;
        --firstFrame;
        auto firstTime = firstFrame->first;
        auto lastTime = lastFrame->first;
        auto lastPart = (time - firstTime)/(lastTime - firstTime);
        auto firstPart = 1 - lastPart;
        Keyframeable value = static_cast<Keyframeable>(firstFrame->second * firstPart + lastFrame->second * lastPart);

        if (animatorFunction){
            return animatorFunction(value, time);
        }
        else{
            return value;
        }


    }

private:
    typedef std::pair<double,Keyframeable> Frame;

    std::vector<Frame>frames;
};


#endif //__Animation_H_
