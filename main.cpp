#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

constexpr int width = 800;
constexpr int height = 800;


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
        if(steep) framebuffer.set(y, x, color);
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

int main(int argc, char** argv) {

    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<std::vector<float>> v_array;
    std::vector<std::vector<int>> f_array, v_array_2d;
    vec3 x(0.0, 0.0, 1.0), y(0.0, 1.0, 0.0);

    Model model("diablo3_pose.obj");

    for (int i = 0; i < model.getFacesCount(); i++) {
        vec3 a, b, c;
        a = model.vert(i, 0);
        b = model.vert(i, 1);
        c = model.vert(i, 2);
        auto [ax, ay] = project(a, x, y);
        auto [bx, by] = project(b, x, y);
        auto [cx, cy] = project(c, x, y);

        line(ax, ay, bx, by, framebuffer, red);
        line(ax, ay, cx, cy, framebuffer, red);
        line(bx, by, cx, cy, framebuffer, red);

    }

    for (int i = 0; i < model.getVertCount(); i++) {
        auto [ax, ay] = project(model.vert(i), x, y);
        framebuffer.set(ax, ay, white);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

