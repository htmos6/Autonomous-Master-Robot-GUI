#include "drawwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QThread>

static void clearCanvas(QImage &canvas, int width, int height)
{
    canvas = QImage(width, height, QImage::Format_RGB888);
    canvas.fill(QColor(Qt::white));
}


DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    m_drawColor = QColor(Qt::black);
    clearCanvas(m_canvas, width(), height());
    // connect(&clickTimer, &QTimer::timeout, this, &DrawWidget::enableClick); // If timer finishes, enable click function.
}


DrawWidget::~DrawWidget()
{

}


void DrawWidget::drawPixel(QPoint pt)
{
    QPainter linePainter(&m_canvas);
    QRgb lineValue = m_drawColor.rgb();

    customizePen(pen, lineValue, 1, "custom");
    linePainter.setPen(pen);


    if (previousPt.isNull()) // If previous point does not initialized, initialize it.
    {
        previousPt = pt;
        prePt10 = pt;
        points += 1;
        qDebug() << "x: " << prePt10.x() << "\t" << "y: " << prePt10.y() << "\t" << Qt::endl;
    }
    else
    {
        points += 1;
        if (points % 10 == 0)
        {
            customizePen(pen, lineValue, 3, "red"); // lineValue is a black but inside function it is modified as a red.
            linePainter.setPen(pen);
            linePainter.drawLine(previousPt, pt);

            curPt10 = pt;
            calculateAngleDistance(prePt10, curPt10);
            qDebug() << "x: " << curPt10.x() << "\t" << "y: " << curPt10.y() << "\t" << Qt::endl;
            prePt10 = curPt10;
        }
        else 
        {
            linePainter.drawLine(previousPt, pt);
        }
        previousPt = pt;
    }

}


void DrawWidget::calculateAngleDistance(QPoint prePt10, QPoint curPt10)
{
    // Calculate the bisector vector
    QPoint bisectorVector;
    bisectorVector.setX(curPt10.x() - prePt10.x());
    bisectorVector.setY(curPt10.y() - prePt10.y());

    // Calculate the angle of the bisector vector
    qreal bisectorVectorAngle = qRound(qRadiansToDegrees(qAtan2(bisectorVector.y(), bisectorVector.x())));
    qreal bisectorVectorLength = qRound(qSqrt(qPow(bisectorVector.x(), 2) + qPow(bisectorVector.y(), 2)));
    qDebug() << "Angle: " << bisectorVectorAngle << "Length: " << bisectorVectorLength << Qt::endl;
}


void DrawWidget::clear()
{
    clearCanvas(m_canvas, width(), height());
    update();

    points = 0;
    previousPt = QPoint();
    prePt10 = QPoint();
    curPt10 = QPoint();

    resetPen();
}


void DrawWidget::customizePen(QPen &currentPen, QRgb valueCustom, int width, QString colorName)
{
    QColor colorRed(255, 0, 0); // Create red QColor object
    QColor colorBlack(0, 0, 0); // Create black QColor object
    QRgb valueRed = colorRed.rgb(); // Get QColor objects rgb red
    QRgb valueBlack = colorBlack.rgb(); // Get QColor objects rgb black

    if (colorName == "red")
    {
        currentPen.setColor(valueRed);
        currentPen.setWidth(width); // Set pen width to 5.
    }
    else if (colorName == "black")
    {
        currentPen.setColor(valueBlack);
        currentPen.setWidth(width); // Set pen width to 5.
    }
    else
    {
        currentPen.setColor(valueCustom);
        currentPen.setWidth(width); // Set pen width to 5
    }


}


void DrawWidget::resetPen() // If canvas is resetted, set pen color to black.
{
    QColor color(0, 0, 0); // Create black QColor object
    QRgb valueBlack = color.rgb(); // Get QColor objects rgb values

    setDrawColor(color);
    customizePen(pen, valueBlack, 1, "black"); // Customize corresponding pen
}


void DrawWidget::printPoints()
{
    qDebug() << "Total number of points: " << points << Qt::endl;
}


void DrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
}


void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        drawPixel(event->pos());
        repaint();
        // start the timer with a 100 ms interval
        // clickTimer.start(10);
        // setEnabled(true); // disable mouse click function
    }
}


void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        drawPixel(event->pos());
        repaint();
    }
}


QColor DrawWidget::drawColor()
{
    return m_drawColor;
}


void DrawWidget::setDrawColor(QColor color)
{
    m_drawColor = color;
}


void DrawWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QImage newCanvas;
    clearCanvas(newCanvas, width(), height());

    QPainter p(&newCanvas);
    p.drawImage(0, 0, m_canvas);

    m_canvas = newCanvas;
    update();
}

void DrawWidget::enableClick()
{
    ;
    /*
    setEnabled(true); // re-enable mouse click function
    clickTimer.stop(); // stop the timer
    */
}















/*
    void DrawWidget::drawTriangle(QPoint previousTrianglePt, QPoint currentTrianglePt, int angle, int length)
    {
        QPainter trianglePainter(&m_canvas);
        QColor colorTriangle(0, 0, 255); // Create BLUE QColor object
        QRgb valueTriangle = colorTriangle.rgb(); // get QColor objects rgb values

        // Set the pen for the triangle painter
        customizePen(trianglePen, valueTriangle, 3);
        trianglePainter.setPen(trianglePen);

        triangleCanvas = QImage(m_canvas.size(), QImage::Format_ARGB32_Premultiplied); // Create a new QImage object for the triangle

        // Calculate the bisector vector
        QPoint bisectorVector;
        bisectorVector.setX(currentTrianglePt.x() - previousTrianglePt.x());
        bisectorVector.setY(currentTrianglePt.y() - previousTrianglePt.y());

        // Calculate the angle of the bisector vector
        qreal bisectorVectorAngle = qRadiansToDegrees(qAtan2(bisectorVector.y(), bisectorVector.x()));

        // Calculate the angles of the adjacent vertices
        qreal vertexXAngle = bisectorVectorAngle - angle;
        qreal vertexYAngle = bisectorVectorAngle + angle;

        // Calculate the positions of the adjacent vertices
        QPoint vertexX(currentTrianglePt.x() + length * qCos(qDegreesToRadians(vertexXAngle)),
            currentTrianglePt.y() + length * qSin(qDegreesToRadians(vertexXAngle)));
        QPoint vertexY(currentTrianglePt.x() + length * qCos(qDegreesToRadians(vertexYAngle)),
            currentTrianglePt.y() + length * qSin(qDegreesToRadians(vertexYAngle)));

        // Draw the triangle on the triangle image
        QPoint v1(currentTrianglePt.x(), currentTrianglePt.y());
        QPoint v2(vertexX.x(), vertexX.y());
        QPoint v3(vertexY.x(), vertexY.y());

        // trianglePainter.drawLine(v1, v2);
        // trianglePainter.drawLine(v1, v3);
        // trianglePainter.drawLine(v2, v3);

        // Create a QPainter object to paint the result image
        QPainter resultPainter(&m_canvas);

        // Draw the triangle image on top of the m_canvas image
        resultPainter.drawImage(0, 0, triangleCanvas);
        update();
    }
*/