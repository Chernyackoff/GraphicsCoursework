#include "Parallelepiped.hpp"
#include "Scene.hpp"
#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QApplication>


class MainWindow : public QMainWindow {
public:
    MainWindow() : QMainWindow(), scene(), view(&scene) {
        resize(800, 600);
        auto central = new QWidget(this);
        auto layout = new QHBoxLayout(central);
        central->setLayout(layout);
        setCentralWidget(central);

        auto vLayout = new QVBoxLayout(central);
        auto label = new QLabel("Controls:\n1. Movement: WASDVB\n"
                                "2. Rotation Z: QE\n3. Rotation Y: RF\n"
                                "4. Rotation X: TG\n"
                                "5. Scale: ZC\n"
                                "6. Change projection: Space",
                                central);

        vLayout->addWidget(label);
        layout->addLayout(vLayout);
        layout->addWidget(&view);

        scene.setParent(central);
        view.setParent(central);

        scene.setSceneRect(0, 0, 550, 550);
        view.setSceneRect(0, 0, 550, 550);

        main_scene = new Scene(&scene);

        labelZ = new QLabel("Center Z: 40", central);
        vLayout->addWidget(labelZ);

    }
    void change_figure(){
        if(f == paral) f = pyramid;
        else f = paral;
    }

    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_W:
                main_scene->move(f, 0, -delta_move, 0);
                break;
            case Qt::Key_S:
                main_scene->move(f, 0, delta_move, 0);
                break;
            case Qt::Key_A:
                main_scene->move(f, -delta_move, 0, 0);
                break;
            case Qt::Key_D:
                main_scene->move(f, delta_move, 0, 0);
                break;
            case Qt::Key_V:
                main_scene->move(f,0,0,-delta_move);
               break;
            case Qt::Key_B:
                main_scene->move(f, 0, 0, delta_move);
                break;
            case Qt::Key_Q:
                main_scene->rotateZ(f, -rotation_angle);
                break;
            case Qt::Key_E:
                main_scene->rotateZ(f, rotation_angle);
                break;
            case Qt::Key_R:
                main_scene->rotateY(f, -rotation_angle);
                break;
            case Qt::Key_F:
                main_scene->rotateY(f, rotation_angle);
                break;
            case Qt::Key_T:
                main_scene->rotateX(f, -rotation_angle);
                break;
            case Qt::Key_G:
                main_scene->rotateX(f, rotation_angle);
                break;
            case Qt::Key_Z:
                main_scene->resize(f, 0.7);
                break;
            case Qt::Key_C:
                main_scene->resize(f, 1.3);
                break;
            case Qt::Key_Space:
                main_scene->change_projection();
                break;
            case Qt::Key_1:
                change_figure();
                break;
        }
    }

private:
    QLabel *labelZ;
    QGraphicsScene scene;
    QGraphicsView view;
    Scene *main_scene;
    Figure f = paral;
    float delta_move = 30;
    double rotation_angle = 15 * std::numbers::pi / 180;

};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    return QApplication::exec();
    return 0;
}
