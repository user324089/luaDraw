#pragma once

#include <string>
#include <vector>

class shapeStorage {
    public:
        struct point {
            float x = 0, y = 0;
            int color = -1;
            std::string name = "";
        };
        struct line {
            float a = 1, b = 0, c = 0;
            int color = -1;
            std::string name = "";
        };
        struct circle {
            float x = 0, y = 0, r = 1;
            int color = -1;
            std::string name = "";
        };
    private:
        std::vector <point> pointInstances;
        std::vector <line> lineInstances;
        std::vector <circle> circleInstances;
    public:
        typedef std::vector <point>::iterator pointIterator;
        typedef std::vector <line>::iterator lineIterator;
        typedef std::vector <circle>::iterator circleIterator;

        pointIterator pointBegin () { return pointInstances.begin(); }
        pointIterator pointEnd () { return pointInstances.end(); }
        lineIterator lineBegin () { return lineInstances.begin(); }
        lineIterator lineEnd () { return lineInstances.end(); }
        circleIterator circleBegin () { return circleInstances.begin(); }
        circleIterator circleEnd () { return circleInstances.end(); }

        struct sizeMemento {
            private:
                std::size_t pointNum, lineNum, circleNum;
                friend class shapeStorage;
        };

        sizeMemento exportSize ();
        void importSize (const sizeMemento & memento);
        std::size_t newPoint ();
        std::size_t newLine ();
        std::size_t newCircle ();
        point & getPoint (std::size_t id);
        line & getLine (std::size_t id);
        circle & getCircle (std::size_t id);
};
