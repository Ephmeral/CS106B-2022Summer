/*
 * CS 106B, Julie Zelenski
 *
 * This file declares a class named Ball.
 * A class is a template or blueprint for a new type of object.
 *
 * Each Ball object represents information about a ball.
 * A ball has a location (x,y), velocity (vx, vy) and id number.
 */
#pragma once
#include "gwindow.h"
#include "gfont.h"
#include <string>

class Ball {

  public:

    // constructor (initializes new object)

    // id number is used to label ball when drawn
    // The new ball is placed at random location of window
    // and set into motion with random velocity
    Ball(int id, GWindow* w);

    // member functions

    // Draw this ball as a numbered circle at its current location
    void draw();

    // Move this ball by one velocity increment
    void move();

  private:
    // member variables
    // names prefixed with underscore to remind us these are member variables
    int _x;
    int _y;
    int _vx;
    int _vy;
    int _id;
    GWindow* _window;
};


// This is a goopy piece of code needed to account for platform differences in
// how fonts are sized. Pay no attention.
static void drawCenteredText(GWindow *win, double x, double y, double width, double height, std::string text)
{
    struct { int width, height; } bounds;
    int font_size = 1;
    do {
        std::string font_desc = "Helvetica-" + integerToString(++font_size);
        win->setFont(font_desc);
        QFontMetrics metrics(GFont::toQFont(font_desc));
        bounds = { metrics.horizontalAdvance(QString::fromStdString(text)), metrics.ascent() };
    } while (bounds.height < height/2 && bounds.width < width/2);
    win->drawString(text, x + (width-bounds.width)/2, y + height - (height-bounds.height)/2);
}
