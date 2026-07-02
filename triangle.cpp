#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

constexpr TGAColor white = { 255, 255, 255, 255 }; // attention, BGRA order
constexpr TGAColor green = { 0, 255,   0, 255 };
constexpr TGAColor red = { 0,   0, 255, 255 };
constexpr TGAColor blue = { 255, 128,  64, 255 };
constexpr TGAColor yellow = { 0, 200, 255, 255 };

constexpr int width = 200;
constexpr int height = 200;


void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
    bool steep = std::abs(by - ay) > std::abs(bx - ax);
    if (steep) {
        std::swap(ay, ax);
        std::swap(by, bx);
    }
    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    for (int x = ax; x <= bx; x++) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + t * (by - ay));
        if (steep) framebuffer.set(y, x, color);
        else framebuffer.set(x, y, color);
    }

}

std::pair<int, int> project(vec3 v, vec3 x_axis, vec3 y_axis) {
    float x, y, x_mag = std::hypot(x_axis[0], x_axis[1], x_axis[2]), y_mag = std::hypot(y_axis[0], y_axis[1], y_axis[2]);;

    x = (v[0] * x_axis[0] + v[1] * x_axis[1] + v[2] * x_axis[2]) / x_mag;
    x = x * (width / 2) + width / 2;
    y = (v[0] * y_axis[0] + v[1] * y_axis[1] + v[2] * y_axis[2]) / x_mag;
    y = y * (height / 2) + height / 2;

    return std::make_pair(x, y);


}




void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage& framebuffer, TGAColor color) {
    

    if (ay > by) {
        std::swap(ay, by);
        std::swap(ax, bx);
    }
    if (by > cy) {
        std::swap(by, cy);
        std::swap(bx, cx);
    }
    if (ay > by) {
        std::swap(ay, by);
        std::swap(ax, bx);
    }

    if (std::hypot((cx - ax), (cy - ay)) > std::hypot((cx - bx), (cy - by))) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    //std::cout << ax <<"," << ay << " " << bx << "," << by << " " << cx << "," << cy << std::endl;
    

    for (int y=by; y <= cy; y++) {
        if (y < ay) {
            float t1 = (y - by) / static_cast<float>(cy - by);
            float t2 = (y - by) / static_cast<float>(ay - by);
            int x1 = std::round(bx + t1 * (cx - bx));
            int x2 = std::round(bx + t2 * (ax - bx));

            std::cout << x1 << " " << x2 << std::endl;
            if (x1 > x2) std::swap(x1, x2);
            for (int x = x1; x <= x2; x++) {
                framebuffer.set(x, y, color);
            }
        }
        else {
            float t1 = (y - by) / static_cast<float>(cy - by);
            float t2 = (y - ay) / static_cast<float>(cy - ay);
            int x1 = std::round(bx + t1 * (cx - bx));
            int x2 = std::round(ax + t2 * (cx - ax));

            if (x1 > x2) std::swap(x1, x2);
            for (int x = x1; x <= x2; x++) {
                framebuffer.set(x, y, color);
            }
        }
    }
    line(ax, ay, bx, by, framebuffer, red);
    line(bx, by, cx, cy, framebuffer, yellow);
    line(cx, cy, ax, ay, framebuffer, green);

}



int main(int argc, char** argv) {

    TGAImage framebuffer(width, height, TGAImage::RGB);
    triangle(7, 45, 35, 100, 45, 60, framebuffer, red);
    triangle(120, 35, 90, 5, 45, 110, framebuffer, white);
    triangle(115, 83, 80, 90, 85, 120, framebuffer, green);
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;

}