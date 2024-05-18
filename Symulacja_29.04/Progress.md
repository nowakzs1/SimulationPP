# Plan programu
1. Wyczyszczenie użytkowników
2. Wczytywanie użytkowników
    - sprawdzanie dostepnosci Stacji
    - przekierowanie
    - odrzucenie
3. Generowanie użytkownika w odpowiednim miejscu (tam gdzie juz obsłużone) i kalibracja czasu
4. Skok do najbliższego użytkownika
5. Powrót do pkt 1.

# Budowa
1. Klasa Symulacji Sieci
    - Funkcja Pętli Sieci (Bsy)
        - return lista(straceni, zuzyta moc)
2. Klasa BS
    - Generowanie użytkowników
        return next_user
    - Wczytywanie użytkowników
        return True/False
    - Redukcja bloków

# Skonczono na
    1.05 15:00 - runMainLoop next user arrival 
    1.05 17:00 - reduce żeby redukowalo wszystko, standardowo RB - template list - !!!Odrzucony pomysl
    2.05 1:55 - Zamiana users na 3 zmienne
    3.05 00:52 - wczytywanie uzytkownika - zrobic funkcje przekazywania usera, pierwsza do drugiej. druga do trzeciej, trzecia do pierwszej. zmienna passed mowiaca o koncu pętli
    8.05 - duzo zmian - dodanie L i zasypiania - TRZEBA TO SPRAWDZIC WSZYSTKO i dodac budzenie sie przy przekroczeniu progu H.

# TODO
 [x] dodanie funkcji sleepWell do BS
 [x] dodanie funkcji czyszczacej userow i RB (disconnect)
 [x] dodanie funkcji dodajacej userow
 [x] dodanie funkcji redukującej RB
 [x] sprawdzenie argumentu nex_hop w reduceRB
 [] rozszerzyc program o dodawanie ilosci dni symulacji
 [x] dodanie getLambda
 [x] dodanie generatora uzytkownikow i userPerSecond w BS
 [x] dodanie fun reduce do Network i ogarnij return
 [x] Wczytywanie uzytkownikow 
 [x] Generowanie uzytkownika w miejsce '0'
 [x] Zamiana users na 3 zmienne
 [x] dodanie sąsiadów do bsów 
 [x] dodanie funkcji passToNeighbor i zmiennej pass!!!
 [x] dodanie zmiennej connected informujacej o odrzuceniu/przujeciu usera
 []  dodanie Tau jako randoma w zakresie zaleznym od lambdy
 [x] dodanie funkcjonalnosci Is_full
 [] dodanie dwoch sasiadow dla rownomiernego przekazywania userow.(dwie zmienne if variable_1 = 0 or nie ma miejsca w drugim to do jednego a w przeciwnym do drugiego i wyzeruj zmienne. )
 [x] dodanie progu H
 [x] dodanie dwóch sąsiadów
 [] dodanie wyboru neighboura w connect()
 [x] dodanie flagi zasypiania dopiero po przekroczeniu L w gore. (_canGoToSleep)
 [x] dodanie funkcji sleepWell ktora rownomiernie przekazuje userow do sasiednich stacji. Proces zasypiania jest w reduceRB
 [] dodanie funkcjonalnosci progu H (budzenie)
 [] dodanie connected i disconnected users jako zmiennej do klasy bs
 [] dodanie funkcji get_power_status ktora mowi ile pobiera pradu aktualnie bs
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 [] 
 