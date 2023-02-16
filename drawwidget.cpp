#include "drawwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>

static void clearCanvas(QImage &canvas, int width, int height)
{
    canvas = QImage(width,height,QImage::Format_RGB888);
    canvas.fill(QColor(Qt::white));
}

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    m_drawColor = QColor(Qt::black);
    clearCanvas(m_canvas, width(), height());
}

DrawWidget::~DrawWidget()
{

}

void DrawWidget::drawPixel(QPoint pt){
    QRgb value = m_drawColor.rgb();
    m_canvas.setPixel(pt.x(),pt.y(),value);
}

void DrawWidget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        drawPixel(event->pos());
        repaint();
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        drawPixel(event->pos());
        repaint();
    }
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QImage newCanvas;
    clearCanvas(newCanvas, width(), height());

    QPainter p(&newCanvas);
    p.drawImage(0,0,m_canvas);

    m_canvas = newCanvas;

    update();
}

QColor DrawWidget::drawColor()
{
    return m_drawColor;
}

void DrawWidget::setDrawColor(QColor color)
{
    m_drawColor = color;
}

void DrawWidget::clear()
{
    clearCanvas(m_canvas, width(), height());
    update();
}

void DrawWidget::printPoints(){
    for (int y = 0; y < m_canvas.height(); ++y) {
        unsigned char *line = m_canvas.scanLine(y);
        for (int x = 0; x < m_canvas.width(); ++x) {
            unsigned char rgb = line[x];
            if (rgb == 0x0)
                qDebug() << "x: " << x << "\t" << "y: " << y << "\t" << QString("0x%1").arg(rgb, 8, 16, QLatin1Char( '0' )) << Qt::endl;
        }
    }
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap::fromImage(m_canvas));
}

