#include "block.h"

#include <iostream>
#include <vector>
#include <utility>
#include <queue>

using namespace std;

Block::Block(BlockType t, std::pair<int, int> pos, int level)
{
    this->type_ = t;
    this->coords_ = pos;
    this->level_ = level;
    // std::cout << "Block born" << std::endl;

    this->mWidth_ = 3;
    this->mHeight_ = 3;
    matrix_.resize(this->mHeight_, std::vector<char>(this->mWidth_, ' '));
    this->numCells_ = 4;

    if (t == I_BLK)
    {
        this->mWidth_ = 4;
        this->mHeight_ = 4;
        this->matrix_.clear();
        this->matrix_.resize(this->mHeight_, std::vector<char>(this->mWidth_, ' '));
        matrix_[0][0] = 'I';
        matrix_[0][1] = 'I';
        matrix_[0][2] = 'I';
        matrix_[0][3] = 'I';
        this->blockHeight_ = 1;
        this->blockWidth_ = 4;
    }
    else if (t == J_BLK)
    {
        matrix_[0][0] = 'J';
        matrix_[1][0] = 'J';
        matrix_[1][1] = 'J';
        matrix_[1][2] = 'J';
        this->blockHeight_ = 2;
        this->blockWidth_ = 3;
    }
    else if (t == L_BLK)
    {
        matrix_[0][2] = 'L';
        matrix_[1][0] = 'L';
        matrix_[1][1] = 'L';
        matrix_[1][2] = 'L';
        this->blockHeight_ = 2;
        this->blockWidth_ = 3;
    }
    else if (t == O_BLK)
    {
        matrix_[0][0] = 'O';
        matrix_[0][1] = 'O';
        matrix_[1][0] = 'O';
        matrix_[1][1] = 'O';
        this->blockHeight_ = 2;
        this->blockWidth_ = 2;
    }
    else if (t == S_BLK)
    {
        matrix_[0][1] = 'S';
        matrix_[0][2] = 'S';
        matrix_[1][0] = 'S';
        matrix_[1][1] = 'S';
        this->blockHeight_ = 2;
        this->blockWidth_ = 3;
    }
    else if (t == Z_BLK)
    {
        matrix_[0][0] = 'Z';
        matrix_[0][1] = 'Z';
        matrix_[1][1] = 'Z';
        matrix_[1][2] = 'Z';
        this->blockHeight_ = 2;
        this->blockWidth_ = 3;
    }
    else if (t == T_BLK)
    {
        matrix_[0][0] = 'T';
        matrix_[0][1] = 'T';
        matrix_[0][2] = 'T';
        matrix_[1][1] = 'T';
        this->blockHeight_ = 2;
        this->blockWidth_ = 3;
    }
};

HeavyBlock::HeavyBlock(BlockType t, std::pair<int, int> pos, int level) : Block(t, pos, level)
{
    std::cout << "Heavy block born" << std::endl;
}
StarBlock::StarBlock(std::pair<int, int> pos, int level) : Block(SPECIAL_BLK, pos, level)
{
    // std::cout << "Block born" << std::endl;

    this->mWidth_ = 1;
    this->mHeight_ = 1;

    this->matrix_.resize(this->mHeight_, std::vector<char>(this->mWidth_, ' '));
    matrix_[0][0] = 'X';

    this->blockHeight_ = 1;
    this->blockWidth_ = 1;
}

Block::~Block(){
    // std::cout << "Block died" << std::endl;
};

HeavyBlock::~HeavyBlock(){
    // std::cout << "Block died" << std::endl;
};

StarBlock::~StarBlock(){
    // std::cout << "Block died" << std::endl;
};

std::queue<Block> Block::moveBlock(Command c)
{
    // cout << "Move regular block" << endl;
    std::queue<Block> blocks;
    // Copy current block
    Block movedBlock{*this};

    if (c == LEFT)
    {
        movedBlock.coords_.second -= 1;
    }
    else if (c == RIGHT)
    {
        movedBlock.coords_.second += 1;
    }
    else if (c == DOWN)
    {
        movedBlock.coords_.first += 1;
    }
    blocks.push(movedBlock);

    return blocks;
};

// TODO: Helper remove
void print(std::vector<std::vector<char>> &mat)
{
    for (int i = 0; i < mat.size(); i++)
    {
        for (int j = 0; j < mat[i].size(); j++)
        {
            cout << mat[i][j];
        }
        cout << endl;
    }
}

bool isRowEmpty(int h, std::vector<std::vector<char>> &mat)
{
    for (int j = 0; j < mat[h].size(); j++)
    {
        if (mat[h][j] != ' ')
        {
            return false;
        }
    }
    return true;
};

bool isColEmpty(int w, std::vector<std::vector<char>> &mat)
{
    for (int i = 0; i < mat.size(); i++)
    {
        if (mat[i][w] != ' ')
        {
            return false;
        }
    }
    return true;
};

void removeRow(int h, std::vector<std::vector<char>> &mat)
{
    // Move everything up by 1
    // cout << "Before" << endl;
    // print(mat);
    for (int i = h; i < mat.size() - 1; i++)
    {
        for (int j = 0; j < mat[i].size(); j++)
        {
            mat[i][j] = mat[i + 1][j];
        }
    }
    // cout << "After" << endl;
    // print(mat);

    // Clear bottom row
    for (int j = 0; j < mat[mat.size() - 1].size(); j++)
    {
        mat[mat.size() - 1][j] = ' ';
    }
};

void removeCol(int w, std::vector<std::vector<char>> &mat)
{
    // Move everything left by 1
    // cout << "Before" << endl;
    // print(mat);
    for (int j = w; j < mat[w].size() - 1; j++)
    {
        for (int i = 0; i < mat.size(); i++)
        {
            mat[i][j] = mat[i][j + 1];
        }
    }
    // cout << "After" << endl;
    // print(mat);

    // Clear right col
    for (int i = 0; i < mat.size(); i++)
    {
        mat[i][mat[w].size() - 1] = ' ';
    }
};

void Block::removeWhitespace()
{
    std::vector<std::vector<char>> &mat = this->matrix_;
    // Remove empty horizontal rows by moving up

    int row = 0;
    // cout << "Remove line?: " << row << endl;
    // print(mat);
    while (isRowEmpty(row, mat))
    {
        removeRow(row, mat);
    }

    int col = 0;
    while (isColEmpty(col, mat))
    {
        removeCol(col, mat);
    }
}

void Block::calcBlockSize()
{
    this->blockHeight_ = this->matrix_.size();

    for (int i = this->matrix_.size() - 1; i >= 0; i--)
    {
        if (!isRowEmpty(i, this->matrix_))
        {
            this->blockHeight_ = i + 1;
            break;
        }
    }

    this->blockWidth_ = 0;
    for (int j = this->matrix_[0].size() - 1; j >= 0; j--)
    {
        if (!isColEmpty(j, this->matrix_))
        {
            this->blockWidth_ = j + 1;
            break;
        }
    }
    // cout << "New B:" << this->blockWidth_ << endl;
    // cout << "New H:" << this->blockHeight_ << endl;
}

void Block::rotateClockwise()
{
    std::vector<std::vector<char>> &mat = this->matrix_;
    int N = this->mWidth_;

    // Consider all squares one by one
    for (int x = 0; x < N / 2; x++)
    {
        // Consider elements in group
        // of 4 in current square
        for (int y = x; y < N - x - 1; y++)
        {
            // Store current cell in
            // temp variable
            int temp = mat[x][y];

            // Move values from top to right
            mat[x][y] = mat[N - 1 - y][x];

            // Move values from right to bottom
            mat[N - 1 - y][x] = mat[N - 1 - x][N - 1 - y];

            // Move values from bottom to left
            mat[N - 1 - x][N - 1 - y] = mat[y][N - 1 - x];

            // Assign temp to right
            mat[y][N - 1 - x] = temp;
        }
    }
};

void Block::rotateCounterclockwise()
{
    std::vector<std::vector<char>> &mat = this->matrix_;
    int N = this->mWidth_;

    // Consider all squares one by one
    for (int x = 0; x < N / 2; x++)
    {
        // Consider elements in group
        // of 4 in current square
        for (int y = x; y < N - x - 1; y++)
        {
            // Store current cell in
            // temp variable
            int temp = mat[x][y];

            // Move values from right to top
            mat[x][y] = mat[y][N - 1 - x];

            // Move values from bottom to right
            mat[y][N - 1 - x] = mat[N - 1 - x][N - 1 - y];

            // Move values from left to bottom
            mat[N - 1 - x][N - 1 - y] = mat[N - 1 - y][x];

            // Assign temp to left
            mat[N - 1 - y][x] = temp;
        }
    }
};

// Only supports rotations on square matrix
std::queue<Block> Block::rotateBlock(Command c)
{
    cout << "Rotate regular block" << endl;

    std::queue<Block> blocks;

    // Copy current block
    Block rotatedBlock{*this};

    // Prev top left corner
    pair<int, int> prevTCorner = rotatedBlock.getPos();
    // cout << "Old T Corner: " << prevTCorner.first << ":" << prevTCorner.second << endl;
    // Prev bottom left corner
    pair<int, int> prevBCorner = make_pair(prevTCorner.first + rotatedBlock.getBlockHeight() - 1, prevTCorner.second);
    // cout << "Old B Corner: " << prevBCorner.first << ":" << prevBCorner.second << endl;
    // cout << "Old height: " << rotatedBlock.getBlockHeight() << endl;

    if (c == COUNTERCLOCKWISE)
    {

        rotatedBlock.rotateCounterclockwise();
        rotatedBlock.removeWhitespace();
        rotatedBlock.calcBlockSize();
    }
    else if (c == CLOCKWISE)
    {
        rotatedBlock.rotateClockwise();
        rotatedBlock.removeWhitespace();
        rotatedBlock.calcBlockSize();
    }
    else
    {
        //  Assert should not be here
    }

    // New bottom left corner
    // cout << "New height: " << rotatedBlock.getBlockHeight() << endl;
    pair<int, int> newBCorner = make_pair(prevTCorner.first + rotatedBlock.getBlockHeight() - 1, prevTCorner.second);
    // cout << "New B Corner: " << newBCorner.first << ":" << newBCorner.second << endl;

    // New top left corner
    int diff = newBCorner.first - prevBCorner.first;
    pair<int, int> newTCorner = make_pair(prevTCorner.first - diff, prevTCorner.second);
    // cout << "New T Corner: " << newTCorner.first << ":" << newTCorner.second << endl;

    rotatedBlock.setPos(newTCorner);

    blocks.push(rotatedBlock);

    return blocks;
};

void Block::setMatrix(std::vector<std::vector<char>> m, int mHeight, int mWidth)
{
    this->matrix_ = m;
    this->mHeight_ = mHeight;
    this->mWidth_ = mWidth;
    this->calcBlockSize();
}

void Block::setPos(std::pair<int, int> m)
{
    this->coords_ = m;
}

int Block::getBlockHeight() const
{
    return this->blockHeight_;
};

int Block::getBlockWidth() const
{
    return this->blockWidth_;
};

int Block::getLevelGen() const
{
    return this->level_;
};

std::vector<std::vector<char>> Block::getCells() const
{
    return this->matrix_;
};

int Block::getBoxWidth() const
{
    return this->mWidth_;
};

int Block::getBoxHeight() const
{
    return this->mHeight_;
};

std::pair<int, int> Block::getPos() const
{
    return this->coords_;
};

bool Block::removeLine(int h)
{
    // cout << "Removing line from matrix: " << h << endl;
    // cout << "Old num cells: " << this->numCells_ << endl;
    // print(this->matrix_);

    int cellsCleared = 0;
    for (int j = 0; j < this->blockWidth_; j++)
    {
        if (this->matrix_[h][j] != ' ')
        {
            this->matrix_[h][j] = ' ';
            cellsCleared += 1;
        }
    }
    this->numCells_ -= cellsCleared;

    // print(this->matrix_);

    if (h == 0)
    {
        this->coords_.first += 1;
    }

    if (this->numCells_ == 0)
    {
        return true;
    }
    else
    {
        // cout << "Remove whitespace" << endl;
        this->removeWhitespace();

        // cout << "Calc block size" << endl;
        this->calcBlockSize();

        // print(this->matrix_);

        // cout << "New num cells: " << this->numCells_ << endl;
    }
    return false;
}

std::ostream &operator<<(std::ostream &sout, const Block &b)
{
    std::vector<std::vector<char>> cells = b.getCells();
    for (int i = 0; i < cells.size(); i++)
    {
        string row;
        bool emptyRow = true;
        for (int j = 0; j < cells[i].size(); j++)
        {
            if (cells[i][j] != ' ')
            {
                emptyRow = false;
            }
            row += cells[i][j];
        }
        if (!emptyRow)
        {
            sout << row << endl;
        }
    }
    return sout;
};

std::queue<Block> HeavyBlock::moveBlock(Command c)
{
    // std::cout << "Heavy block move " << std::endl;

    std::queue<Block> blocks = Block::moveBlock(c);

    // Heavy blocks move down automatically by 1
    Block newBlock = blocks.front();
    pair<int, int> pos = newBlock.getPos();
    pos.first += 1;
    newBlock.setPos(pos);
    blocks.push(newBlock);

    return blocks;
};

std::queue<Block> HeavyBlock::rotateBlock(Command c)
{
    std::cout << "Heavy block rotate " << std::endl;

    std::queue<Block> blocks = Block::rotateBlock(c);

    // Heavy blocks move down automatically by 1
    Block newBlock = blocks.front();
    pair<int, int> pos = newBlock.getPos();
    pos.first += 1;
    newBlock.setPos(pos);
    blocks.push(newBlock);

    return blocks;
}