
#include "board.h"
#include "cell.h"
#include "block.h"
#include "controller.h"

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

Board::Board(int h, int w)
{
    std::cout << "Board born" << std::endl;
    this->height_ = h;
    this->width_ = w;
    for (int i = 0; i < h; i++)
    {
        std::vector<Cell *> newRow;
        this->board_.push_back(newRow);
        for (int j = 0; j < w; j++)
        {
            Cell *newCell = new Cell(i, j, ' ');
            this->board_[i].push_back(newCell);
        }
    }
};

Board::~Board()
{
    std::cout << "Board died" << std::endl;
    for (int i = 0; i < this->height_; i++)
    {
        for (int j = 0; j < this->width_; j++)
        {
            Cell *cell = this->board_[i][j];
            delete cell;
        }
    }
    for (int i = 0; i < this->blocks_.size(); i++)
    {
        Block *block = this->blocks_[i];
        delete block;
    }
};

void Board::addBlock(Block *block)
{
    std::cout << "Adding block to board" << std::endl;

    this->currBlock_ = block;
    // TODO: Need to check if CAN add block
    this->blocks_.push_back(block);
    this->updateCells(block);
};

void Board::clearCells(Block *block)
{
    std::vector<std::vector<char>> cells = block->getCells();
    int cellWidth = block->getBoxWidth();
    int cellHeight = block->getBoxHeight();

    std::pair<int, int> bottomLeftCorner = block->getPos();

    // TODO: SHOULD BE ITERATOR PATTERN?
    // Set cells
    int bRow = bottomLeftCorner.first;
    int bCol = bottomLeftCorner.second;

    int k = cellHeight - 1;
    for (int i = bRow; i > bRow - cellHeight; i--)
    {
        for (int j = bCol; j < cellWidth; j++)
        {
            if (cells[k][j] != ' ')
            {
                this->setCell(i, j, ' ');
            }
        }
        k--;
    }
}

void Board::updateCells(Block *block)
{
    std::vector<std::vector<char>> cells = block->getCells();
    int cellWidth = block->getBoxWidth();
    int cellHeight = block->getBoxHeight();

    std::pair<int, int> bottomLeftCorner = block->getPos();

    // TODO: SHOULD BE ITERATOR PATTERN?
    // Set cells
    int bRow = bottomLeftCorner.first;
    int bCol = bottomLeftCorner.second;

    int k = cellHeight - 1;
    for (int i = bRow; i > bRow - cellHeight; i--)
    {
        for (int j = bCol; j < cellWidth; j++)
        {
            if (cells[k][j] != ' ')
            {
                this->setCell(i, j, cells[k][j]);
            }
        }
        k--;
    }
}

void Board::setCell(int i, int j, char c)
{
    this->board_[i][j]->setSymbol(c);
};

bool Board::moveCurrentBlock(Command)
{
    return true;
};

bool Board::rotateCurrentBlock(Command c)
{
    this->currBlock_->rotateBlock(c);
    return true;
};

int Board::dropCurrentBlock()
{
    pair<int, int> curPos = this->currBlock_->getPos();
    int newH;
    for (newH = curPos.first + 1; newH < this->height_; newH++)
    {
        if (this->board_[newH][curPos.second]->isOccupied())
        {
            break;
        }
    }
    this->clearCells(this->currBlock_);
    this->currBlock_->dropBlock(newH);
    pair<int, int> newPos = this->currBlock_->getPos();
    this->updateCells(this->currBlock_);
    // Update cells

    // Check if last row full
    bool rowFull = true;
    int rowsCleared = 0;
    do
    {
        for (int j = 0; j < this->width_; j++)
        {
            if (this->board_[this->height_ - 1][j]->getSymbol() != ' ')
            {
                rowFull = false;
                break;
            }
        }
        // Clear bottm row
        if (rowFull)
        {
            rowsCleared += 1;
            for (int i = this->height_ - 2; i >= 0; i--)
            {
                for (int j = 0; j < this->height_; j++)
                {
                    this->setCell(i + 1, j, this->board_[i][j]->getSymbol());
                }
            }
            for (int j = 0; j < this->width_; j++)
            {
                this->setCell(0, j, ' ');
            }
        }
    } while (rowFull);

    return rowsCleared;
};

std::vector<std::vector<char>> Board::getBoard()
{
    std::vector<std::vector<char>> printBoard;
    for (int i = 0; i < this->height_; i++)
    {
        std::vector<char> newRow;
        printBoard.push_back(newRow);
        for (int j = 0; j < this->width_; j++)
        {
            Cell *cell = this->board_[i][j];
            printBoard[i].push_back(cell->getSymbol());
        }
    }
    return printBoard;
};

bool Board::isOccupied(int i, int j)
{
    return this->board_[i][j]->isOccupied();
};

bool Board::isLineFull(int h)
{
    return true;
};

bool Board::removeLine(int)
{
    return true;
};

void Board::resetBoard(){};

void Board::showHint(){};

int Board::getHeight() const
{
    return this->height_;
};
int Board::getWidth() const
{
    return this->width_;
};