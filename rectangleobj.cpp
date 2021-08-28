#include "rectangleobj.h"

#include <iostream>

RectangleObj::RectangleObj(int rectangleValue, qreal X, QBrush bColor) {
    this->rectangleValue = rectangleValue;
    this->X = X;
    Y = 0;
    height = rectangleValue;
    width = 10;
    fillColor = bColor;
    outlineColor = QPen(Qt::black);
    outlineColor.setWidth(2);
    //QGraphicsRectItem params: X, Y, width, height
    rectangle = new QGraphicsRectItem(X, Y, width, height);
    rectangle->setBrush(fillColor);
    rectangle->setPen(outlineColor);
}

void RectangleObj::updateFillColor(QBrush fColor) {
    fillColor = fColor;
    rectangle->setBrush(fillColor);
}

void RectangleObj::updateOutlineColor(QPen oColor) {
    outlineColor = oColor;
    rectangle->setBrush(fillColor);
}

void RectangleObj::swapRectangleValues(RectangleObj *&otherRec) {
    if(rectangle == otherRec->rectangle) return;
    delete this->rectangle;
    delete otherRec->rectangle;
    rectangle = otherRec->rectangle = nullptr;
    // Reconstruct rectangle items
    int localX = X;
    X = otherRec->X;
    otherRec->X = localX;

    // Since the top is swapped, we can assign the corresponding rectangles
    rectangle = new QGraphicsRectItem(X, Y, width, height);
    otherRec->rectangle = new QGraphicsRectItem(otherRec->X, otherRec->Y, otherRec->width, otherRec->height);
    rectangle->setBrush(this->fillColor);
    otherRec->rectangle->setBrush(otherRec->fillColor);
}

bool RectangleObj::operator<(const RectangleObj &otherRec) {
    if(this->rectangleValue < otherRec.rectangleValue) return true;
    return false;
}

bool RectangleObj::operator>(const RectangleObj &otherRec) {
    if(this->rectangleValue > otherRec.rectangleValue) return true;
    return false;
}

QGraphicsRectItem* RectangleObj::rectanglePtr() const { return rectangle; }

int RectangleObj::value() const { return rectangleValue; }

RectangleObj::~RectangleObj() {
    delete rectangle;
}
