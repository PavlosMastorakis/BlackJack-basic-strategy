#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

void playerDraw(int& Hand, int hand1, int hand2, vector<vector<string>>& deck, vector<vector<int>>& deck_values) {
    while (Hand < 17) {
        int r = rand() % 4;
        int c = rand() % 13;
        while (deck[r][c] == "NULL") {
            r = rand() % 4;
            c = rand() % 13;
        }
        Hand = Hand + deck_values[r][c];
        if (deck_values[r][c] == 11 && Hand > 21) { Hand -= 10; }
        cout << "extra card: " << deck_values[r][c] << "\n";
        deck[r][c] = "NULL";
        deck_values[r][c] = NULL;
    }
}

void dealerDraw(int& Hand, vector<vector<string>>& deck, vector<vector<int>>& deck_values) {
    while (Hand < 17) {
        int r = rand() % 4;
        int c = rand() % 13;
        while (deck[r][c] == "NULL") {
            r = rand() % 4;
            c = rand() % 13;
        }
        Hand = Hand + deck_values[r][c];
        if (deck_values[r][c] == 11 && Hand > 21) { Hand -= 10; }
        cout << "extra card: " << deck_values[r][c] << "\n";
        deck[r][c] = "NULL";
        deck_values[r][c] = NULL;
    }
}

void doubleDown(int& playerHand, vector<vector<string>>& deck, vector<vector<int>>& deck_values, double& bet, double& wallet) {
    int r = rand() % 4;
    int c = rand() % 13;
    while (deck[r][c] == "NULL") {
        r = rand() % 4;
        c = rand() % 13;
    }
    playerHand = playerHand + deck_values[r][c];
    cout << "extra card for DD: " << deck_values[r][c] << "\n";
    deck[r][c] = "NULL";
    deck_values[r][c] = NULL;
    if (wallet - bet >= 0) {
        bet *= 2;
    }
    else {
        bet += wallet;
    }
}

int drawOneForSplit(int& playerHand, vector<vector<string>>& deck, vector<vector<int>>& deck_values) {
    int r = rand() % 4, ret;
    int c = rand() % 13;
    while (deck[r][c] == "NULL") {
        r = rand() % 4;
        c = rand() % 13;
    }
    ret = deck_values[r][c];
    playerHand = playerHand + deck_values[r][c];
    cout << "extra card: " << deck_values[r][c] << "\n";
    deck[r][c] = "NULL";
    deck_values[r][c] = NULL;
    return ret;
}

vector<int> split(int& playerHand, vector<vector<string>>& deck, vector<vector<int>>& deck_values, double& bet, double& wallet) {
    int playerHand1 = playerHand / 2, playerHand2 = playerHand1;
    int secCard1 = drawOneForSplit(playerHand1, deck, deck_values);
    int secCard2 = drawOneForSplit(playerHand2, deck, deck_values);
    playerDraw(playerHand1, playerHand1 - secCard1, secCard1, deck, deck_values);
    playerDraw(playerHand2, playerHand2 - secCard2, secCard2, deck, deck_values);
    vector<int> results;
    results.push_back(playerHand1);
    results.push_back(playerHand2);
    wallet -= bet;
    return results;
}

bool checkForBJ(int h_v[2]) {
    // Checks if the player or the dealer have BJ in their initial hands.
    if (h_v[0] == 11 && h_v[1] == 10 || h_v[0] == 10 && h_v[1] == 11) { return true; }
    else { return false; }
}

int main()
{
    srand(time(0));
    int n = 10000, defeats = 0, internal_reps = 0;
    
    
    for (int MCrepeats = 0; MCrepeats < n; MCrepeats++) {
        
        double wallet = 50, bet;
        int r, c, prev_run, d_h_v[2], p_h_v[2];
        string d_h[2], p_h[2];
        prev_run = -1; // 2 αν κέρδισε, 1 αν έφερε ισοπαλία, 0 αν έχασε στον προηγούμενο γύρο (-1 για την πρώτη φορά),
                       // 3 για surrender, 4 για split

        while (wallet > 0 && wallet < 100) {

            //Deck creation
            vector<vector<string>> deck = { {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"},
                                   {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"},
                                   {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"},
                                   {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"} };
            vector<vector<int>> deck_values = { { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 },
                                       { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 },
                                       { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 },
                                       { 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10 } };

            if (prev_run == -1 || prev_run == 2 || prev_run == 4) { bet = 1; }
            if (prev_run == 0) { bet = (wallet - 2 * bet >= 0) ? (2 * bet) : (wallet); }
            // if surrender, or Draw, bet = bet

            //RNG tests ~ d_h=dealer's hand ~ d_h_v=dealer's hand value ~ p_h=player's hand ~ p_h_v=player's hand value
            r = rand() % 4;
            c = rand() % 13;
            d_h[0] = deck[r][c];
            d_h_v[0] = deck_values[r][c];
            deck[r][c] = "NULL";
            deck_values[r][c] = NULL;
            cout << "Dealer's Hand: " << d_h[0] << " with value " << d_h_v[0] << endl;

            //
            for (int i = 0; i < 2; i++) {
                r = rand() % 4;
                c = rand() % 13;
                while (deck[r][c] == "NULL") {
                    r = rand() % 4;
                    c = rand() % 13;
                }
                p_h[i] = deck[r][c];
                p_h_v[i] = deck_values[r][c];
                deck[r][c] = "NULL";
                deck_values[r][c] = NULL;
            }
            cout << "Player's hand: " << p_h[0] << " and " << p_h[1] << " with sum " << p_h_v[0] + p_h_v[1] << endl;
            cout << "Bet = " << bet << endl;

            // Core calculations for 1 game rep
            bool playerBJ = checkForBJ(p_h_v), end = false, Win = false, Draw = false, Lose = false, Win2 = false, 
                Draw2 = false, Lose2 = false, surrender = false;

            if (playerBJ) {
                end = true;
                r = rand() % 4;
                c = rand() % 13;
                while (deck[r][c] == "NULL") {
                    r = rand() % 4;
                    c = rand() % 13;
                }
                d_h[1] = deck[r][c];
                d_h_v[1] = deck_values[r][c];
                deck[r][c] = "NULL";
                deck_values[r][c] = NULL;
                cout << "Dealer's extra card: " << d_h[1] << " with value " << d_h_v[1] << endl;
                bool dealerBJ = checkForBJ(d_h_v);
                if (dealerBJ) { Draw = true; }
                else { Win = true; }
            }
            if (Draw) { prev_run = 1; } // wallet unchanged
            if (Win) { prev_run = 2; wallet += 2 * bet; }

            if (!end) {

                int playerHand = p_h_v[0] + p_h_v[1], dealerHand = d_h_v[0], playerHandForSplit(0);
                bool split = false, hit = false, dhit = false, pairs = false, oneace = false;

                // Pairs
                if (p_h_v[0] == p_h_v[1]) {
                    pairs = true;
                    // A,A
                    if (p_h_v[0] == 11) { 
                        split = true;
                    }
                    // 10,10
                    // 9,9
                    if (p_h_v[0] == 9 && d_h_v[0] != 7 && d_h_v[0] != 10 && d_h_v[0] != 11) {
                        split = true;
                    }
                    // 8,8
                    if (p_h_v[0] == 8) {
                        if (d_h_v[0] == 11) { surrender = true; }
                        else { split = true; }
                    }
                    // 7,7
                    if (p_h_v[0] == 7) {
                        if (d_h_v[0] < 8) { split = true; }
                        else { hit = true; }
                    }
                    // 6,6
                    if (p_h_v[0] == 6) {
                        if (d_h_v[0] < 7) { split = true; }
                        else { hit = true; }
                    }
                    // 5,5
                    if (p_h_v[0] == 5) {
                        if (d_h_v[0] < 10) { dhit = true; }
                        else { hit = true; }
                    }
                    // 4,4
                    if (p_h_v[0] == 4) {
                        if (d_h_v[0] == 5 || d_h_v[0] == 6) { split = true; }
                        else { hit = true; }
                    }
                    // 2,2 or 3,3
                    if (p_h_v[0] == 2 || p_h_v[0] == 3) {
                        if (d_h_v[0] < 8) { split = true; }
                        else { hit = true; }
                     }
                }

                // Soft totals (one A)
                if ((p_h_v[0] == 11 || p_h_v[1] == 11) && !pairs) {
                    oneace = true;
                    int ace = 11, other = (p_h_v[0] == 11) ? (p_h_v[1]) : (p_h_v[0]);
                    // A,9
                    // A,8
                    if (other == 8 && d_h_v[0] == 6) { dhit = true; }
                    // A,7
                    if (other == 7 && d_h_v[0] < 7) { dhit = true; }
                    if (other == 7 && d_h_v[0] > 8) { hit = true; }
                    // A,6
                    if (other == 6 && d_h_v[0] > 2 && d_h_v[0] < 7) { dhit = true; }
                    if (other == 6 && (d_h_v[0] < 3 || d_h_v[0] > 6)) { hit = true; }
                    // A,4 or A,5
                    if ((other == 4 || other == 5) && d_h_v[0] > 3 && d_h_v[0] < 7) { dhit = true; }
                    if ((other == 4 || other == 5) && (d_h_v[0] < 4 || d_h_v[0] > 6)) { hit = true; }
                    // A,2 or A,3
                    if ((other == 2 || other == 3) && d_h_v[0] > 4 && d_h_v[0] < 7) { dhit = true; }
                    if ((other == 2 || other == 3) && (d_h_v[0] < 5 || d_h_v[0] > 6)) { hit = true; }
                }

                // Hard totals (no A)
                if ((p_h_v[0] != 11 || p_h_v[1] != 11) && !pairs && !oneace) {
                    // 18-21
                    // 17
                    if (playerHand == 17 && d_h_v[0] == 11) { surrender = true; }
                    // 16
                    if (playerHand == 16 && (d_h_v[0] > 6 && d_h_v[0] < 9)) { hit = true; }
                    if (playerHand == 16 && d_h_v[0] > 8) { surrender = true; }
                    // 15
                    if (playerHand == 15 && (d_h_v[0] > 6 && d_h_v[0] < 10)) { hit = true; }
                    if (playerHand == 15 && d_h_v[0] > 9) { surrender = true; }
                    // 13 or 14
                    if ((playerHand == 13 || playerHand == 14) && d_h_v[0] > 6) { hit = true; }
                    // 12
                    if (playerHand == 12 && (d_h_v[0] < 4 || d_h_v[0] > 6)) { hit = true; }
                    // 11
                    if (playerHand == 11) { dhit = true; }
                    // 10
                    if (playerHand == 10 && d_h_v[0] > 9) { hit = true; }
                    if (playerHand == 10 && d_h_v[0] < 10) { dhit = true; }
                    // 9
                    if (playerHand == 9 && d_h_v[0] > 2 && d_h_v[0] < 7) { dhit = true; }
                    if (playerHand == 9 && (d_h_v[0] < 3 || d_h_v[0] > 6)) { hit = true; }
                    // 5-8
                    if (playerHand > 4 && playerHand < 9) { hit = true; }
                }

                if (split) {
                    if (wallet - 2 * bet >= 0) {
                        playerHand = drawOneForSplit(p_h_v[0], deck, deck_values);
                        playerHandForSplit = drawOneForSplit(p_h_v[1], deck, deck_values);
                    }
                    else {
                        playerDraw(playerHand, p_h_v[0], p_h_v[1], deck, deck_values);
                    }
                }
                if (dhit) {
                    if (wallet - 2 * bet >= 0) {
                        doubleDown(playerHand, deck, deck_values, bet, wallet);
                    }
                    else {
                        playerDraw(playerHand, p_h_v[0], p_h_v[1], deck, deck_values);
                    }
                }
                if (hit) { playerDraw(playerHand, p_h_v[0], p_h_v[1], deck, deck_values); }

                cout << "Player's Hand: " << playerHand << endl;

                // Comparisons
                if (!surrender) {
                    if (!split) {
                        if (playerHand > 21) { Lose = true; }
                        else {
                            dealerDraw(dealerHand, deck, deck_values);
                            cout << "Dealer's Hand: " << dealerHand << endl;
                            if (dealerHand > 21) { Win = true; }
                            else {
                                if (playerHand > dealerHand) { Win = true; }
                                if (playerHand == dealerHand) { Draw = true; }
                                if (playerHand < dealerHand) { Lose = true; }
                            }
                        }
                    }
                    else {
                        if (playerHand > 21) { Lose = true; }
                        else {
                            dealerDraw(dealerHand, deck, deck_values);
                            if (dealerHand > 21) { Win = true; }
                            else {
                                if (playerHand > dealerHand) { Win = true; }
                                if (playerHand == dealerHand) { Draw = true; }
                                if (playerHand < dealerHand) { Lose = true; }
                            }
                        }
                        if (playerHandForSplit > 21) { Lose = true; }
                        else {
                            dealerDraw(dealerHand, deck, deck_values);
                            if (dealerHand > 21) { Win = true; }
                            else {
                                if (playerHandForSplit > dealerHand) { Win2 = true; }
                                if (playerHandForSplit == dealerHand) { Draw2 = true; }
                                if (playerHandForSplit < dealerHand) { Lose2 = true; }
                            }
                        }
                    }
                }

                // Wallet changes
                if (surrender) { prev_run = 3; wallet -= bet / 2; }
                else {
                    if (!split) {
                        if (Win) { prev_run = 2; wallet += bet; }
                        if (Draw) { prev_run = 1; }
                        if (Lose) { prev_run = 0; wallet -= bet; }
                    }
                    else {
                        prev_run = 4;
                        if (Win) { wallet += bet; }
                        if (Lose) { wallet -= bet; }
                        if (Win2) { wallet += bet; }
                        if (Lose2) { wallet -= bet; }
                    }
                }
            }

            internal_reps = internal_reps + 1;
            cout << wallet << endl;
        }
        if (wallet <= 0) {
            defeats = defeats + 1;
        }
        else {
            cout << wallet << endl;
        }
    }
    cout << endl;
    cout << "Number of defeats: " << defeats << " with probability: " << double(defeats) / n << endl;
    cout << "Number of victories: " << n - defeats << " with probability: " << double(n - defeats) / n << endl;
    cout << "Number of games played: " << internal_reps << endl;
    cout << "Average number of games played per round: " << double(internal_reps) / n << endl;
}
