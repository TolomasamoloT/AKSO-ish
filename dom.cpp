#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// zmienne globalne dla ułatwienia
vector<vector<long long>> dp;
vector<vector<int>> board;
vector<long long> pomocniczy;
int k,n;

/* Funkcja pomocnicza, która oblicza największą możlową wartości
rozmieszczenia kamieni domina, w poprzedniej kolumnie, w taki sposób, aby
nie nachodziły na żaden z kamieni w sprawdanej masce.
*/
long long pom(int mask, int i) {
    // jak już mamy policzony wynik, to go zwracamy
    if (pomocniczy[mask] != -1) return pomocniczy[mask];
    
    // bo zawsze da się to osiągnąć nic nie stawiając na całej planszy :D
    pomocniczy[mask] = 0;
    // a to nie stawiając nic w danej kolumnie :P
    pomocniczy[mask] = max(pomocniczy[mask], dp[i-1][0]);
    int complement = (~mask) & ((1 << k) - 1);      // maska dopełnienia
    // przechodzimy przez wszystkie możliwe maski i wyliczamy maksymalną wartości
    for (int submask = complement; submask > 0; submask = (submask - 1) & complement) {
        if ((submask & mask) == 0) { // dodatkowy warunek sprawdzający brak nakładania się masek    
            pomocniczy[mask] = max(pomocniczy[mask], dp[i-1][submask]);
        }
    }
    return pomocniczy[mask];
}

void reset_pom() {
    fill(pomocniczy.begin(), pomocniczy.end(), -1);
}


int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    // Wczytujemy wymiary planszy
    cin >> n >> k;

    // Inicjalizujemy tablicę dp wartościami -10^10, żeby nie wychodzić poza plansze
    dp.resize(n+1, vector<long long>(1 << k, 0));
    for (int j = 0; j < 1<< k; j++) dp[0][j] = -10000000000;

    // tak samo dla planszy z wartości
    board.resize(n+1, vector<int>(k, 0));
    for (int j = 0; j < k; j++) board[0][j] = -1000000000;

    pomocniczy.resize(1 << k, -1);

    // Wczytujemy wartości na planszy
    for(int j = 0; j < k; j++) {
        for(int i = 1; i <= n; i++) {
            cin >> board[i][j];
        }
    }

    // dla każdej kolumny, 
    // dla każdego stanu wypełnienia kolumny kamykami domina
    // (reprezentowanego przez maski bitowe)
    // obliczamy wszystkie możliwe roztawienia tych kamieni (poziomo / pionowo)
    for (int i = 1; i < n+1; i++){
        for (int maska = 0; maska < 1 << k; maska++) {
            // case maska == 0 (0 kamieni domina)
            if (maska == 0) {
                dp[i][maska] = pom(maska, i);
                continue;
            }

            long long res = 0;

            // case same poziome
            long long suma = 0;
            for (int pozycja = 0; pozycja < k; pozycja++) {
                if (!(maska & (1 << pozycja))) continue;
                suma += board[i-1][pozycja] + board[i][pozycja];
            }
            res = pom(maska, i) + suma;

            // case 1 pionowy
            // stawiamy jedno domino pionowo, i patzymy jaką maksymalną wartość mamy
            for (int poz = 0; poz < k-1; poz++) {
                if (!((maska & (1 << poz)) && (maska & 1 << poz+1))) continue;
                int kamyk = board[i][poz] + board[i][poz+1];
                if (kamyk < 0) continue; // bo zawsze da się dostać większy wynik nic nie stawiając :P
                int nowaMaska = maska &~ (1 << poz) &~ (1 << (poz+1));  // maska stanu bez tego kamyka
                res = max(res, dp[i][nowaMaska] + kamyk);
            }

            dp[i][maska] = res;
        }
        reset_pom();
    }

    // znajdujemy maxymalną wartość po wszystkich rozstawieniach
    long long res = 0;
    for (int maska = 0; maska < (1 << k); maska++) {
        res = max(res, dp[n][maska]);
    }

    cout << res << "\n";
    return 0;
}