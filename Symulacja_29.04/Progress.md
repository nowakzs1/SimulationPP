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
    20.05 - Sprawdzanie! skonczono na: równomierne rozdzielanie uwzględniające pojemność sąsiada w sleepWell
    22.05 Zrobiono prog H i budzenie i sprawdzono to. Pozostaje pobieranie informacji o tym ile prądu pobiera stacja bazowa 
    PAMIETAJ O UWZGLEDNIENIU CZASU ZASYPIANIA I WSTAWANIA

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

 20.05  Sprawdzene + zmiana _h i _l na float
            - działa zasypianie gdy Resourceblock.size() jest empty
            - przekazywanie równomierne działa
        - Sprawdzenie connect w środku SleepWell
            - Czy gdy sąsiad śpi to przekaże gdzie indziej lub rozłączy?
                Tak, rozłączy (co jest mało prawdopodobne gdy nie śpi) lub przekaże 
                [x] neighbours_space rozdzielone na 1 i 2
                [x] dodanie connected i disconnected users jako zmiennej do klasy bs (usuniecie disconnected w main loop i sleepWell)
                [x] równomierne rozdzielanie uwzględniające pojemność sąsiada

 [x] modyfikacja SleepWell pod kątem miejsca sąsiadów i sprawdzenia czy sąsiedzi śpią lub mają status overloading
 22.05 Sprawdzenie SleepWell:
    [x] jeśli resourceblock jest empty
    [x] jesli sąsiedzi nie mają miejsca
    [x] jesli 1 sasiad nie ma miejsca
    [x] jesli 2 sasiad nie ma miejsca
    [x] jesli jest na styk miejsca
    [x] jesli sasiedzi spią

 [x] równomierne przekazywanie userow nawet podczas spania
 [x] dodanie funkcjonalnosci progu H (budzenie)
    [x] dodanie funkcji wakeUpNeighbour
    [x] sprawdzenie WakeUpNeighbour
        - budzenie jednego i drugiego
 [x] dodanie czasu pracy Bsa (pobieranie danych przy zasypianiu itd.) i później przekonwertowanie to na moc zużytą 
    [x] dodanie changeStatus()
    [x] BS->addRunTime - do poprawy z blokowaniem
    [x] dodanie blokowania stacji bazowej 
    [x] dodanie funkcji blokowania BSa na 50ms przy zasypianiu i budzeniu sie
    [x] sprawdzenie blokowania
        [x] Stacja sie budzi ale jest zablokowana wiec przekierowywuje userow - zostaje sprawdzenie.
 [x] poprawa zbijania overloading - zostalo sprawdzenie
 [x] sprawdzic reduceRB poza petla z userem rownym next hop
 [x] BS->_PowerConsumed - liczenie mocy z _Runtime i _SleepTime
    [x] BS-> PowerConsumed()
    [x] test - sprawdzić next user arrival bo nie wiem czy dobrze liczy power consumed (odpal po prostu debuga i wchodz w poszczegolne funkcje sprawdzajac power consumed run time sleep time i next user arrival, czy wszystko sie dobrze dodaje)


----------------------- 27.05 -------------------------

Generatory:
 [x] Zakomentowanie prototypu losowania mi
 [x] Dodanie miejsca na dane i uwzglednienie seed
 [x] Dodanie generatora równomiernego
    [x] Wskaźnik generatora równomiernego do sieci 
 [x] Dodanie generatora wykładniczego
    [x] Wskaznik generatora wykladniczego do stacji bazowej
    [x] i przekazywanie go do generateUser
    [x] Każdy BS osobny generator

[x] Oczyszczenie kodu
[x] Narysowanie wykresow generatorow dla odpowiednich seedow

----------------------- 28.05 -------------------------

1. Pojawienie sie uzytkownika
2. Zwolnienie zasobow
3. Zmiana stanu stacji
4. Zmiana intensywnosci
5. Przekierowanie uzytkownika
6. Stracenie uzytkownika


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
 
