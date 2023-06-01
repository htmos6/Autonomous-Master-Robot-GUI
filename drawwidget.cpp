#include "drawwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QThread>
#include <QUdpSocket>

static void clearCanvas(QImage &canvas, int width, int height)
{
    canvas = QImage(width, height, QImage::Format_RGB888);
    canvas.fill(QColor(Qt::white));
    QPainter painter(&canvas);

    // Set the pen properties for the grid lines
    QPen pen(Qt::gray); // Set the color of the grid lines
    pen.setWidth(1);     // Set the width of the grid lines

    painter.setPen(pen);

    // Define the size and spacing of the grid cells
    int gridSize = 10;   // Size of each grid cell in pixels

    // Draw the vertical lines
    for (int x = 0; x < width; x += gridSize) {
        painter.drawLine(x, 0, x, height);
    }

    // Draw the horizontal lines
    for (int y = 0; y < height; y += gridSize) {
        painter.drawLine(0, y, width, y);
    }

    // Done drawing on the QImage
    painter.end();
}


DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    m_drawColor = QColor(Qt::black);
    clearCanvas(m_canvas, width(), height());
    // connect(&clickTimer, &QTimer::timeout, this, &DrawWidget::enableClick); // If timer finishes, enable click function.
    resetPen();
}


QString receive_broadcasted_ip(uint16_t port) {
    // Create a UDP socket for receiving broadcast messages
    QUdpSocket udpSocket;
    udpSocket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);

    // Join the multicast group to receive broadcast messages
    udpSocket.joinMulticastGroup(QHostAddress::Broadcast);

    // Wait for a broadcast message to be received
    QByteArray datagram;
    while (datagram.isEmpty()) {
        if (udpSocket.waitForReadyRead(3000)) {
            datagram.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(datagram.data(), datagram.size());
        } else {
            qWarning() << "Timed out waiting for broadcast message";
            return "";
        }
    }

    // Convert the received message to an IP address string
    QString ip_str = QString::fromLatin1(datagram);

    // Print the received IP address
    qDebug() << "Received broadcasted IP address:" << ip_str;

    return ip_str;
}


void DrawWidget::connectToPico(){
    /*pico = new PicoConnection(this);
    pico->connect("192.168.137.41", 10006);
    qDebug() << "test" << Qt::endl;*/
    QString ip = receive_broadcasted_ip(12345); //"100.81.244.102";

    if(ip == ""){
        qDebug() << "No Pico Found!" << Qt::endl;
        return;
    }
    pico = new PicoConnection(this);
    pico->connect(ip, 4242);
}


DrawWidget::~DrawWidget()
{
    delete pico;
}


void DrawWidget::drawPixel(QPoint pt, bool is_echoed)
{
    QPainter linePainter(&m_canvas);
    //QRgb lineValue = m_drawColor.rgb();

    //customizePen(pen, lineValue, 1, "custom");
    linePainter.setPen(pen);


    if (previousPt.isNull()) // If previous point does not initialized, initialize it.
    {
        previousPt = pt;
        prePt10 = pt;
        points += 1;
        qDebug() << "x: " << prePt10.x() << "\t" << "y: " << prePt10.y() << "\t" << Qt::endl;

        /*if(have_samples){
            curPt10 = pt;
            customizePen(pen, lineValue, 3, "red"); // lineValue is a black but inside function it is modified as a red.
            linePainter.setPen(pen);

            // Equalize pair object's angle & distance values
            angleDistancePair.first = curPt10.x();
            angleDistancePair.second = curPt10.y();

            // Store angle and distance pair object inside the Queue
            angleDistanceQueue.enqueue(angleDistancePair);
        }*/
    }
    else
    {
        points += 1;
        if (points % 1 == 0)
        {
            if (distCalculator(prePt10, pt) || is_echoed)
            {

                linePainter.drawLine(previousPt, pt);
                curPt10 = pt;
                qDebug() << "x: " << curPt10.x() << "\t" << "y: " << curPt10.y() << "\t" << Qt::endl;

                // Equalize pair object's angle & distance values
                angleDistancePair.first = curPt10.x();
                angleDistancePair.second = curPt10.y();

                // Store angle and distance pair object inside the Queue
                angleDistanceQueue.enqueue(angleDistancePair);

                //calculateAngleDistance(prePt10, curPt10);
                prePt10 = curPt10;
            }
        }
        else
        {
            linePainter.drawLine(previousPt, pt);
        }
        previousPt = pt;
    }

}


bool DrawWidget::distCalculator(QPoint prePt10, QPoint curPt10)
{
    double distVal = (qSqrt(qPow(curPt10.x()-prePt10.x(), 2) + qPow(curPt10.y()-prePt10.y(), 2)));
    if (distVal >= 9 && distVal <= 11)
    {
        return true;
    }
    else
    {
        return false;
    }

}


void DrawWidget::calculateAngleDistance(QPoint prePt10, QPoint curPt10)
{
    // Calculate the bisector vector
    QPoint bisectorVector;
    qreal bisectorVectorAnglePrev = bisectorVectorAngle;

    bisectorVector.setX(curPt10.x() - prePt10.x());
    bisectorVector.setY(curPt10.y() - prePt10.y());

    // Calculate the angle of the bisector vector
    bisectorVectorAngle = qRound(qRadiansToDegrees(qAtan2(bisectorVector.y(), bisectorVector.x())));
    bisectorVectorLength = qRound(qSqrt(qPow(bisectorVector.x(), 2) + qPow(bisectorVector.y(), 2)));

    // Equalize pair object's angle & distance values
    angleDistancePair.first = int(bisectorVectorAngle - bisectorVectorAnglePrev);
    angleDistancePair.second = int(bisectorVectorLength);

    // Store angle and distance pair object inside the Queue
    angleDistanceQueue.enqueue(angleDistancePair);

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
    
    while (angleDistanceQueue.isEmpty() == 0)
    {
        angleDistanceQueue.dequeue();
    }

    resetPen();
}


void DrawWidget::customizePen(QPen &currentPen, QRgb valueCustom, int width, QString colorName)
{
    QColor colorRed(255, 0, 0); // Create red QColor object
    QColor colorBlack(0, 0, 0); // Create black QColor object
    QColor colorBlue(0, 0, 255);
    QRgb valueRed = colorRed.rgb(); // Get QColor objects rgb red
    QRgb valueBlack = colorBlack.rgb(); // Get QColor objects rgb black
    QRgb valueBlue = colorBlue.rgb();

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
    else if(colorName == "blue")
    {
        currentPen.setColor(valueBlue);
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
    QColor color(255, 0, 0); // Create black QColor object
    QRgb valueBlack = color.rgb(); // Get QColor objects rgb values

    setDrawColor(color);
    customizePen(pen, valueBlack, 3, "red"); // Customize corresponding pen
}


void DrawWidget::printPoints()
{
    qDebug() << "Total number of points: " << points << Qt::endl;
    if(pico==nullptr){
        qDebug() << "No connection to pico!" << Qt::endl;
        return;
    }
    pico->send("Path Begin\n");
    while (!angleDistanceQueue.isEmpty())
    {
        pico->send(QString::number(angleDistanceQueue.front().first) + "," + QString::number(angleDistanceQueue.front().second) + "\n");
        qDebug() << angleDistanceQueue.dequeue() << Qt::endl;
    }

    previousPt = QPoint();

    QColor color(0, 0, 255); // Create black QColor object
    QRgb valueBlue = color.rgb(); // Get QColor objects rgb values

    setDrawColor(color);
    customizePen(pen, valueBlue, 3, "blue");

    // Customize corresponding pen

    pico->receive_cb = [this](QString received){
        std::vector<std::string> locs = split(received.toStdString(), ';');
        qDebug() << received << Qt::endl;
        for (std::string loc : locs){
            std::vector<std::string> loc_x_y = split(loc, ',');
            drawPixel(QPoint(stoi(loc_x_y[1]), stoi(loc_x_y[2])), true);
            repaint();
        }
    };
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
    if((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::TouchPointPressed))
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
