/* abGate Knob Style
 *
 * Copyright 2012 Antanas Bružas
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <QtGui>
#include <iostream>
#include <cmath>
#include "abgatestyle.h"

inline int valueAngle(const QStyleOptionSlider *dial)
{
    return -((dial->sliderValue - dial->minimum) * 300 * 16) / (dial->maximum - dial->minimum);
}

inline void paintArc(QPainter *p, const QStyleOptionSlider *dial)
{
    QColor color_0f0f0f(15, 15, 15);
    QColor color_b10a0a(170, 10, 10);
    QColor arcColor = (dial->state & QStyle::State_Enabled) ? color_0f0f0f : color_b10a0a;
    QLinearGradient linGrad(80, 100, 140, 140);
    linGrad.setColorAt(0, arcColor.dark(140));
    linGrad.setColorAt(1, arcColor.light().light());
    linGrad.setSpread(QGradient::PadSpread);
    p->setBrush(linGrad);

    p->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF rectangle(1, 1, 198, 198);

    int spanAngle = valueAngle(dial);
    int startAngle = 240 * 16;
    p->drawPie(rectangle, startAngle, spanAngle);
}

inline void paintDial(QPainter *painter)
{
    // Metal color
    QColor color_d0d2c2(208, 210, 194);
    // Color for cirles
    QColor transparent_black(0, 0, 0, 15);
    // Color #1 for plastic bg gradient
    QColor color_1b1b1b(27, 27, 27);
    // Color #2 for plastic bg gradient
    QColor color_63635c(70, 70, 65);

    painter->setPen(Qt::NoPen);

    // Shadow for plastic
    QRadialGradient radGrad1(100, 100, 100, 100, 100);
   // radGrad1.setColorAt(0, Qt::black);
    radGrad1.setColorAt(0.8, Qt::black);
    radGrad1.setColorAt(1, Qt::transparent);
    painter->setBrush(radGrad1);
    painter->drawEllipse(1, 1, 198, 198);

    // Black plastic gradient background
    QConicalGradient coneGradient1(100, 100, 135.0);
    coneGradient1.setColorAt(0.0, color_1b1b1b);
    coneGradient1.setColorAt(0.17, color_63635c);
    coneGradient1.setColorAt(0.34, color_1b1b1b);
    coneGradient1.setColorAt(0.51, color_63635c);
    coneGradient1.setColorAt(0.68, color_1b1b1b);
    coneGradient1.setColorAt(0.85, color_63635c);
    coneGradient1.setColorAt(1.0, color_1b1b1b);

    painter->setBrush(coneGradient1);
    // Drawing plastic gradiented ellipse
    painter->drawEllipse(5, 5, 190, 190);

    // Shadow for metal
    QRadialGradient radGrad2(100, 100, 60, 100, 100);
    radGrad2.setColorAt(0, Qt::black);
    radGrad2.setColorAt(1, Qt::transparent);
    painter->setBrush(radGrad2);
    painter->drawEllipse(40, 40, 120, 120);

    // First metal gradient background
    QConicalGradient coneGradient2(100, 100, 90.0);
    coneGradient2.setColorAt(0.0, color_d0d2c2);
    coneGradient2.setColorAt(0.17, Qt::white);
    coneGradient2.setColorAt(0.34, color_d0d2c2);
    coneGradient2.setColorAt(0.51, Qt::white);
    coneGradient2.setColorAt(0.68, color_d0d2c2);
    coneGradient2.setColorAt(0.85, Qt::white);
    coneGradient2.setColorAt(1.0, color_d0d2c2);

    painter->setBrush(coneGradient2);
    // Drawing first metal gradiented ellipse
    painter->drawEllipse(45, 45, 110, 110);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(transparent_black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    // Drawing small circle
    painter->drawEllipse(50, 50, 100, 100);

    // Second Metal gradient background
    QConicalGradient coneGradient3(100, 100, 135.0);
    coneGradient3.setColorAt(0.0, color_d0d2c2);
    coneGradient3.setColorAt(0.17, Qt::white);
    coneGradient3.setColorAt(0.34, color_d0d2c2);
    coneGradient3.setColorAt(0.51, Qt::white);
    coneGradient3.setColorAt(0.68, color_d0d2c2);
    coneGradient3.setColorAt(0.85, Qt::white);
    coneGradient3.setColorAt(1.0, color_d0d2c2);

    painter->setBrush(coneGradient3);
    // Drawing second metal gradiented ellipse
    painter->drawEllipse(53, 53, 94, 94);
}

inline void paintDot(QPainter *painter, const QStyleOptionSlider *dial)
{
    int startPoint = (240 * 16) + valueAngle(dial);
    QColor color_0f0f0f(15, 15, 15, 150);
    QColor color_b10a0a(170, 10, 10, 224);
    QPalette pal = dial->palette;
    QPen dotPen((dial->state & QStyle::State_Enabled) ? color_b10a0a : pal.mid().color(), 9, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QRect rectangle1(63, 63, 74, 74);
    painter->setPen(dotPen);
    painter->drawArc(rectangle1, startPoint, 5);

    painter->setPen(QPen(Qt::transparent, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    // Red pointer gradient
    QRadialGradient radGrad1(100, 100, 104, 100, 100);
    radGrad1.setColorAt(0.0, Qt::transparent);
    radGrad1.setColorAt(0.69, Qt::transparent);
    radGrad1.setColorAt(0.7, color_b10a0a);
    radGrad1.setColorAt(1.0, color_b10a0a);
    painter->setBrush(radGrad1);
    painter->drawPie(5, 5, 190, 190, startPoint - 30, 60);

    // Dark pointer gradient
    QRadialGradient radGrad2(100, 100, 104, 100, 100);
    radGrad2.setColorAt(0.0, Qt::transparent);
    radGrad2.setColorAt(0.69, Qt::transparent);
    radGrad2.setColorAt(0.7, color_0f0f0f);
    radGrad2.setColorAt(1.0, color_0f0f0f);
    painter->setBrush(radGrad2);
    for (int i = 1; i < 16; ++i) {
        painter->drawPie(6, 6, 188, 188, startPoint - 30 - 16 * 22.5 * i, 60);
    }

    /*painter->translate(100, 100);
    for (int j = 0; j < 16; ++j) {
//        if ((j % 5) != 0)
            painter->drawLine(74, 0, 93, 0);
        painter->rotate(22.5 + valueAngle(dial));
    }*/
}

inline void paintScale(QPainter *painter, const QStyleOptionSlider *dial)
{
    if (dial->subControls & QStyle::SC_DialTickmarks)
    {
        QRect rectangle2(5, 5, 190, 190);
        int ns = dial->tickInterval;
        int dot = -1 + (dial->maximum + ns - dial->minimum) / ns; //int dot = 25;
        double delta = 300.0*16.0 / dot;
        painter->setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (int i = 0; i <= dot; i++)
        {
            painter->drawArc(rectangle2, int(240*16 - delta*i), 5);
        }
    }
}

void abGateStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const
{
    if (cc != QStyle::CC_Dial) {
        QCommonStyle::drawComplexControl(cc, opt, p, widget);
        return;
    }

    const QStyleOptionSlider *dial = qstyleoption_cast<const QStyleOptionSlider *>(opt);
    if (dial == NULL) { return; }

    p->save();
    int size = dial->rect.width() < dial->rect.height() ? dial->rect.width() : dial->rect.height();
    p->setViewport((dial->rect.width()-size)/2, (dial->rect.height()-size)/2, size, size);
    p->setWindow(0, 0, 200, 200);
    p->setRenderHint(QPainter::Antialiasing);

//    paintArc(p, dial);
    paintScale(p, dial);
    paintDial(p);
    paintDot(p, dial);

    // done
    p->restore();
}
