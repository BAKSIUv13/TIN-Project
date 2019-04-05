# Dokumentacja projektowa - dokumentacja główna

## Słownik

Sytuacje krytyczne - np. chwilowa/trwała utrata łączności między
węzłami, błędne pakiety, przekroczone limity czasowe,
scenariusz "wstawania" poszczególnych węzłów systemu

## Opis ogólny

Piotr Małyszek - 283694

Bartłomiej Kulik - 283687

Łukasz Smogorzewski - 283766

Norbert Pietrucha-Kacprowicz - 277344

Program będzie wzorowany na narzędziach webowych typu AWW App,
Google Whiteboard - umożliwiać będzie wspólne rysowanie grafiki wektorowej na
jednej płaszczyźnie. Zostanie udostępnionych kilka narzędzi:

- rysowanie prostokąta (grubość krawędzi, kolor krawędzi, kolor wypełnienia)
- rysowanie odcinka (grubość odcinka, kolor odcinka)
- rysowanie pędzlem (grubość pędzla, kolor pędzla)

Zapis obiektów graficznych zostanie wykonany w postaci wektorowej.

Program będzie zarządzał wszystkimi figurami w pełni obiektowo -
każda figura będzie obiektem (warstwą). Użytkownicy mogą dowolnie
zmieniać kolejność rysowania warstw, rozmiar obiektów, ich położenie
oraz inne właściwości już po pierwotnym narysowaniu. Zaznaczony przez
użytkownika obiekt jest mu udostępniany na wyłączność na pewien czas.

Program administratorski będzie umożliwiał przeglądanie listy obiektów,
listy zmian, usuwanie obiektów, cofanie zmian, usuwanie użytkowników z serwera.

Zadaniem serwera będzie zarządzanie klientami, synchronizacja listy obiektów
między nimi, uwierzytelnienie użytkowników i administratorów.

Zadaniem klientów będzie rasteryzacja listy obiektów wektorowych,
przekazywanie informacji o nowo narysowanych oraz zmodyfikowanych
obiektach do serwera i udostępnienie interfejsu użytkownikowi
do rysowania, wyświetlania oraz modyfikacji obiektów.

I Etap - wyświetlenie płaszczyzny na każdym kliencie wraz z pokazywaniem 
i synchronizowaniem pozycji kursorów klientów przez serwer.

## Opis użytkowy

### Wymagania funkcjonalne i niefunkcjonalne

TODO

### Przypadki użycia - użytkowe

TODO

### Sytuacje krytyczne i nietypowe

Zostaną uwzględnione sytuacje krytyczne zdefiniowane w słowniku.

### Platformy docelowe

TODO

## Opis techniczny

### Zarys architektury

TODO

### Szczegółowy opis protokołów

TODO

### Szczegółowy opis propozycji rozwiązania sytuacji krytycznych

TODO

### Przypadki użycia - systemowe

TODO

### Diagramy stanów

TODO

### Diagramy sekwencji

TODO

### Dokumentacja testowania wraz z uzasadnieniem i nawiązaniem do wymagań funkcjonalnych oraz niefunkcjonalnych

TODO