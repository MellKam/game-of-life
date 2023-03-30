#include <vector>
#include <iostream>
#include <memory>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

int rand_from_range(int low, int high)
{
  std::uniform_int_distribution<int> dist(low, high);
  return dist(gen);
}

template <typename T>
using Matrix = std::vector<std::vector<T>>;

class Board
{
private:
  Matrix<bool> board;

public:
  size_t R;
  size_t C;

  Board(size_t rowSize, size_t columnSize)
      : board(rowSize, std::vector<bool>(columnSize, false))
  {
    this->C = columnSize;
    this->R = rowSize;

    for (int r = 0; r < R; r++)
    {
      for (int c = 0; c < C; c++)
      {
        this->board[r][c] = (bool)rand_from_range(0, 1);
      }
    }
  }

  void update()
  {
    std::vector<std::vector<int>> neighboursMap(R, std::vector<int>(C, 0));

    for (int r = 0; r < R; r++)
    {
      for (int c = 0; c < C; c++)
      {
        // skip if the current cell is not active
        if (!this->board[r][c])
          continue;

        int x_max = std::min((int)R, r + 2);

        for (int x = std::max(0, r - 1); x < x_max; x++)
        {
          int y_max = std::min((int)C, c + 2);

          for (int y = std::max(0, c - 1); y < y_max; y++)
          {
            // skip if it is the center cell
            if (x == r && y == c)
              continue;

            neighboursMap[x][y] += 1;
          }
        }
      }
    }

    for (int r = 0; r < R; r++)
    {
      for (int c = 0; c < C; c++)
      {
        if (
            (neighboursMap[r][c] == 3) ||
            (this->board[r][c] && neighboursMap[r][c] == 2))
        {
          this->board[r][c] = true;
          continue;
        }

        this->board[r][c] = false;
      }
    }
  }

  void print()
  {
    for (const auto &row : this->board)
    {
      for (const auto &item : row)
      {
        char symbol = item == true ? '#' : '.';
        std::cout << symbol << " ";
      }
      std::cout << std::endl;
    }
  }
};

int main()
{
  size_t boardSize;
  std::cout << "Enter the size of board: ";
  std::cin >> boardSize;

  auto board = new Board(boardSize, boardSize);
  board->print();

  while (true)
  {
    std::cout << "\nPress Enter to move to the next state: ";
    std::cin.ignore();
    board->update();
    std::cout << "\033[2J\033[1;1H"; // clear screen and move cursor to top left
    board->print();
  }
  return 0;
}