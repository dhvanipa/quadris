#include "observer.h"
#include "view.h"
#include "controller.h"
#include "model.h"
#include "subject.h"

#include <iostream>
#include <memory>
#include <string>

using namespace std;

View::View(std::shared_ptr<Controller> c, std::shared_ptr<Model> m)
{
    std::cout << "View born" << std::endl;
    this->model_ = m;
    this->controller_ = c;

    // Register view as observer of model
    model_->subscribe(this);
}

View::~View()
{
    std::cout << "View died" << std::endl;
    model_->unsubscribe(this);
}

void View::update()
{
    std::cout << "Updating view" << std::endl;
    // Drawing

    if (!this->model_->isGameOver())
    {
        std::cout << "Drawing board\n"
                  << std::endl;
        this->draw();
    }
    else
    {
        std::cout << "Game over!" << std::endl;
    }
    std::cout << std::endl;
}

void View::run()
{
    std::cout << "Running application" << std::endl;
    this->model_->startGame();

    while (!this->model_->isGameOver())
    {
        controller_->getCommand();
    }
}

void View::draw()
{
    // Draw score
    // TODO: Maybe use overloaded << operator
    // TODO: Justify to right
    std::cout << "Level:     " << model_->getLevelNum() << std::endl;
    std::cout << "Score:     " << model_->getScore() << std::endl;
    std::cout << "Hi Score:  " << model_->getHiScore() << std::endl;
    std::cout << "-----------" << std::endl;

    std::cout << model_->getBoard();

    std::cout << "-----------" << std::endl;
    std::cout << "Next:      " << std::endl;
    std::cout << model_->getNextBlock();
}
