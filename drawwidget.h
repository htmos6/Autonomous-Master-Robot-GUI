#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QPen>

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
    void enableClick();
    void customizePen(QRgb value);
    void resetPen();

private:
    QColor m_drawColor;
    QPixmap m_pixmap;
    QImage m_canvas;

    QPoint previousPt;
    QTimer clickTimer;
    QPen pen;
    int points = 0;
};

#endif // DRAWWIDGET_H
