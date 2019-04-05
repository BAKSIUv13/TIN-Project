# Dokumentacja projektowa - django-client

## Osoba odpowiedzialna - Bartłomiej Kulik 283687

## Słownik

Sytuacje krytyczne - np. chwilowa/trwała utrata łączności między
węzłami, błędne pakiety, przekroczone limity czasowe,
scenariusz "wstawania" poszczególnych węzłów systemu.

## Opis ogólny

django-client będzie klientem serwera przeznaczonym dla
administratora. Program administratorski będzie umożliwiał
przeglądanie listy obiektów, listy zmian, usuwanie obiektów,
cofanie zmian, usuwanie użytkowników z serwera.

## Opis użytkowy

### Wymagania funkcjonalne i niefunkcjonalne

#### 1. Zarządzanie kontem administratora

#### 1.1 Logowanie

#### 1.2 Zmiana danych

#### 1.3 Usuwanie konta

#### 1.4 Wylogowywanie

#### 2. Obsługa serwera

#### 2.1 Otwieranie serwera

#### 2.2 Zamykanie serwera

#### 2.3 Obsługa listy użytkowników

#### 2.3.1 Usuwanie użytkownika

#### 2.3.2 Dodawanie użytkownika

#### 2.3.3 Zmienianie uprawnień użytkownikowi

#### 2.4 Obsługa listy obiektów

#### 2.4.1 Wyświetlanie listy obiektów

#### 2.4.2 Usuwanie obiektów z listy

#### 2.5 Obsługa listy zmian

#### 2.5.1 Wyświetlanie listy zmian

#### 2.5.2 Usuwanie pojedyńczej zmiany

#### 3. Wyświetlanie aktualnej planszy

### Wymagania niefunkcjonalne

#### 1. Bezpieczeństwo - użycie programu stunnel do bezpiecznej komunikacji django - przeglądarka

#### 2. Czytelność kodu - stosowanie się do konwencji Python PEP-8

### Przypadki użycia - użytkowe

1.0 Logowanie się do panelu administracyjnego

- administrator wprowadza nazwę użytkownika oraz hasło
- serwer dokonuje uwierzytelnienia
- wyświetlany jest panel administratora

2.0 Cofnięcie zmiany użytkownika

- `1.0 Logowanie się do panelu administracyjnego`
- wyświetlenie listy zmian
- usunięcie wybranej zmiany użytkownika

### Sytuacje krytyczne i nietypowe

Zostaną uwzględnione sytuacje krytyczne zdefiniowane w słowniku.

### Platformy docelowe

macOS(newest version)/Ubuntu 18.04 + Google Chrome

## Opis techniczny

### Stos technologoczny

python3/sockets + django

### Zarys architektury

Program będzie podzielony na kilka wątków:

- wątek główny
- wątek odpowiedzialny za odbieranie danych z serwera
- wątek odpowiedzialny za przesyłanie danych do serwera
- wątek odpowiedzialny za rysowanie aktualnej planszy

### Moduły/obiekty i interfejsy pomiędzy nimi

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