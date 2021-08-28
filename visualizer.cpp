#include "visualizer.h"
#include "ui_visualizer.h"

#include <QTime>
#include <QEventLoop>
#include <cstdlib>

#include <iostream>

Visualizer::Visualizer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Visualizer)
{
    ui->setupUi(this);
    pauseTrigger = false;
    isVisualizerRunning = false;
    delayTime_ms = ui->sortingSpeed_slider->minimum();
    pointerColor = QBrush(Qt::red);
    barColor = QBrush(Qt::green);
    /*
     0 - Bubble sort
     1 - Selection sort
     2 - Insertion sort
     3 - Quicksort
     4 - Merge Sort
     5 - Radix Sort
    */
    // Default bar colors

    // Setting up the scene
    visualizerScene = new QGraphicsScene;
    ui->visualizerGraphicsView->setScene(visualizerScene);

    ui->visualizerGraphicsView->setInteractive(false);


    QTransform transform(ui->visualizerGraphicsView->transform());
    qreal m11 = transform.m11();    // Horizontal scaling
    qreal m12 = transform.m12();    // Vertical shearing
    qreal m13 = transform.m13();    // Horizontal Projection
    qreal m21 = transform.m21();    // Horizontal shearing
    qreal m22 = transform.m22();    // vertical scaling
    qreal m23 = transform.m23();    // Vertical Projection
    qreal m31 = transform.m31();    // Horizontal Position (DX)
    qreal m32 = transform.m32();    // Vertical Position (DY)
    qreal m33 = transform.m33();    // Addtional Projection Factor
    // Horizontal flip
    m11 = -m11;
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
    ui->visualizerGraphicsView->setTransform(transform);
    // Horizontal transformation source code credit: https://gerrysweeney.com/horizontal-and-vertical-flip-transformations-of-a-qgraphicsitem-in-qt-qgraphicsview/
    ui->visualizerGraphicsView->rotate(180);
    // Generate our array
    for(int i = 0; i < RECTANGLEOBJSIZE; i++) {
        rectangleObjects[i] = new RectangleObj(rand() % 250 + 15, i * 10, barColor);
        visualizerScene->addItem(rectangleObjects[i]->rectanglePtr());
    }
    // Connect buttons and slots
    connect(ui->algorithmType_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSortInfo()));
    connect(ui->barColor_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBarColor()));
    connect(ui->forceQuit_button, SIGNAL(pressed()), this, SLOT(forceQuit()));
    connect(ui->pointerColor_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePointerBarColor()));
    connect(ui->pause_button, SIGNAL(pressed()), this, SLOT(pauseVisualizer()));
    connect(ui->sortButton, SIGNAL(pressed()), this, SLOT(initializeSort()));
    connect(ui->createArray_button, SIGNAL(pressed()), this, SLOT(createArray()));
}

void Visualizer::createArray() {
    if(isVisualizerRunning == true) return;
    for(int i = 0; i < RECTANGLEOBJSIZE; i++) {
        visualizerScene->removeItem(rectangleObjects[i]->rectanglePtr());
        delete rectangleObjects[i];
    }
    for(int i = 0; i < RECTANGLEOBJSIZE; i++) {
        rectangleObjects[i] = new RectangleObj(rand() % 250 + 15, i * 10, barColor);
        visualizerScene->addItem(rectangleObjects[i]->rectanglePtr());
    }
}

void Visualizer::updateSortInfo() {
    switch(ui->algorithmType_comboBox->currentIndex()) {
        case(BUBBLESORT):
            updateLabelInfo("n²", "n²", "n²", "1");
            break;
        case(SELECTIONSORT):
            updateLabelInfo("n²", "n²", "n²", "1");
            break;
        case(INSERTIONSORT):
            updateLabelInfo("n", "n²", "n²", "1");
            break;
        case(QUICKSORT):
            updateLabelInfo("nlogn", "nlogn", "n²", "logn");
            break;
        case(MERGESORT):
            updateLabelInfo("nlogn", "nlogn", "nlogn", "n");
            break;
        case(RADIXSORT):
            updateLabelInfo("nk", "nk", "nk", "n+k");
            break;
    }
}

void Visualizer::updateLabelInfo(QString nBestCase, QString nAvgCase, QString nWorstCase, QString nMem) {
    QString default_bc = "Best Case: ";
    QString default_ac = "Average Case: ";
    QString default_wc = "Worst Case: ";
    QString default_m = "Memory: ";
    ui->bestCase_label->setText(default_bc + "O(" + nBestCase + ")");
    ui->averageCase_label->setText(default_ac + "O(" + nAvgCase + ")");
    ui->worstCase_label->setText(default_wc + "O(" + nWorstCase + ")");
    ui->spaceO_label->setText(default_m + "O(" + nMem + ")");
}

void Visualizer::changeBarColor() {
    /*Int for Bar Colors
    0 - Green (Default)
    1 - Red
    2 - Blue
    3 - White
    4 - Orange
    5 - Pink
    6 - Purple
    */
    BarColors bc = static_cast<BarColors>(ui->barColor_comboBox->currentIndex());
    switch(bc) {
        case(BarColors::GREEN):
            barColor = QBrush(Qt::green);
            break;
        case(BarColors::RED):
            barColor = QBrush(Qt::red);
            break;
        case(BarColors::BLUE):
            barColor = QBrush(Qt::cyan);
            break;
        case(BarColors::WHITE):
            barColor = QBrush(Qt::white);
            break;
        case(BarColors::PINK):
            barColor = QBrush(Qt::magenta);
            break;
    }

    for(int i = 0 ; i < RECTANGLEOBJSIZE; i++) {
        rectangleObjects[i]->updateFillColor(barColor);
    }
}

void Visualizer::changePointerBarColor() {
    QBrush newPtrColor;
    PointerColors pc = static_cast<PointerColors>(ui->pointerColor_comboBox->currentIndex());
    switch(pc) {
        case(PointerColors::RED):
            pointerColor = QBrush(Qt::red);
            break;
        case(PointerColors::GREEN):
            pointerColor = QBrush(Qt::green);
            break;
        case(PointerColors::BLUE):
            pointerColor = QBrush(Qt::blue);
            break;
        case(PointerColors::WHITE):
            pointerColor = QBrush(Qt::white);
            break;
        case(PointerColors::PINK):
            pointerColor = QBrush(Qt::magenta);
            break;
    }
}

void Visualizer::swapItems(int firstIndex, int secondIndex) {
    // Remove both rectangles from the Graphics Scene
    if(firstIndex == secondIndex) {
        return; // we return, no reason to swap the same indexes y'know
    }
    visualizerScene->removeItem(rectangleObjects[firstIndex]->rectanglePtr());
    visualizerScene->removeItem(rectangleObjects[secondIndex]->rectanglePtr());
    // Swap the values of the rectangles and reconstruct the QGraphicsRectItem
    rectangleObjects[firstIndex]->swapRectangleValues(rectangleObjects[secondIndex]); // dereference arg

    // Swapping the positions of the pointer
    RectangleObj *objCopy = rectangleObjects[firstIndex]; // temporary pointer of the first rectangle
    // Swap the rectangleObj pointers' positions in the array

    rectangleObjects[firstIndex] = rectangleObjects[secondIndex];
    rectangleObjects[secondIndex] = objCopy;
    objCopy = nullptr; // no longer needing this pointer

    // Add both items back into the scene
    visualizerScene->addItem(rectangleObjects[firstIndex]->rectanglePtr());
    visualizerScene->addItem(rectangleObjects[secondIndex]->rectanglePtr());
}


void Visualizer::forceQuit() {
    exit(EXIT_FAILURE);
}


void Visualizer::markPointers(const std::vector<int*> &pointerIndexes) {
    for(unsigned int i = 0; i < pointerIndexes.size(); i++) {
        rectangleObjects[*pointerIndexes[i]]->updateFillColor(pointerColor);
        // here
        delayMS();
    }
    for(unsigned int i = 0; i < pointerIndexes.size(); i++) {
        rectangleObjects[*pointerIndexes[i]]->updateFillColor(barColor);
    }
}

void Visualizer::delayMS() {
    delayTime_ms = ui->sortingSpeed_slider->value();
    QTime resumeTime = QTime::currentTime().addMSecs(delayTime_ms);
    while(QTime::currentTime() < resumeTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, delayTime_ms);
    }
}

void Visualizer::pauseVisualizer() {
    pauseTrigger = true;
}

void Visualizer::initializeSort() {
    if(isVisualizerRunning == true) return;
    isVisualizerRunning = true;
    switch(ui->algorithmType_comboBox->currentIndex()) {
        case(BUBBLESORT):
            bubbleSort();
            break;
        case(SELECTIONSORT):
            selectionSort();
            break;
        case(INSERTIONSORT):
            insertionSort();
            break;
        default:
            return;
    }
    isVisualizerRunning = false;
    pauseTrigger = false;
}


/********************
Sorting Algorithms
*********************/

void Visualizer::bubbleSort() {
    int i = 0;
    int j = 0;
    int h = 0; // h = j + 1
    std::vector<int*> pointers;
    pointers.push_back(&i);
    pointers.push_back(&j);
    pointers.push_back(&h);
    for(i = 0; i < RECTANGLEOBJSIZE - 1; i++) {
        for(j = 0; j < RECTANGLEOBJSIZE - 1; j++) {
            h = j + 1;
            markPointers(pointers);
            if(pauseTrigger == true) return;
            if(*rectangleObjects[j] > *rectangleObjects[h]) {
                swapItems(j, h);
            }
        }
        if(pauseTrigger == true) return;
    }
}

void Visualizer::selectionSort() {
    std::vector<int*> pointers;
    int i = 0;
    int j = 0;
    int smallIndex = 0;
    pointers.push_back(&i);
    pointers.push_back(&j);
    pointers.push_back(&smallIndex);
    for(i = 0; i < RECTANGLEOBJSIZE - 1; i++) {
        smallIndex = i;
        for(j = i + 1; j < RECTANGLEOBJSIZE; j++) {
            if(pauseTrigger == true) return;
            markPointers(pointers);
            if(*rectangleObjects[j] < *rectangleObjects[smallIndex]) {
                smallIndex = j;
            }
        }
        swapItems(smallIndex, i);
    }
}

void Visualizer::insertionSort() {
    std::vector<int*> pointers;
    int i = 0;
    int j = 0;
    pointers.push_back(&i);
    pointers.push_back(&j);
    for(i = 1; i < RECTANGLEOBJSIZE; i++) {
        j = i;
        while(j > 0 && *rectangleObjects[j - 1] > *rectangleObjects[j]) {
            if(pauseTrigger == true) return;
            markPointers(pointers);
            swapItems(j - 1, j);
            j--;
        }
    }
}

// Destructor
Visualizer::~Visualizer() {
    for(int i = 0; i < RECTANGLEOBJSIZE; i++) {
        delete rectangleObjects[i];
    }
    delete visualizerScene;
    delete ui;
}

