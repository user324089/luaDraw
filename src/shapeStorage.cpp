#include "shapeStorage.hpp"
#include <stdexcept>

shapeStorage::sizeMemento shapeStorage::exportSize () {
    sizeMemento result;
    result.pointNum = pointInstances.size();
    result.lineNum = lineInstances.size();
    result.circleNum = circleInstances.size();
    return result;
};

void shapeStorage::importSize (const sizeMemento & memento) {
    if (memento.pointNum > pointInstances.size() ||
            memento.lineNum > lineInstances.size() ||
            memento.circleNum > circleInstances.size()) {
        throw std::runtime_error ("shape storage cannot import bigger size than it has");
    }
    pointInstances.resize (memento.pointNum);
    lineInstances.resize (memento.lineNum);
    circleInstances.resize (memento.circleNum);
};

std::size_t shapeStorage::newPoint () {
    std::size_t id = pointInstances.size();
    pointInstances.emplace_back ();
    return id;
}
std::size_t shapeStorage::newLine () {
    std::size_t id = lineInstances.size();
    lineInstances.emplace_back ();
    return id;
}
std::size_t shapeStorage::newCircle () {
    std::size_t id = circleInstances.size();
    circleInstances.emplace_back ();
    return id;
}

shapeStorage::point & shapeStorage::getPoint (std::size_t id) {
    return pointInstances[id];
}
shapeStorage::line & shapeStorage::getLine (std::size_t id) {
    return lineInstances[id];
}
shapeStorage::circle & shapeStorage::getCircle (std::size_t id) {
    return circleInstances[id];
}
