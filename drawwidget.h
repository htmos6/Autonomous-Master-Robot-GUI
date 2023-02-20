#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QPen>
#include <QPainter>

class QPaintEvent;
class QMouseEvent;
class DrawWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit DrawWidget(QWidget *parent = 0);
        ~DrawWidget();

        void drawPixel(QPoint pt);
        void printPoints();
        //unsigned int pixels[1920][1080];

    public slots:
        void clear();
        QColor drawColor();
        void setDrawColor(QColor color);

    signals:
    protected:
        void paintEvent(QPaintEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void resizeEvent(QResizeEvent *);

        void customizePen(QPen &customizePen, QRgb value, int width);
        void resetPen();
        void drawTriangle(QPoint previousTrianglePt, QPoint currentTrianglePt, int angle, int length);
        void enableClick();

    private:
        QPixmap m_pixmap;
        QImage m_canvas;
        QColor m_drawColor;
        QPen pen;

        QImage triangleCanvas;
        QPen trianglePen;

        QPoint previousPt;
        QTimer clickTimer;

        int points = 0;

};

#endif // DRAWWIDGET_H
