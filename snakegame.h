#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <QList>

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SnakeGame : public QMainWindow
{
    Q_OBJECT

public:
    SnakeGame(QWidget *parent = nullptr);
    ~SnakeGame();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent*event) override;

private slots:
    void gameUpdate();
    void startGame();
    void pauseGame();

private:
    Ui::MainWindow *ui;

    // 游戏常量
    static const int BLOCK_SIZE = 20;
    static const int GAME_WIDTH = 400;
    static const int GAME_HEIGHT = 400;
    enum Direction { UP, DOWN, LEFT, RIGHT };

    QTimer *timer;
    QList<Point> snake;
    Point food;
    Direction dir;
    bool isGameOver;
    bool isPaused;
    int score;

    // 游戏方法
    void initGame();
    void spawnFood();
    void moveSnake();
    bool checkCollision();
};

#endif // SNAKEGAME_H