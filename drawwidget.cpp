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
    connect(&clickTimer, &QTimer::timeout, this, &DrawWidget::enableClick); // If timer finishes, enable click function.
}


DrawWidget::~DrawWidget()
{

}


void DrawWidget::drawPixel(QPoint pt)
{
    QPainter linePainter(&m_canvas);
    QRgb value = m_drawColor.rgb();

    customizePen(value);
    linePainter.setPen(pen);

    if (previousPt.isNull()) // If point does not initialized, initialize it.
    {
        // m_canvas.setPixel(pt.x(), pt.y(), value);
        this->previousPt = pt;
    }
    else
    {
        linePainter.drawLine(previousPt, pt);
        this->previousPt = pt;
    }

    points += 1;
    qDebug() << "x: " << pt.x() << "\t" << "y: " << pt.y() << "\t" << Qt::endl;


}


void DrawWidget::clear()
{
    clearCanvas(m_canvas, width(), height());
    update();

    points = 0;
    previousPt = QPoint();

    resetPen();
}


void DrawWidget::customizePen(QRgb value)
{
    pen.setColor(value);
    pen.setWidth(5); // Set pen width to 5.
}

void DrawWidget::resetPen() // If canvas is resetted, set pen color to black.
{
    QColor color(0, 0, 0); // Create black QColor object
    QRgb value = color.rgb(); // get QColor objects rgb values

    setDrawColor(color);
    pen.setColor(value); // Set it to pen's color.
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

    qDebug() << "Total number of points: " << this->points << Qt::endl;
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
