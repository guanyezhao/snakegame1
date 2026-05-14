#include "snakegame.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QKeyEvent>

// 构造函数
SnakeGame::SnakeGame(QWidget *parent)
    : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(GAME_WIDTH, GAME_HEIGHT);
    this->setWindowTitle("C++ 贪吃蛇游戏");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeGame::gameUpdate);

    initGame();
}

// 析构函数
SnakeGame::~SnakeGame()
{
    delete ui;
}

// 初始化游戏
void SnakeGame::initGame()
{
    snake.clear();

    // 初始化蛇（3个方块）
    snake.append({GAME_WIDTH/2, GAME_HEIGHT/2});
    snake.append({GAME_WIDTH/2 - BLOCK_SIZE, GAME_HEIGHT/2});
    snake.append({GAME_WIDTH/2 - 2*BLOCK_SIZE, GAME_HEIGHT/2});

    dir = RIGHT;
    isGameOver = false;
    isPaused = false;
    score = 0;

    spawnFood();
    timer->start(150); // 控制速度
}

// 生成食物
void SnakeGame::spawnFood()
{
    QRandomGenerator *gen = QRandomGenerator::global();
    do {
        food.x = gen->bounded(0, GAME_WIDTH / BLOCK_SIZE) * BLOCK_SIZE;
        food.y = gen->bounded(0, GAME_HEIGHT / BLOCK_SIZE) * BLOCK_SIZE;
    } while (snake.contains(food));
}

// 移动蛇
void SnakeGame::moveSnake()
{
    Point head = snake.first();
    Point newHead;

    switch (dir) {
    case UP:    newHead = {head.x, head.y - BLOCK_SIZE}; break;
    case DOWN:  newHead = {head.x, head.y + BLOCK_SIZE}; break;
    case LEFT:  newHead = {head.x - BLOCK_SIZE, head.y}; break;
    case RIGHT: newHead = {head.x + BLOCK_SIZE, head.y}; break;
    }

    snake.prepend(newHead);

    // 吃到食物
    if (newHead.x == food.x && newHead.y == food.y) {
        score += 10;
        spawnFood();
    } else {
        snake.removeLast();
    }
}

// 碰撞检测
bool SnakeGame::checkCollision()
{
    Point head = snake.first();
    // 撞墙
    if (head.x < 0 || head.x >= GAME_WIDTH || head.y < 0 || head.y >= GAME_HEIGHT)
        return true;
    // 撞自己
    for (int i = 1; i < snake.size(); i++) {
        if (head.x == snake[i].x && head.y == snake[i].y)
            return true;
    }
    return false;
}

// 游戏更新
void SnakeGame::gameUpdate()
{
    if (isGameOver || isPaused) return;

    moveSnake();
    if (checkCollision()) {
        isGameOver = true;
        timer->stop();
        QMessageBox::information(this, "游戏结束", QString("游戏结束！你的得分：%1").arg(score));
    }
    update();
}

// 绘图
void SnakeGame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // 画背景
    painter.fillRect(rect(), Qt::white);

    // 画蛇
    painter.setBrush(Qt::darkGreen);
    for (const auto &p : snake) {
        painter.drawRect(p.x, p.y, BLOCK_SIZE, BLOCK_SIZE);
    }

    // 画食物
    painter.setBrush(Qt::red);
    painter.drawRect(food.x, food.y, BLOCK_SIZE, BLOCK_SIZE);

    // 画分数
    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("分数: %1").arg(score));

    if (isGameOver) {
        painter.setPen(Qt::red);
        painter.drawText(rect(), Qt::AlignCenter, "游戏结束！按R键重新开始");
    }
}

// 键盘控制
void SnakeGame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:    if (dir != DOWN) dir = UP; break;
    case Qt::Key_Down:  if (dir != UP) dir = DOWN; break;
    case Qt::Key_Left:  if (dir != RIGHT) dir = LEFT; break;
    case Qt::Key_Right: if (dir != LEFT) dir = RIGHT; break;
    case Qt::Key_P:     isPaused = !isPaused; break;
    case Qt::Key_R:     if (isGameOver) initGame(); break;
    }
}

// 开始游戏
void SnakeGame::startGame()
{
    initGame();
}

// 暂停游戏
void SnakeGame::pauseGame()
{
    isPaused = !isPaused;
}