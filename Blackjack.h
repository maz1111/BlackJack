#pragma once

#include <QtWidgets/QMainWindow>
#include <ui_BlackJack.h>
#include <vector>
#include <memory>
#include <random>
#include <map>
#include <QLabel>
#include <QString>
#include <QTimer>

enum Suit { HEARTS, DIAMONDS, SPADES, CLUBS };
enum Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };


class Card {
public:
    Card(Suit suit, Rank rank);
    Rank getRank() const;
    Suit getSuit() const;
private:
    Suit suit_;
    Rank rank_;
};

class Deck {
public:
    Deck();
    void shuffleDeck();
    std::unique_ptr<Card> drawCard();
private:
    std::vector<std::unique_ptr<Card>> deck_;
};

class Player {
public:
    void addCard(std::unique_ptr<Card> card);
    int getScore() const;
    int getHandSize() const;
    std::unique_ptr<Card> playerLastCard();
    std::unique_ptr<Card> playerFirstCard();

private:
    std::vector<std::unique_ptr<Card>> hand_;
    int score_;
};

class Dealer : public Player {

};

class BlackJack : public QMainWindow
{
    Q_OBJECT

public:
    BlackJack(QWidget* parent = nullptr);
    ~BlackJack();
    const void displayPlayerCard();
    const void clearPlayerCards();
    const void displayDealerCard();
    const void clearDealerCards();
    const void updateLossCounter();
    const void updateWinCounter();

    void initialize();

public slots:
    void onHitButtonClicked();
    void onStandButtonClicked();
    void onContinueButtonClicked();
    void dealDealerCard();
    void checkEndConditions();

private:
    Ui::BlackJack ui;

    static std::vector<QString> playerObjectNames;
    static std::vector<QString> dealerObjectNames;
    static std::map<std::pair<Suit, Rank>, std::string> cardImagePaths;
    static std::string getPath(Suit suit, Rank rank);

    int lossCounter_ = 0;
    int winCounter_ = 0;

    Deck deck_;
    Player player_;
    Dealer dealer_;
};