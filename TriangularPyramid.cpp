#include "TriangularPyramid.hpp"

TriangularPyramid::TriangularPyramid(std::array<QVector3D, 4> points,
                                     QVector3D cam, QVector3D light) : camera(cam), light_point(light) {
    points3D = points;
    update_center();
}

void TriangularPyramid::update_center() {
    float sum_x = 0, sum_y = 0, sum_z = 0;
    for (auto &point: points3D) {
        sum_x += point.x();
        sum_y += point.y();
        sum_z += point.z();
    }
    center.setX(sum_x / 4);
    center.setY(sum_y / 4);
    center.setZ(sum_z / 4);
}

void TriangularPyramid::change_projection() {
    is_flat = !is_flat;
}

bool TriangularPyramid::is_visible(int *face) {
    float vx1, vy1, vz1;
    float vx2, vy2, vz2;
    vx1 = points3D[face[0]].x() - points3D[face[1]].x();
    vy1 = points3D[face[0]].y() - points3D[face[1]].y();
    vz1 = points3D[face[0]].z() - points3D[face[1]].z();

    vx2 = points3D[face[1]].x() - points3D[face[2]].x();
    vy2 = points3D[face[1]].y() - points3D[face[2]].y();
    vz2 = points3D[face[1]].z() - points3D[face[2]].z();

    float a, b, c, d;
    a = vy1 * vz2 - vy2 * vz1;
    b = vz1 * vx2 - vz2 * vx1;
    c = vx1 * vy2 - vx2 * vy1;
    d = -(a * points3D[face[1]].x() + b * points3D[face[1]].y() + c * points3D[face[1]].z());
    float sign = ((a * center.x() + b * center.y() + c * center.z() + d) < 0) ? 1 : -1;
    a *= sign;
    b *= sign;
    c *= sign;
    d *= sign;

    if ((a * camera.x() + b * camera.y() + c * camera.z() + d) < 0) return false;
    return true;
}

bool TriangularPyramid::is_shadowed(int *face) {
    float vx1, vy1, vz1;
    float vx2, vy2, vz2;
    vx1 = points3D[face[0]].x() - points3D[face[1]].x();
    vy1 = points3D[face[0]].y() - points3D[face[1]].y();
    vz1 = points3D[face[0]].z() - points3D[face[1]].z();

    vx2 = points3D[face[1]].x() - points3D[face[2]].x();
    vy2 = points3D[face[1]].y() - points3D[face[2]].y();
    vz2 = points3D[face[1]].z() - points3D[face[2]].z();

    float a, b, c, d;
    a = vy1 * vz2 - vy2 * vz1;
    b = vz1 * vx2 - vz2 * vx1;
    c = vx1 * vy2 - vx2 * vy1;
    d = -(a * points3D[face[1]].x() + b * points3D[face[1]].y() + c * points3D[face[1]].z());
    float sign = ((a * center.x() + b * center.y() + c * center.z() + d) < 0) ? 1 : -1;
    a *= sign;
    b *= sign;
    c *= sign;
    d *= sign;

    if ((a * light_point.x() + b * light_point.y() + c * light_point.z() + d) < 0) return true;
    return false;
}

std::vector<int *> TriangularPyramid::get_visible_faces() {
    std::vector<int*> result;
    for (auto& face : faces){
        if (is_visible(face)){
            result.push_back(face);
        }
    }
    return result;
}

std::vector<bool> TriangularPyramid::get_shadowed_faces(const std::vector<int *>& vis_faces) {
    std::vector<bool> result;
    for (auto & face : vis_faces){
        result.push_back(is_shadowed(face));
    }
    return result;
}

void TriangularPyramid::project() {
    is_flat ? project_flat() : project_volume();
}

void TriangularPyramid::project_flat() {
    for(auto i = 0; i < 4; i++){
        points2D[i] = points3D[i].toPointF();
    }
}

void TriangularPyramid::project_volume() {
    for (int i = 0; i < 4; i++) {
        double xp, yp;
        xp = points3D[i].x() + points3D[i].z() * std::cos(projection_angle); // Jean Cavalier projection
        yp = points3D[i].y() + points3D[i].z() * std::sin(projection_angle);
        points2D[i].setX(xp);
        points2D[i].setY(yp);
    }
}

std::array<QPointF, 4> TriangularPyramid::get_points() {
    project();
    return points2D;
}

void TriangularPyramid::move(float dx, float dy, float dz) {
    for(auto& point : points3D){
        point.setX(point.x() + dx);
        point.setY(point.y() + dy);
        point.setZ(point.z() + dz);
    }
    update_center();
}

void TriangularPyramid::rotateX(double angle) {
    for (auto &point: points3D) {
        auto dz = point.z() - center.z(), dy = point.y() - center.y();
        double sz, sy;
        sy = dy * cos(angle) - dz * sin(angle);
        sz = dy * sin(angle) + dz * cos(angle);
        point.setZ(sz + center.z());
        point.setY(sy + center.y());
    }
}

void TriangularPyramid::rotateY(double angle) {
    for (auto &point: points3D) {
        auto dz = point.z() - center.z(), dx = point.x() - center.x();
        double sx, sz;
        sx = dx * cos(angle) + dz * sin(angle);
        sz = -dx * sin(angle) + dz * cos(angle);
        point.setZ(sz + center.z());
        point.setX(sx + center.x());
    }
}

void TriangularPyramid::rotateZ(double angle) {
    for (auto &point: points3D) {
        auto dx = point.x() - center.x(), dy = point.y() - center.y();
        double sx, sy;
        sx = dx * cos(angle) - dy * sin(angle);
        sy = dx * sin(angle) + dy * cos(angle);
        point.setX(sx + center.x());
        point.setY(sy + center.y());
    }
}

void TriangularPyramid::resize(float delta) {
    for (auto &point: points3D) {
        auto dx = point.x() - center.x(), dy = point.y() - center.y(), dz = point.z() - center.z();
        float sx, sy, sz;
        sx = dx * delta;
        sy = dy * delta;
        sz = dz * delta;
        point.setX(sx + center.x());
        point.setY(sy + center.y());
        point.setZ(sz + center.z());
    }
}

QVector3D *TriangularPyramid::operator[](int i) {
    return &points3D[i];
}

std::array<QVector3D, 4> TriangularPyramid::get_3D_points() {
    return points3D;
}
