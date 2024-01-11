// gas.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get City::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;               // max number of rows in a city
const int MAXCOLS = 30;               // max number of columns in a city
const int MAXTOOTERS = 125;           // max number of Tooters allowed

const int UP      = 0;
const int DOWN    = 1;
const int LEFT    = 2;
const int RIGHT   = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class City;  // This is needed to let the compiler know that City is a
             // type name, since it's mentioned in the Tooter declaration.

class Tooter
{
  public:
        // Constructor
    Tooter(City* cp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();

  private:
    City* m_city;
    int   m_row;
    int   m_col;
};

class Player
{
  public:
        // Constructor
    Player(City *cp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    int  health() const;
    bool isPassedOut() const;

        // Mutators
    void  preach();
    void  move(int dir);
    void  getGassed();

  private:
    City* m_city;
    int   m_row;
    int   m_col;
    int   m_age;
      //       data member to support your implementation of the behavior
      //       affected by the player's being blasted with gas.
    int m_health;
};

class City
{
  public:
        // Constructor/destructor
    City(int nRows, int nCols);
    ~City();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    bool    isPlayerAt(int r, int c) const;
    int     tooterCount() const;
    int     nTootersAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    
    //helper
    bool isInBounds(int r, int c) const;
    bool isAdjacent(int row1, int col1, int row2, int col2) const;

        // Mutators
    bool addTooter(int r, int c);
    bool addPlayer(int r, int c);
    void preachToTootersAroundPlayer();
    void moveTooters();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Tooter* m_tooters[MAXTOOTERS];
    int     m_nTooters;

      // Helper functions
//    bool isInBounds(int r, int c) const; //moved
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nTooters);
    ~Game();

        // Mutators
    void play();

  private:
    City* m_city;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
int randInt(int min, int max);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Tooter implementation
///////////////////////////////////////////////////////////////////////////

Tooter::Tooter(City* cp, int r, int c)
{
    if (cp == nullptr)
    {
        cout << "***** A Tooter must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > cp->rows()  ||  c < 1  ||  c > cp->cols())
    {
        cout << "***** Tooter created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_city = cp;
    m_row = r;
    m_col = c;
}

int Tooter::row() const
{
    return m_row;
}

int Tooter::col() const
{
    return m_col;
}

void Tooter::move()
{
      // Attempt to move in a random direction; if it can't move, don't move.
      // If the player is there, don't move.
    int dir = randInt(0, NUMDIRS-1);  // dir is now UP, DOWN, LEFT, or RIGHT
    //Move in the appropriate direction if allowed
    
    int currentRow = row();
    int currentCol = col();
    
    int n_row = currentRow; int n_col = currentCol;
    
    switch(dir){
        case 0://Up
            if(n_row -1 >=1){
                n_row -= 1;//up one row
            }
            break;
        case 1:
            //Down
            if(n_row+1<=(m_city->rows())){ // <=because city 1,1 means tooter can occupy that
                n_row += 1;//down one row
            }
            break;
        case 2:
            //Left
            if(n_col-1>=1){
                n_col -= 1;//left one row
            }
            break;
        case 3:
            //Right
            if(n_col+1<=m_city->cols()){
                n_col += 1;//left one row
            }
            break;
            
    }
    if((m_city->isPlayerAt(n_row,n_col)==false)){
        m_col = n_col;
        m_row = n_row;
//    }else{
//        cout << "cannot move to (" << n_row<< ", " << n_row<<") due to player";
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(City* cp, int r, int c)
{
    if (cp == nullptr)
    {
        cout << "***** The player must be created in some City!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > cp->rows()  ||  c < 1  ||  c > cp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_city = cp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_health = 12; //12 puffs til death
    
}

int Player::row() const
{
    //Return the row the player is at.
    return m_row;
}

int Player::col() const
{
    //Return the column the player is at.
    return m_col;
}

int Player::age() const
{
    //Return the player's age.
    return m_age;
}

int Player::health() const
{
    //Return the player's health status.
    return m_health;
}

bool Player::isPassedOut() const
{
    //Return whether the player is passed out.
    return (m_health <= 0);
}

void Player::preach()
{
    m_age++;
    m_city->preachToTootersAroundPlayer();
}

void Player::move(int dir)
{
    m_age++;
      // If there is a grid position in the indicated direction that is
      //        adjacent to the player and vacant, move the player there;
    
    int currentRow = row();
    int currentCol = col();
    
    int n_row = currentRow; int n_col = currentCol;
    
    
    switch(dir){
        case 0://Up
            n_row--;
            break;
        case 1:
            //Down
            n_row++;
            break;
        case 2:
            //Left
            n_col--;
            break;
        case 3:
            //Right
            n_col++;
            break;
    }
      //        otherwise, don't move.
    if((m_city->isInBounds(n_row, n_col)) && (m_city->nTootersAt(n_row,n_col)==0))
    {
//        cout << "Moving to (" << n_row << ", " << n_col << ")" << endl;
        m_col = n_col;
        m_row = n_row;
//    }else{
//        cout << "Cannot move to (" << n_row << ", " << n_col << ") due to a Tooter." << endl;

    }
    
}

void Player::getGassed()
{
    //health decreases by 1
        m_health--;
}

///////////////////////////////////////////////////////////////////////////
//  City implementations
///////////////////////////////////////////////////////////////////////////

City::City(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** City created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nTooters = 0;
}

City::~City()
{
    //Delete the player and all remaining dynamically allocated Tooters.
    delete m_player;
    for (int i = 0;i<m_nTooters;i++){
        if(m_tooters[i] != nullptr){
            delete m_tooters[i];//deletes every tooters
            m_tooters[i] = nullptr;
        }
    }
}

int City::rows() const
{
    //Return the number of rows in the city.
    return m_rows;
}

int City::cols() const
{
    //Return the number of columns in the city.
    return m_cols;
}

Player* City::player() const
{
    return m_player;
}

bool City::isPlayerAt(int r, int c) const
{
    //Return true if the player is at row r, column c, otherwise false.
    return ((m_player->row()==r) && (m_player->col()==c));
}

int City::tooterCount() const
{
    return m_nTooters;
}

int City::nTootersAt(int r, int c) const
{
    //Return the number of Tooters at row r, column c.
    int counter = 0;
    for(int i = 0;i<m_nTooters;i++)
    {
        if(m_tooters[i] == nullptr){
            continue;//added
        }
        else if((m_tooters[i]->row()==r) && (m_tooters[i]->col() == c)){
            counter++;
        }
    }
    return counter;
}

bool City::determineNewPosition(int& r, int& c, int dir) const
{
      //If a move from row r, column c, one step in direction dir
      //        would go off the edge of the city, leave r and c unchanged and
      //        return false.  Otherwise, set r or c so that row r, column c,
      //        is now the new position resulting from the proposed move, and
      //        return true.
    
    int n_r = r; 
    int n_c = c;//create new col and row
    
    switch (dir)
    {
      case UP:
        //Implement the behavior if dir is UP.
            n_r--;
        break;
      case DOWN:
            n_r++;
        break;
      case LEFT:
            n_c--;
            break;
      case RIGHT:
            n_c++;
            break;
      default:
        return false;
    }
    if(!isInBounds(n_r, n_c)){
        return false;
    }else{
        r=n_r;
        c=n_c;
        return true;
    }
}

void City::display() const
{
      // Position (row,col) in the city coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

        // Indicate each Tooter's position
    // If one Tooter is at some grid point, set the char to 'T'.
    //        If it's 2 though 8, set it to '2' through '8'.
    //        For 9 or more, set it to '9'.
    for(int i = 0;i<m_nTooters;i++){
        
        int totr = m_tooters[i]->row() - 1;
        int totc = m_tooters[i]->col() - 1;
        
        switch (grid[totr][totc])
        {
            case '.':
                grid[totr][totc] = 'T';
                break;
            case 'T':
                grid[totr][totc] = '2';
                break;
            case '2':
                grid[totr][totc] = '3';
                break;
            case '3':
                grid[totr][totc] = '4';
                break;
            case '4':
                grid[totr][totc] = '5';
                break;
            case '5':
                grid[totr][totc] = '6';
                break;
            case '6':
                grid[totr][totc] = '7';
                break;
            case '7':
                grid[totr][totc] = '8';
                break;
            default:
                grid[totr][totc] = '9';
                break;
        }
    }

        // Indicate player's position
    if (m_player != nullptr)
    {
          // Set the char to '@', unless there's also a Tooter there
          // (which should never happen), in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }
    
        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, Tooter, and player info
    cout << "There are " << m_nTooters << " unconverted Tooters remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isPassedOut())
            cout << "The player has passed out." << endl;
        else
            cout << "The player's health level is " << m_player->health() << endl;
    }
}

bool City::addTooter(int r, int c)
{
    if (! isInBounds(r, c))
        return false;

      // Don't add a Tooter on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

      // If there are MAXTOOTERS unconverted Tooters, return false.
    if(m_nTooters >= MAXTOOTERS)
    {
        return false;
    }
    else{// Otherwise, dynamically allocate a new Tooter at coordinates (r,c).
        m_tooters[m_nTooters] = new Tooter(this,r,c);
        m_nTooters++;
    }
    return true;

      // Save the pointer to the newly allocated Tooter and return true.

      // Your function must work as specified in the preceding paragraph even
      // in this scenario (which won't occur in this game):  MAXTOOTERS
      // Tooters are added, then some are converted and removed from the city,
      // then more are added.
}

bool City::addPlayer(int r, int c)
{
    if (! isInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a Tooter
    if (nTootersAt(r, c) > 0)
        return false;

      // Dynamically allocate a new Player and add it to the city
    m_player = new Player(this, r, c);
    return true;
}

//void City::preachToTootersAroundPlayer()
//{
//      // Preach to Tooters orthogonally or diagonally adjacent to player.
//      // If a Tooter is converted, destroy it and remove it from the city,
//      // since we have no further need to display it or have it interact with
//      // the player.
//    
//    //tooter count:
//    int ntooterCount = m_nTooters;
//
//    
//    //origin
//    int pr = m_player->row();
//    int pc = m_player->col();
//
//    //loop
//    for(int i = 0;i<ntooterCount;i++){
//        if(m_tooters[i] == nullptr){
//            continue;//
//        }
//        //tooter pos
//        int totr = m_tooters[i]->row();
//        int totc = m_tooters[i]->col();
//        //loop
//        if(totr >= pr-1 && totr <= pr+1 && totc >= pc-1 && totc <=pc+1){
//            if(randInt(1, 3)<=2){
//                cout << "66% drew" <<endl;
//                if(m_tooters[i] != nullptr){
//                    delete m_tooters[i];//deletes every tooters
//                    m_tooters[i] = nullptr;
//                    m_nTooters--;//
//                    cout << "tooter deleted; count: " <<m_nTooters<<endl;
//                }
//            }else{
//                cout << "33% drew"<<endl;
//            }
//        }
//    }
//}
//
void City::moveTooters()
{
    for (int k = 0; k < m_nTooters; k++)
    {
      //Have the k-th Tooter in the city make one move.
      //        If that move results in that Tooter being orthogonally
      //        adjacent to the player, the player suffers a gas blast.
        m_tooters[k]->move();
    //player pos
        int pr = m_player->row(); 
        int pc = m_player->col();
    //tooter pos
        int totr = m_tooters[k]->row(); 
        int totc = m_tooters[k]->col();
    //if orthogonally adjacent to the player...
        if ((totr == pr && std::abs(totc - pc) == 1) || (totc == pc && std::abs(totr - pr) == 1)){
                m_player->getGassed();
        }
    }
}

void City::preachToTootersAroundPlayer() {
    // Get the player's position
    int playerRow = m_player->row();
    int playerCol = m_player->col();

    // Iterate through all Tooters
    for (int i = 0; i < m_nTooters; ++i) {
        // Skip nullptr Tooters
        Tooter* currentTooter = m_tooters[i];
        if (currentTooter == nullptr) {
            continue;
        }

        // Get the Tooter's position
        int tooterRow = currentTooter->row();
        int tooterCol = currentTooter->col();

        // Check if the Tooter is adjacent to the player
        if (isAdjacent(playerRow, playerCol, tooterRow, tooterCol)) {
            // Convert the Tooter with 2/3 probability
            if (randInt(1, 3) <= 2) {
                // Delete the Tooter and shift array to fill the gap
                delete currentTooter;

                for (int j = i; j < m_nTooters - 1; ++j) {
                    m_tooters[j] = m_tooters[j + 1];
                }

                m_tooters[m_nTooters - 1] = nullptr;
                m_nTooters--;
                i--;  // Decrement i to recheck the current position
            }
        }
    }
}

// Helper function to check if two positions are adjacent
bool City::isAdjacent(int row1, int col1, int row2, int col2) const {
    int rowDiff = abs(row1 - row2);
    int colDiff = abs(col1 - col2);
    return (rowDiff <= 1 && colDiff <= 1);
}



bool City::isInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nTooters)
{
    if (nTooters < 0)
    {
        cout << "***** Cannot create Game with negative number of Tooters!" << endl;
        exit(1);
    }
    if (nTooters > MAXTOOTERS)
    {
        cout << "***** Trying to create Game with " << nTooters
             << " Tooters; only " << MAXTOOTERS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nTooters > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the Tooters!" << endl;
        exit(1);
    }

        // Create city
    m_city = new City(rows, cols);

        // Add a player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_city->addPlayer(rPlayer, cPlayer);

      // Populate with Tooters
    while (nTooters > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a Tooter where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_city->addTooter(r, c);
        nTooters--;
    }
}

Game::~Game()
{
    delete m_city;
}

void Game::play()
{
    m_city->display();
    Player* p = m_city->player();
    if (p == nullptr)
        return;

    while ( ! p->isPassedOut()  &&  m_city->tooterCount() > 0)
    {
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player preaches
            p->preach();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                p->move(decodeDirection(action[0]));
                break;
            }
        }
        m_city->moveTooters();
        m_city->display();
    }
    if (p->isPassedOut())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

  // Return a random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rdv;
    static default_random_engine generator(rdv());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);
//    Game g(3, 4, 2);

      // Play the game
    g.play();
}

//#include <type_traits>
//#include <cassert>
//
//#define CHECKTYPE(c, f, r, a)  \
//    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
//       "FAILED: You changed the type of " #c "::" #f);  \
//    { [[gnu::unused]] auto p = static_cast<r(c::*)a>(&c::f); }
//
//void thisFunctionWillNeverBeCalled()
//{
//      // If the student deleted or changed the interfaces to the public
//      // functions, this won't compile.  (This uses magic beyond the scope
//      // of CS 31.)
//
//    Tooter(static_cast<City*>(0), 1, 1);
//    CHECKTYPE(Tooter, row, int, () const);
//    CHECKTYPE(Tooter, col, int, () const);
//    CHECKTYPE(Tooter, move, void, ());
//
//    Player(static_cast<City*>(0), 1, 1);
//    CHECKTYPE(Player, row, int, () const);
//    CHECKTYPE(Player, col, int, () const);
//    CHECKTYPE(Player, age, int, () const);
//    CHECKTYPE(Player, health, int, () const);
//    CHECKTYPE(Player, isPassedOut, bool, () const);
//    CHECKTYPE(Player, preach, void, ());
//    CHECKTYPE(Player, move, void, (int));
//    CHECKTYPE(Player, getGassed, void, ());
//
//    City(1, 1);
//    CHECKTYPE(City, rows, int, () const);
//    CHECKTYPE(City, cols, int, () const);
//    CHECKTYPE(City, player, Player*, () const);
//    CHECKTYPE(City, isPlayerAt, bool, (int,int) const);
//    CHECKTYPE(City, tooterCount, int, () const);
//    CHECKTYPE(City, nTootersAt, int, (int,int) const);
//    CHECKTYPE(City, determineNewPosition, bool, (int&,int&,int) const);
//    CHECKTYPE(City, display, void, () const);
//    CHECKTYPE(City, addTooter, bool, (int,int));
//    CHECKTYPE(City, addPlayer, bool, (int,int));
//    CHECKTYPE(City, preachToTootersAroundPlayer, void, ());
//    CHECKTYPE(City, moveTooters, void, ());
//
//    Game(1, 1, 1);
//    CHECKTYPE(Game, play, void, ());
//}
//
//void doBasicTests()
//{
//    {
//        City walk(10, 20);
//        assert(walk.addPlayer(2, 6));
//        Player* pp = walk.player();
//        assert(walk.isPlayerAt(2, 6)  && ! pp->isPassedOut());
//        pp->move(UP);
//        assert(walk.isPlayerAt(1, 6)  && ! pp->isPassedOut());
//        pp->move(UP);
//        assert(walk.isPlayerAt(1, 6)  && ! pp->isPassedOut());
//        for (int k = 1; k <= 11; k++)
//            pp->getGassed();
//        assert(! pp->isPassedOut());
//        pp->getGassed();
//        assert(pp->isPassedOut());
//    }
//    {
//        City ofAngels(2, 2);
//        assert(ofAngels.addPlayer(1, 1));
//        assert(ofAngels.addTooter(2, 2));
//        Player* pp = ofAngels.player();
//        ofAngels.moveTooters();
//        assert( ! pp->isPassedOut());
//        for (int k = 0; k < 1000  && ! pp->isPassedOut(); k++)
//            ofAngels.moveTooters();
//        assert(pp->isPassedOut());
//    }
//    {
//        City ousDarth(2, 2);
//        assert(ousDarth.addPlayer(1, 1));
//        for (int k = 0; k < 50; k++)
//        {
//            assert(ousDarth.addTooter(1, 2));
//            assert(ousDarth.addTooter(2, 2));
//        }
//        ousDarth.preachToTootersAroundPlayer();
//        assert(ousDarth.nTootersAt(1, 1) == 0);
//        assert(ousDarth.nTootersAt(2, 1) == 0);
//        for (int r = 1; r <= 2; r++)
//        {     // .9999 probability that between 5 and 29 out of 50 are unconverted
//            int n = ousDarth.nTootersAt(r, 2);
//            assert(n >= 5  &&  n <= 29);
//        }
//        int m = ousDarth.nTootersAt(1, 2);
//        assert(ousDarth.addTooter(1, 2));
//        assert(ousDarth.nTootersAt(1, 2) == m+1);
//    }
//    {
//        City univer(5, 20);
//        univer.addPlayer(3, 3);
//        int r = 1;
//        int c = 1;
//        for (int k = 1; k <= 5*5; k++)
//        {
//            if (r != 3 || c != 3)
//                univer.addTooter(r, c);
//            if (r == 5)
//            {
//                r = c+1;
//                c = 5;
//            }
//            else if (c == 1)
//            {
//                c = r + 1;
//                r = 1;
//            }
//            else
//            {
//                c--;
//                r++;
//            }
//        }
//        assert(univer.tooterCount() == 24);
////        cout<<"debug1 start"<<endl;
//        for (int k = 0; k < 1000  && univer.tooterCount() > 16; k++){
//            univer.preachToTootersAroundPlayer();
////            cout << "tooter Count: "<<univer.tooterCount() << endl;
////            cout << "K count: "<<k<<endl;
//        }
////        int temp = univer.tooterCount();//test
////        cout << temp;//test
//        assert(univer.tooterCount() == 16);
//        for (int r = 1; r <= 5; r++)
//        {
//            for (int c = 1; c <= 5; c++)
//            {
//                int expected = 1;
//                if (r >= 2  &&  r <= 4  &&  c >= 2  &&  c <= 4)
//                    expected = 0;
////                int waa = univer.nTootersAt(r, c);
////                cout << waa << "tootersatnum" <<endl;
//                assert(univer.nTootersAt(r, c) == expected);
//            }
//        }
//        univer.addTooter(3, 2);
//        assert(univer.tooterCount() == 17);
//          // If the program crashes after leaving this compound statement, you
//          // are probably messing something up when you delete a Tooter after
//          // it is converted (or you have mis-coded the destructor).
//          //
//          // Draw a picture of your m_tooters array before the Tooters are
//          // preached to and also note the values of m_nTooters or any other
//          // variables you might have that are involved with the number of
//          // Tooters.  Trace through your code step by step as the Tooters
//          // are preached to and removed, updating the picture according to
//          // what the code says, not what you want it to do.  If you don't see
//          // a problem then, try tracing through the destruction of the city.
//          //
//          // If you execute the code, use the debugger to check on the values
//          // of key variables at various points.  If you didn't try to learn
//          // to use the debugger, insert statements that write the values of
//          // key variables to cerr so you can trace the execution of your code
//          // and see the first place where something has gone amiss.  (Comment
//          // out the call to clearScreen in City::display so that your output
//          // doesn't disappear.)
//    }
//    cout << "Passed all basic tests" << endl;
//}
//int main(){
//    doBasicTests(); // Remove this line after completing test.
//    return 0;       // Remove this line after completing test.
//}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32  //  Windows

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Windows

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
