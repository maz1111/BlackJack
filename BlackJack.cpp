#include "BlackJack.h"


std::map<std::pair<Suit, Rank>, std::string> BlackJack::cardImagePaths = {

{{HEARTS, ACE}, "./cards/ace_of_hearts.png"},
{{HEARTS, TWO}, "./cards/2_of_hearts.png"},
{{HEARTS, THREE}, "./cards/3_of_hearts.png"},
{{HEARTS, FOUR}, "./cards/4_of_hearts.png"},
{{HEARTS, FIVE}, "./cards/5_of_hearts.png"},
{{HEARTS, SIX}, "./cards/6_of_hearts.png"},
{{HEARTS, SEVEN}, "./cards/7_of_hearts.png"},
{{HEARTS, EIGHT}, "./cards/8_of_hearts.png"},
{{HEARTS, NINE}, "./cards/9_of_hearts.png"},
{{HEARTS, TEN}, "./cards/10_of_hearts.png"},
{{HEARTS, JACK}, "./cards/jack_of_hearts.png"},
{{HEARTS, QUEEN}, "./cards/queen_of_hearts.png"},
{{HEARTS, KING}, "./cards/king_of_hearts.png"},

{{DIAMONDS, ACE}, "./cards/ace_of_diamonds.png"},
{{DIAMONDS, TWO}, "./cards/2_of_diamonds.png"},
{{DIAMONDS, THREE}, "./cards/3_of_diamonds.png"},
{{DIAMONDS, FOUR}, "./cards/4_of_diamonds.png"},
{{DIAMONDS, FIVE}, "./cards/5_of_diamonds.png"},
{{DIAMONDS, SIX}, "./cards/6_of_diamonds.png"},
{{DIAMONDS, SEVEN}, "./cards/7_of_diamonds.png"},
{{DIAMONDS, EIGHT}, "./cards/8_of_diamonds.png"},
{{DIAMONDS, NINE}, "./cards/9_of_diamonds.png"},
{{DIAMONDS, TEN}, "./cards/10_of_diamonds.png"},
{{DIAMONDS, JACK}, "./cards/jack_of_diamonds.png"},
{{DIAMONDS, QUEEN}, "./cards/queen_of_diamonds.png"},
{{DIAMONDS, KING}, "./cards/king_of_diamonds.png"},

{{SPADES, ACE}, "./cards/ace_of_spades.png"},
{{SPADES, TWO}, "./cards/2_of_spades.png"},
{{SPADES, THREE}, "./cards/3_of_spades.png"},
{{SPADES, FOUR}, "./cards/4_of_spades.png"},
{{SPADES, FIVE}, "./cards/5_of_spades.png"},
{{SPADES, SIX}, "./cards/6_of_spades.png"},
{{SPADES, SEVEN}, "./cards/7_of_spades.png"},
{{SPADES, EIGHT}, "./cards/8_of_spades.png"},
{{SPADES, NINE}, "./cards/9_of_spades.png"},
{{SPADES, TEN}, "./cards/10_of_spades.png"},
{{SPADES, JACK}, "./cards/jack_of_spades.png"},
{{SPADES, QUEEN}, "./cards/queen_of_spades.png"},
{{SPADES, KING}, "./cards/king_of_spades.png"},

{{CLUBS, ACE}, "./cards/ace_of_clubs.png"},
{{CLUBS, TWO}, "./cards/2_of_clubs.png"},
{{CLUBS, THREE}, "./cards/3_of_clubs.png"},
{{CLUBS, FOUR}, "./cards/4_of_clubs.png"},
{{CLUBS, FIVE}, "./cards/5_of_clubs.png"},
{{CLUBS, SIX}, "./cards/6_of_clubs.png"},
{{CLUBS, SEVEN}, "./cards/7_of_clubs.png"},
{{CLUBS, EIGHT}, "./cards/8_of_clubs.png"},
{{CLUBS, NINE}, "./cards/9_of_clubs.png"},
{{CLUBS, TEN}, "./cards/10_of_clubs.png"},
{{CLUBS, JACK}, "./cards/jack_of_clubs.png"},
{{CLUBS, QUEEN}, "./cards/queen_of_clubs.png"},
{{CLUBS, KING}, "./cards/king_of_clubs.png"}
};
std::vector<QString> BlackJack::playerObjectNames = { "playersCard1", "playersCard2", "playersCard3", "playersCard4", "playersCard5", "playersCard6", "playersCard7" };
std::vector<QString> BlackJack::dealerObjectNames = { "dealersCard1", "dealersCard2", "dealersCard3", "dealersCard4", "dealersCard5", "dealersCard6", "dealersCard7" };


BlackJack::BlackJack(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initialize();

    connect(ui.hitButton, &QPushButton::clicked, this, &BlackJack::onHitButtonClicked);
    connect(ui.standButton, &QPushButton::clicked, this, &BlackJack::onStandButtonClicked);
    connect(ui.continueButton, &QPushButton::clicked, this, &BlackJack::onContinueButtonClicked);
}

BlackJack::~BlackJack()
{}

void BlackJack::initialize() {
    ui.hitButton->setDisabled(false);
    ui.standButton->setDisabled(false);
    ui.bustedText->setVisible(false);
    ui.drawText->setVisible(false);
    ui.lostText->setVisible(false);
    ui.wonText->setVisible(false);
    ui.continueButton->setVisible(false);
    ui.continueButton->setDisabled(true);

    deck_.shuffleDeck();

    player_.addCard(deck_.drawCard());
    displayPlayerCard();
    player_.addCard(deck_.drawCard());
    displayPlayerCard();

    dealer_.addCard(deck_.drawCard());
    QPixmap cardImage("./cards/card_back.png");
    ui.dealersCard1->setPixmap(cardImage);
    dealer_.addCard(deck_.drawCard());
    displayDealerCard();
}

void BlackJack::onHitButtonClicked() {
    player_.addCard(deck_.drawCard());
    displayPlayerCard();
    if (player_.getScore() > 21) {
        ui.hitButton->setDisabled(true);
        ui.standButton->setDisabled(true);
        ui.bustedText->setVisible(true);
        ui.continueButton->setVisible(true);
        ui.continueButton->setDisabled(false);
        lossCounter_++;
        updateLossCounter();
    }
}

void BlackJack::onStandButtonClicked() {
    QPixmap cardImage(QString::fromStdString(getPath(dealer_.playerFirstCard()->getSuit(), dealer_.playerFirstCard()->getRank())));
    ui.dealersCard1->setPixmap(cardImage);

    ui.hitButton->setDisabled(true);
    ui.standButton->setDisabled(true);

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(1000);

    if (dealer_.getScore() > player_.getScore()) {
        ui.lostText->setVisible(true);
        ui.continueButton->setVisible(true);
        ui.continueButton->setDisabled(false);
        lossCounter_++;
        updateLossCounter();
        return;
    }

    if (dealer_.getScore() == player_.getScore()) {
        if (dealer_.getScore() < 17) {
            dealDealerCard();
        }
        else {
            ui.drawText->setVisible(true);
            ui.continueButton->setVisible(true);
            ui.continueButton->setDisabled(false);
        }
        return;
    }

    connect(timer, &QTimer::timeout, this, &BlackJack::dealDealerCard);
}

void BlackJack::dealDealerCard() {
    dealer_.addCard(deck_.drawCard());
    displayDealerCard();

    if (dealer_.getScore() < player_.getScore()) {
        QTimer::singleShot(1000, this, &BlackJack::dealDealerCard);
    }
    else {
        QTimer::singleShot(1000, this, &BlackJack::checkEndConditions);
    }
}

void BlackJack::checkEndConditions() {
    if (dealer_.getScore() <= 21) {
        ui.hitButton->setDisabled(true);
        ui.standButton->setDisabled(true);
        ui.lostText->setVisible(true);
        ui.continueButton->setVisible(true);
        ui.continueButton->setDisabled(false);
        lossCounter_++;
        updateLossCounter();
    }
    else if (dealer_.getScore() == player_.getScore()) {
        if (dealer_.getScore() < 17) {
            dealDealerCard();
        }
        else {
            ui.hitButton->setDisabled(true);
            ui.standButton->setDisabled(true);
            ui.drawText->setVisible(true);
            ui.continueButton->setVisible(true);
            ui.continueButton->setDisabled(false);
        }
    }
    else {
        ui.hitButton->setDisabled(true);
        ui.standButton->setDisabled(true);
        ui.wonText->setVisible(true);
        ui.continueButton->setVisible(true);
        ui.continueButton->setDisabled(false);
        winCounter_++;
        updateWinCounter();
    }
}

void BlackJack::onContinueButtonClicked() {
    clearPlayerCards();
    clearDealerCards();
    player_ = Player();
    dealer_ = Dealer();
    initialize();
}

const void BlackJack::displayPlayerCard() {
    if (player_.getHandSize() > 0) {
        QString objectName = playerObjectNames[player_.getHandSize() - 1];
        QLabel* label = findChild<QLabel*>(objectName);
        if (label != nullptr) {
            QPixmap cardImage(QString::fromStdString(getPath(player_.playerLastCard()->getSuit(), player_.playerLastCard()->getRank())));
            label->setPixmap(cardImage);
        }
    }
}

const void BlackJack::clearPlayerCards() {
    for (int i = player_.getHandSize() - 1; i >= 0; i--)
    {
        QString objectName = playerObjectNames[i];
        QLabel* label = findChild<QLabel*>(objectName);
        label->clear();
    }
}

const void BlackJack::displayDealerCard() {
    if (dealer_.getHandSize() > 0) {
        QString objectName = dealerObjectNames[dealer_.getHandSize() - 1];
        QLabel* label = findChild<QLabel*>(objectName);
        if (label != nullptr) {
            QPixmap cardImage(QString::fromStdString(getPath(dealer_.playerLastCard()->getSuit(), dealer_.playerLastCard()->getRank())));
            label->setPixmap(cardImage);
        }
    }
}

const void BlackJack::clearDealerCards() {
    for (int i = dealer_.getHandSize() - 1; i >= 0; i--)
    {
        QString objectName = dealerObjectNames[i];
        QLabel* label = findChild<QLabel*>(objectName);
        label->clear();
    }
}

const void BlackJack::updateLossCounter() {
    ui.lossCounter->setText(QString::number(lossCounter_));
}

const void BlackJack::updateWinCounter() {
    ui.winCounter->setText(QString::number(winCounter_));
}

Card::Card(Suit suit, Rank rank) : suit_(suit), rank_(rank) {}

Rank Card::getRank() const {
    return rank_;
}

Suit Card::getSuit() const {
    return this->suit_;
}

std::string BlackJack::getPath(Suit suit, Rank rank) {
    auto path = cardImagePaths.find(std::make_pair(suit, rank));
    if (path != cardImagePaths.end()) {
        return path->second;
    }
    else {
        qDebug("Image path not found");
        return "./cards/card_back.png";
    }
}

Deck::Deck() {
    for (int suit = HEARTS; suit <= CLUBS; ++suit) {
        for (int rank = ACE; rank <= KING; ++rank) {
            deck_.push_back(std::make_unique<Card>(static_cast<Suit>(suit), static_cast<Rank>(rank)));
        }
    }
}

void Deck::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck_.begin(), deck_.end(), g);
}

std::unique_ptr<Card> Deck::drawCard() {
    if (deck_.empty()) {
        for (int suit = HEARTS; suit <= CLUBS; ++suit) {
            for (int rank = ACE; rank <= KING; ++rank) {
                deck_.push_back(std::make_unique<Card>(static_cast<Suit>(suit), static_cast<Rank>(rank)));
            }
        }
    }
    std::unique_ptr<Card> card = std::move(deck_.back());
    deck_.pop_back();
    return card;
}

void Player::addCard(std::unique_ptr<Card> card) {
    hand_.push_back(std::move(card));
}

int Player::getScore() const {
    if (hand_.empty()) {
        qDebug("The hand vector is empty");
        return 0;
    }

    int value = 0;
    int acesNumber = 0;

    for (const auto& card : hand_) {
        int cardValue = card->getRank();
        if (cardValue > 10) {
            cardValue = 10;
        }
        else if (cardValue == 1) {
            acesNumber++;
            cardValue = 11;
        }
        value += cardValue;
    }

    while (value > 21 && acesNumber > 0) {
        acesNumber--;
        value -= 10;
    }

    return value;
}

int Player::getHandSize() const {
    return hand_.size();
}

std::unique_ptr<Card> Player::playerLastCard() {
    if (!hand_.empty()) {
        const Card& lastCard = *hand_.back();
        return std::make_unique<Card>(lastCard.getSuit(), lastCard.getRank());
    }
    else {
        return nullptr;
    }
}

std::unique_ptr<Card> Player::playerFirstCard() {
    if (!hand_.empty()) {
        const Card& firstCard = *hand_.front();
        return std::make_unique<Card>(firstCard.getSuit(), firstCard.getRank());
    }
    else {
        return nullptr;
    }
}