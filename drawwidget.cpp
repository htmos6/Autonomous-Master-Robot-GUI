#include "drawwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QRgb>
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
    clearCanvas(triangleCanvas, width(), height());
    connect(&clickTimer, &QTimer::timeout, this, &DrawWidget::enableClick); // If timer finishes, enable click function.
}


DrawWidget::~DrawWidget()
{

}


void DrawWidget::drawPixel(QPoint pt)
{
    QPainter linePainter(&m_canvas);
    QRgb lineValue = m_drawColor.rgb();

    customizePen(pen, lineValue, 5);
    linePainter.setPen(pen);

    if (previousPt.isNull()) // If point does not initialized, initialize it.
    {
        // m_canvas.setPixel(pt.x(), pt.y(), lineValue);
        previousPt = pt;
        linePainter.end();
    }
    else
    {
        linePainter.drawLine(previousPt, pt);
        linePainter.end();
        drawTriangle(previousPt, pt, 40, 100);
        previousPt = pt;
    }

    points += 1;
    qDebug() << "x: " << pt.x() << "\t" << "y: " << pt.y() << "\t" << Qt::endl;
}


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

    trianglePainter.drawLine(v1, v2);
    trianglePainter.drawLine(v1, v3);
    trianglePainter.drawLine(v2, v3);

    // Create a QPainter object to paint the result image
    QPainter resultPainter(&m_canvas);

    // Draw the triangle image on top of the m_canvas image
    resultPainter.drawImage(0, 0, triangleCanvas);
    update();
}


void DrawWidget::clear()
{
    clearCanvas(m_canvas, width(), height());
    update();

    points = 0;
    previousPt = QPoint();

    resetPen();
}


void DrawWidget::customizePen(QPen &currentPen, QRgb value, int width)
{
    currentPen.setColor(value);
    currentPen.setWidth(width); // Set pen width to 5.
}


void DrawWidget::resetPen() // If canvas is resetted, set pen color to black.
{
    QColor color(0, 0, 0); // Create black QColor object
    QRgb valueBlack = color.rgb(); // Get QColor objects rgb values

    setDrawColor(color);
    customizePen(pen, valueBlack, 5); // Customize corresponding pen
}


void DrawWidget::printPoints()
{
    for (int y = 0; y < m_canvas.height(); ++y)
    {
        unsigned char *line = m_canvas.scanLine(y);
        for (int x = 0; x < m_canvas.width(); ++x)
        {
            unsigned char rgb = line[x];
            if (rgb == 0x0)
                ;// qDebug() << "x: " << x << "\t" << "y: " << y << "\t" << QString("0x%1").arg(rgb, 8, 16, QLatin1Char( '0' )) << Qt::endl;
        }
    }

    qDebug() << "Total number of points: " << points << Qt::endl;
}


void DrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
    // painter.drawPixmap(0,0,QPixmap::fromImage(triangleCanvas));
}


void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        drawPixel(event->pos());
        repaint();
        clickTimer.start(100); // start the timer with a 100 ms interval
        setEnabled(false); // disable mouse click function
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
    setEnabled(true); // re-enable mouse click function
    clickTimer.stop(); // stop the timer
}
