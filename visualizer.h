#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <QGraphicsView>

#include <vector>

#include "rectangleobj.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Visualizer; }
QT_END_NAMESPACE



class Visualizer : public QMainWindow
{
    Q_OBJECT

public:
    Visualizer(QWidget *parent = nullptr);
    QGraphicsScene *visualizerScene;
    ~Visualizer();

private:
    static const int RECTANGLEOBJSIZE = 74;
    RectangleObj *rectangleObjects[RECTANGLEOBJSIZE];
    Ui::Visualizer *ui;
    enum SortingAlgorithm {
        BUBBLESORT, SELECTIONSORT, INSERTIONSORT, QUICKSORT, MERGESORT, RADIXSORT
    };

    enum class BarColors {
        GREEN = 0, RED = 1, BLUE = 2, WHITE = 3, PINK = 4
    };

    enum PointerColors {
        RED = 0, GREEN = 1, BLUE = 2, WHITE = 3, PINK = 4
    };

    QBrush pointerColor;
    QBrush barColor;
    int delayTime_ms;
    bool isVisualizerRunning;

    bool pauseTrigger;

    // Parameters: n of (bestcase, avgcase, worstcase, memoryn)
    void updateLabelInfo(QString, QString, QString, QString);

    void swapItems(int firstIndex, int secondIndex);

    void markPointers(const std::vector<int*> &);
    void delayMS();

    // Sorting Algorithms
    void bubbleSort();
    void selectionSort();
    void insertionSort();
private slots:
    void initializeSort();
    void updateSortInfo();
    void createArray();
    void changeBarColor();
    void forceQuit();
    void changePointerBarColor();
    void pauseVisualizer();
};
#endif // VISUALIZER_H
