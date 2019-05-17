#pragma once

#include <qglobal.h>
#include <QColor>

struct RangeColorPoint
{
    uchar blue_ = 255;
    uchar green_= 255;
    uchar red_ = 255;
    uchar alpha_= 255;
    double position_ = 0.0;

    QColor getRgbColor() const
    {
        QColor color;
        color.setRgb(red_, green_, blue_);
        return color;
    }

    QColor getRgbaColor() const
    {
        QColor color;
        color.setRgb(red_, green_, blue_,alpha_);
        return color;
    }

    void setColor(const QColor& color)
    {
        red_ = color.red();
        green_ = color.green();
        blue_ = color.blue();
        alpha_ = color.alpha();
    }
};

struct doubleVec2
{
    double x=0.0;
    double y=0.0;

    doubleVec2()
    {

    }

    doubleVec2(double ix, double iy):x(ix), y(iy)
    {
    }
};

struct size2_t
{
    std::size_t x=0;
    std::size_t y=0;

    size2_t()
    {

    }

    size2_t(std::size_t ix, std::size_t iy):x(ix), y(iy)
    {
    }
};


struct size3_t
{
    std::size_t x=0;
    std::size_t y=0;
    std::size_t z=0;

    size3_t()
    {

    }
    size3_t(std::size_t ix, std::size_t iy, std::size_t iz):x(ix),y(iy),z(iz)
    {
    }
};

enum class HistogramMode { Off, All, P99, P95, P90, Log };
