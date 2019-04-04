# Klient C#
Osoba odpowiedzialna: Norbert Pietrucha-Kacprowicz
* Aplikacja klienta dla systemu Windows zostanie wykonana w języku C# z użyciem biblioteki SharpDX.
* Aplikacja będzie standardowym klientem TCP/UDP, tzn. do pracy będzie wymagała podania adresu IP serwera. Przed uruchomieniem właściwej części rysownika zostanie wyświetlony ekran logowania z polami adresu IP, nazwy użytkownika i hasła oraz nazwy pokoju, które będzie trzeba wypełnić przed rozpoczęciem pracy z programem.
* Wymagania funkcjonalne:
  * Aplikacja będzie udostępniała standardowy dla rysownika zestaw narzędzi - pędzle, figury geometryczne, opcjonalnie obsługę tekstu. Udostępni również płótno z którym użytkownik będzie prowadził interakcję.
  * Każdy narysowany obiekt będzie w pełni niezależnym wektorowym opisem - umożliwi to zmianę jego parametrów już po narysowaniu. Parametrami mogą być na przykład kolor frontu, kolor tła, rozmiar, rozmieszczenie punktów, tekst, obrys elementu.
  * Obiekty będą ułożone warstwami - umożliwi to zmianę kolejności wyświetlania już narysowanych obiektów.
  * Aby zrealizować aspekt sieciowy projektu, zostaną udostępnione funkcje kolaboracji z innymi użytkownikami - każdy klient znajdujący się w pokoju będzie mógł rysować na jednym wspólnym płótnie oraz modyfikować już narysowane obiekty innych klientów, z zastrzeżeniem, że w danej chwili tylko jeden klient może modyfikować dany obiekt.
* Wymagania niefunkcjonalne:
  * Aplikacja musi być bezpieczna - wszelkie dane logowania, będą musiały być przekazane w sposób zaszyfrowany do serwera, zaś na samym serwerze powinny być trzymane jedynie hashe haseł.
  * Aplikacja powinna być wydajna - w tym celu zostanie użyta biblioteka SharpDX, która udostępnia przyspieszenie sprzętowe funkcji rysujących. W zapewnieniu odpowiedniego poziomu wydajności pomoże również zrealizowanie aplikacji na wielu wątkach i tym samym oddzielenie logiki sekcji renderowania, interakcji oraz operacji sieciowych.
  * Aplikacja powinna zapewniać dobrą synchronizację klientów - tak, aby był widoczny aspekt pracy w czasie rzeczywistym. W tym celu należy możliwie zmniejszyć wszelkie opóźnienia protokołu, pobierania operacji wejścia i renderowania grafiki. Dodatkowo należy zoptymalizować metodę wysyłania zmian w obrazie - np. wysyłać tylko wektorowy opis zmienionych obiektów oraz pobierać jedynie zmiany w obrazie, a nie cały jego stan (wszystkie obiekty).
  * Aplikacja powinna być intuicyjna w obsłudze - interfejs powinien być prosty, ale i funkcjonalny. Toolkit z przyciskami (ikonami) narzędziowymi zostanie umieszczony na górze, płótno na środku, zaś sekcja zarządzania połączeniem (adres IP, status, przyciski kontrolowania połączenia) na dole ekranu.
  * Aplikacja powinna radzić sobie z występującymi wyjątkami - w szczególności powinna radzić sobie z problemami z komunikacją sieciową i informować w adekwatny sposób użytkownika o błędach.
