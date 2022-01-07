#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>
#include <assert.h>

#include "Game.h"
#include "Key.h"
#include "Door.h"
#include "Exit.h"
#include "Enemy.h"
#include "Money.h"

using namespace std;

Game::Game()
    : m_gameOver(false)
    , m_hasDied(false)
    , m_player(1, 1, Color::Player)
    , m_healthAtStart(100)
    , m_moneyAtStart(0)
{
}
Game::~Game() {
}

int keyWarnCounter = 0;

// ==================================================================================================================
// main game loop until X in the final level has been reached or player is dead
// ==================================================================================================================
void Game::Run() {
    m_player.SetHealth(hlp.kPlayerStartingHealth);
    while (!m_gameOver) {
        // conditional: load the first or the next level layout from an applicable level.txt file and convert the characters & actors
        // check if the active level lines up with level id the player should be on (which is increased after finding an exit tile during the update function)
        if (m_level.GetActiveLevel() < m_level.GetCurrentLevel()) {
            // try to load next level layout
            m_level.LoadLevel();
            m_player.SetPosition(m_level.GetStartX(), m_level.GetStartY());
            m_player.ResetKeys(); // reset active key amount to 0 on each new level
            system("cls"); // clear screen
            // saving current health and money for a potential level reset rollback
            m_healthAtStart = m_player.GetHealth();
            m_moneyAtStart = m_player.GetMoney();
        }
        // always: clear screen, display inventory items, draw the level & actors, add the symbol legend, then wait for input
        hlp.ClearScreen(); // custom screen "clear" function (not using CLS to avoid some flickering)
        m_interface.Inventory(m_level.GetCurrentLevel(), m_player.GetPositionX(), m_player.GetPositionY(), m_player.KeyAmount(), m_player.HasEasteregg(), m_level.GetLevelName(), m_player.GetHealth(), m_player.GetMoney()); // draw current inventory
        Draw(); // draw actors
        m_interface.Legend(m_player.KeyWarning()); // draw the icon legend
        if (m_player.KeyWarning()) {
            keyWarnCounter++;
            if (keyWarnCounter > 50) {
                keyWarnCounter = 0;
                m_player.KeyWarningSwitch(); // set key warning to false for next loop
            }
        }
        m_gameOver = Update(); // run the input/update loop
        if (m_level.m_quitToMenu) { // player pressed escape, this will instantly go back to the main menu
            m_gameOver = true;
        }
    }
    // conditional screen calls
    if (!m_level.m_quitToMenu && !m_hasDied) m_interface.GameOverScreen(m_player.HasEasteregg(), m_player.GetMoney(), m_player.GetHealth()); // end game screen
    else if (m_hasDied) {
        m_interface.DeathScreen(); // game over death screen
    }
    else {
        // back to main menu
    }
    m_hasDied = false;
    m_gameOver = false;
    m_level.m_quitToMenu = false;
    m_player.ResetGame();
}
// ==================================================================================================================

// input checks and temporary position update tied into collision function
bool Game::Update() {
    Sleep(hlp.kGameSpeed); // update loop slowdown (modifies general game "speed")

    int newPlayerX = m_player.GetPositionX();
    int newPlayerY = m_player.GetPositionY();

    // real time movement input checks for e.g. W-A-S-D and Arrow keys (definable in the helpers and input classes)
    // using a boolean to set key down/key up states to avoid multiple step movement with one key press
    // only reset a keyPressed state if all appropriate keys are released
    // additionally, start a timer (counter) for auto movement in one direction after holding down a key for a while
    
    if (in.KeyDown(VK_UP) || in.KeyDown(in.kMoveUp) || in.KeyDown(VK_DOWN) || in.KeyDown(in.kMoveDown) ||
        in.KeyDown(VK_RIGHT) || in.KeyDown(in.kMoveRight) || in.KeyDown(VK_LEFT) || in.KeyDown(in.kMoveLeft)) {
        in.keyHeldTimer++;
    } else in.keyHeldTimer = 0;

    if ((!in.keyPressedUp || in.keyHeldTimer > in.keyHeldTimerMax) && (in.KeyDown(VK_UP) || in.KeyDown(in.kMoveUp))) {
        newPlayerY--;
        in.keyPressedUp = true;
    } else if ((!in.KeyDown(VK_UP) && !in.KeyDown(in.kMoveUp))) in.keyPressedUp = false;
    if ((!in.keyPressedDown || in.keyHeldTimer > in.keyHeldTimerMax) && (in.KeyDown(VK_DOWN) || in.KeyDown(in.kMoveDown))) {
        newPlayerY++;
        in.keyPressedDown = true;
    } else if ((!in.KeyDown(VK_DOWN) && !in.KeyDown(in.kMoveDown))) in.keyPressedDown = false;
    if ((!in.keyPressedLeft || in.keyHeldTimer > in.keyHeldTimerMax) && (in.KeyDown(VK_LEFT) || in.KeyDown(in.kMoveLeft))) {
        newPlayerX--;
        in.keyPressedLeft = true;
    } else if ((!in.KeyDown(VK_LEFT) && !in.KeyDown(in.kMoveLeft))) in.keyPressedLeft = false;
    if ((!in.keyPressedRight || in.keyHeldTimer > in.keyHeldTimerMax) && (in.KeyDown(VK_RIGHT) || in.KeyDown(in.kMoveRight))) {
        newPlayerX++;
        in.keyPressedRight = true;
    } else if ((!in.KeyDown(VK_RIGHT) && !in.KeyDown(in.kMoveRight))) in.keyPressedRight = false;

    // checking escape key input
    if (in.KeyDown(VK_ESCAPE)) {
        m_level.m_quitToMenu = true; // quit to main menu (progress not saved)
    }
    // checking reset key input
    if (in.KeyDown(hlp.kReset)) {
        // roll back values to when current level was loaded
        m_level.ResetCurrentLevel();
        m_player.SetHealth(m_healthAtStart);
        m_player.SetMoney(m_moneyAtStart);
        return false;
    }
    // reset new player coordinates back to previous player position if running into a wall
    // prevents "dodging" enemies that move onto the same tile while the game thinks the player is inside the wall during the enemy collision check
    if (m_level.IsWall(newPlayerX, newPlayerY)) {
        newPlayerX = m_player.GetPositionX();
        newPlayerY = m_player.GetPositionY();
    }
    return CollisionHandler(newPlayerX, newPlayerY); // tie bool return to actual collision outcome
}

// check if the player collided with any active actor in the level and handle this accordingly
bool Game::CollisionHandler(int newPlayerX, int newPlayerY) {
    Actor* collidedActor = m_level.UpdateActors(newPlayerX, newPlayerY);
    if (collidedActor != nullptr && collidedActor->IsActive()) {
        
        switch (collidedActor->GetType()) {
        case ActorType::Enemy: // player collided with an enemy actor
        {
            Enemy* collidedEnemy = dynamic_cast<Enemy*>(collidedActor);
            assert(collidedEnemy);

            if (collidedEnemy->GetColor() == Color::EnemyW) {
                m_player.ChangeHealth(hlp.kEnemyWeak); // subtract low amount from player's health
            }
            else if (collidedEnemy->GetColor() == Color::EnemyS) {
                m_player.ChangeHealth(hlp.kEnemyStrong); // subtract high amount from player's health
            }
            else {
                // safety, but should not occur ever
            }
            collidedEnemy->Remove(); // disable actor for this level
            m_player.SetPosition(newPlayerX, newPlayerY);
            if (m_player.GetHealth() <= 0) { // player health check
                m_hasDied = true; // additional state to call the correct game over screen
                return true; // since player is dead, enable the game over state
            }
            else hlp.SoundPlayer(6); // enemy hit sound only if player not dead
            break;
        }
        case ActorType::Money: // player collided with a money actor
        {
            Money* collidedMoney = dynamic_cast<Money*>(collidedActor);
            assert(collidedMoney);
            collidedMoney->Remove(); // disable actor for this level
            m_player.AddMoney(collidedMoney->GetWorth());
            hlp.SoundPlayer(8); // money pickup sound
            m_player.SetPosition(newPlayerX, newPlayerY);
            break;
        }
        case ActorType::Key: // player collided with a key actor
        {
            Key* collidedKey = dynamic_cast<Key*>(collidedActor);
            assert(collidedKey);
            if (collidedKey->GetColor() == Color::Red) { // red key
                if (collidedKey->isUnidentified) {
                    m_player.PickupKey(Color::Red, true);
                }
                else m_player.PickupKey(Color::Red, false);
            }
            else if (collidedKey->GetColor() == Color::Green) { // green key
                if (collidedKey->isUnidentified) {
                    m_player.PickupKey(Color::Green, true);
                }
                else m_player.PickupKey(Color::Green, false);
            }
            else if (collidedKey->GetColor() == Color::Blue) { // blue key
                if (collidedKey->isUnidentified) {
                    m_player.PickupKey(Color::Blue, true);
                }
                else m_player.PickupKey(Color::Blue, false);
            }
            else if (collidedKey->GetColor() == Color::White) { // white key
                if (collidedKey->isUnidentified) {
                    m_player.PickupKey(Color::White, true);
                }
                else m_player.PickupKey(Color::White, false);
            }
            else {
                // safety, but should not occur ever
            }
            hlp.SoundPlayer(1); // key pickup sound
            collidedKey->Remove();
            m_player.SetPosition(newPlayerX, newPlayerY);
            break;
        }
        case ActorType::Door: // player collided with a door actor
        {
            Door* collidedDoor = dynamic_cast<Door*>(collidedActor);
            assert(collidedDoor);
            if (collidedDoor->IsOpen()) { // door already unlocked, just move the character
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::Red && *(m_player.KeyAmount()) >= 1) { // door is red, locked and needs a red key
                m_player.UseKey(Color::Red, false); // remove 1 red key from inventory
                hlp.SoundPlayer(2); // door open sound
                collidedDoor->Open(); // "open" the door and set it to a greyed-out state that can be passed at any time now
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::Green && *(m_player.KeyAmount() + 1) >= 1) { // door is green, locked and needs a green key
                m_player.UseKey(Color::Green, false); // remove 1 green key from inventory
                hlp.SoundPlayer(2);
                collidedDoor->Open();
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::Blue && *(m_player.KeyAmount() + 2) >= 1) { // door is blue, locked and needs a blue key
                m_player.UseKey(Color::Blue, false); // remove 1 blue key from inventory
                hlp.SoundPlayer(2);
                collidedDoor->Open();
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::White && *(m_player.KeyAmount() + 3) >= 1) { // door is white, locked and needs a white key
                m_player.UseKey(Color::White, false); // remove 1 white key from inventory
                hlp.SoundPlayer(2);
                collidedDoor->Open();
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::Red && *(m_player.KeyAmount() + 4) >= 1) { // door is red, locked and needs a red key
                m_player.UseKey(Color::Red, true); // remove 1 unidentified red key from inventory
                hlp.SoundPlayer(2); // door open sound
                collidedDoor->Open(); // "open" the door and set it to a greyed-out state that can be passed at any time now
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::Green && *(m_player.KeyAmount() + 5) >= 1) { // door is green, locked and needs a green key
                m_player.UseKey(Color::Green, true); // remove 1 unidentified green key from inventory
                hlp.SoundPlayer(2); // door open sound
                collidedDoor->Open(); // "open" the door and set it to a greyed-out state that can be passed at any time now
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::Blue && *(m_player.KeyAmount() + 6) >= 1) { // door is blue, locked and needs a blue key
                m_player.UseKey(Color::Blue, true); // remove 1 unidentified blue key from inventory
                hlp.SoundPlayer(2); // door open sound
                collidedDoor->Open(); // "open" the door and set it to a greyed-out state that can be passed at any time now
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else if (collidedDoor->GetColor() == Color::White && *(m_player.KeyAmount() + 7) >= 1) { // door is white, locked and needs a white key
                m_player.UseKey(Color::White, true); // remove 1 unidentified white key from inventory
                hlp.SoundPlayer(2); // door open sound
                collidedDoor->Open(); // "open" the door and set it to a greyed-out state that can be passed at any time now
                m_player.SetPosition(newPlayerX, newPlayerY);
            }
            else {
                // don't open door, don't move player and show "missing key" warning below the legend
                if (!m_player.KeyWarning()) {
                    hlp.SoundPlayer(3); // door locked sound
                    m_player.KeyWarningSwitch();
                }
            }
            break;
        }
        case ActorType::Exit:
        {
            Exit* collidedExit = dynamic_cast<Exit*>(collidedActor);
            assert(collidedExit);
            collidedExit->Remove(); // disable actor for this level
            m_player.SetPosition(newPlayerX, newPlayerY);
            // if final level has not been reached, increase value of current level (which will be checked in the "Run" function)
            // otherwise end the game
            if (m_level.GetCurrentLevel() < m_level.GetFinalLevel()) {
                m_level.IncreaseLevel();
                hlp.SoundPlayer(5); // level change sound
            }
            else {
                return true; // final exit, end game
            }
            break;
        }
        default:

            break;
        }
    }
    else if (m_level.IsSpace(newPlayerX, newPlayerY)) { // next tile would be empty/free
        m_player.SetPosition(newPlayerX, newPlayerY);
    }
    else if (m_level.IsEasteregg(newPlayerX, newPlayerY)) { // next tile would be the easteregg
        m_player.PickupEasteregg();
        m_level.ClearTile(newPlayerX, newPlayerY);
        m_player.SetPosition(newPlayerX, newPlayerY);
    }
    else {
    // safety, do nothing
    }
    return false;
}

void Game::Draw() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    m_level.DrawMap(); // draw the level layout and all actors
    
    COORD actorPosition;
    actorPosition.X = m_player.GetPositionX() + hlp.kOffsetX; // +offset for level array x position adjustment
    actorPosition.Y = m_player.GetPositionY() + hlp.kOffsetY; // +offset for inventory display above the level array
    SetConsoleCursorPosition(console, actorPosition); // move the cursor to the (new) player's location
    m_player.Draw(); // draw the player character

    COORD finalCursorPos = { 0, (short)m_level.GetHeight() + hlp.kOffsetY };
    SetConsoleCursorPosition(console, finalCursorPos); // move cursor to below the level layout, including offset
}