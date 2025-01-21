GRA PAC-MAN Z BLOKADĄ ŚCIAN I KILKOMA POZIOMAMI


1. Opis ogólny:
   - Jest to prosta gra 2D w stylu Pac-Mana.
   - Sterujemy postacią (graczem) za pomocą klawiszy strzałek.
   - Naszym celem jest uzyskać 150 punktów w danym poziomie,
     co przenosi nas do kolejnego poziomu.
   - Mamy 5 poziomów łącznie.

2. Ruch i blokowanie:
   - Postać nie może przenikać przez ściany. 
     Jeśli napotka ścianę, zostaje cofnięta do poprzedniej pozycji.
   - Wszelkie ściany (prostokątne i nieregularne) są nie do przejścia.

3. Zbieranie jabłek:
   - Na planszy rozsiane są jabłka. Kolizja z jabłkiem daje +30 punktów.
   - Po uzyskaniu >=150 punktów przechodzimy do następnego poziomu 
     (i odejmujemy 150 od wyniku gracza).

4. Przeciwnicy (duchy):
   - Poruszają się automatycznie (góra-dół). 
   - Jeśli gracz zderzy się z duchem, licznik kolizji rośnie o 1, 
     a postać jest cofana do środka ekranu.
   - Po 3 kolizjach -> GAME_OVER.

5. Ekrany:
   - MENU: Enter = start, R = ranking, ESC=wyjście
   - ROZGRYWKA: widzimy planszę, chodzimy postacią.
   - POMOC: pauza z opisem, powrót F1.
   - GAME_OVER: "Enter" -> spróbuj ponownie (reset punktów, kolizji i poziomu).
   - WYJSCIE_POTWIERDZENIE: T/N.
   - KONIEC: Zamknięcie okna.

6. Pliki graficzne i czcionka:
   - Folder "assets" zawiera: tloLabirynt.png, postac.png, przeciwnik.png, apple.png.
   - Czcionka: CSMS.ttf w głównym katalogu.

7. Wskazówki:
   - Program wymaga biblioteki SFML w wersji 2.5+.
   - Kompilować w trybie C++17 (lub nowszym).
   - W pliku "Silnik.cpp" w metodzie "aktualizuj()" 
     jest główna logika przejścia poziomów i kolizji.

8. Autor:
   - Kod inspirowany mechaniką Pac-Mana,
     modyfikacje i pliki .cpp / .h przygotowane przez Jakuba Dawidowskiego.
