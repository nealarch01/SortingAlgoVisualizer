#ifndef RECTANGLEOBJ_H
#define RECTANGLEOBJ_H


#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QColor>

class RectangleObj : public QGraphicsRectItem {
private:
    int rectangleValue; // number used for sorting
    qreal X; // x coordinate
    qreal Y; // y coordinate (our y coordinate will stay at the bottom so remains y = 0)
    qreal height; // height of the rectangle (x10 than the rectangleValue
    qreal width; // will always remain 10
    QBrush fillColor;
    QPen outlineColor;

    QGraphicsRectItem *rectangle;
public:
    // Parameters: Rectangle value and x coordinate
    RectangleObj(int rValue, qreal xCoord, QBrush bColor);
    ~RectangleObj();

    // Modifier functions
    void updateFillColor(QBrush);
    void updateOutlineColor(QPen);
    void swapRectangleValues(RectangleObj *&);

    // Getter functions
    QGraphicsRectItem* rectanglePtr() const;
    int value() const;
    bool operator<(const RectangleObj &otherRec);
    bool operator>(const RectangleObj &otherRec);
};

#endif // RECTANGLEOBJ_H
