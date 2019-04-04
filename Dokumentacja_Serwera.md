# Serwer

Osoba odpowiedzialna: Piotr Małyszek

## Ogólne informacje

Serwer będzie zrobiony w języku C++. Będzie on samodzielną aplikacją
uruchamianą w terminalu.

Serwer będzie przechowywał aktualny stan obrazka, tzn. wszystkie obiekty, które
na nim są. Oprócz tego będzie przechowywał wszystkie zalogowane sesje
użytkowników, a także informacje o nich, m. in. pozycja kursora (jeśli jest
ustawiony, że ma ją trzymać), zaznaczone (zablokowane) obiekty. Będzie również
możliwość zalogowania się jako administrator, który będzie mógł cofać ruchy
innych, wyrzucać ich, banować i robić właściwie wszystko, co się da.

## Uwierzytelnianie

Będą trzy rodzaje użytkowników:

* zwykły - podaje tylko swoją nazwę,
* zarejestrowany - niektóre nazwy użytkownika będą zapisane jako zarezerwowane 
  i do wejścia na nie trzeba podać hasło,
* administrator - to samo, co użytkownik zarejestrowany, ale może wejść przez
  klienta administratorskiego i robić dowolne rzeczy na serwerze.

Nazwa użytkownika składa się z liter, cyfr i podkreśleń. Wielkość liter nie
jest rozróżniana przy porównywaniu nazw, jednak użytkownik będzie mógł mieć
wielkie i małe litery w nazwie. Nazwa ma co najmniej 3 znaki niebędące spacją
i co najwyżej 16 znaków.

Hasło ma dowolne znaki i ma długość od 4 do 30 znaków.

Hasła nie będą przechowywane bezpośrednio. Przechowywana będzie losowo
generowana sól, która będzie miała 30 znaków i skrót hasła połączonego z solą.

W przypadku użytkowników zarejestrowanych, niezależnie od tego, jakiej
wielkości litery podadzą przy logowaniu, będą zalogowani tak, jak są wpisani
w pliku konfiguracyjnym serwera.

## Konfiguracja

Program będzie korzystał z pliku konfiguracyjnego. W tym pliku będą wpisane
takie ustawienia, jak maksymalna ilość połączonych użytkowników, maksymalny
czas bezczynności, widoczność kursorów innych użytkowników i podobne.

Będzie również plik przechowujący zarejestrwane konta użytkowników. Będzie on
miał podobną strukturę do pliku '/etc/passwd' w systemie Unix. Linia w tym
pliku:

<pre><u>nazwa użytkownika</u><b>:</b>[<b>x</b>]<b>:</b><u>sól hasła</u><b>:</b><u>skrót hasła</u></pre>

gdzie **x** oznacza, czy użytkownik jest administratorem, sól jest zapisana
w postaci Base64, a skrót jako zwykła liczba.

## Współpraca z klientami

Serwer będzie odbierał od klientów pakiety danych, w których będą zawarte
opisy ich czynności i na ich podstawie będzie zmieniał 'stan świata', tzn.
stan obrazkai  informacje o użytkownikach. Co jakiś czas będzie również wysyłał
aktualizacje stanu świata do wszystkich użytkowników.

Będzie również możliwość wymuszenia wysłania całego stanu do klienta, która
będzie przydatna na przykład, gdy klient straci na chwilę połączenie.

Użytkownicy będą się łączyli z serwerem poprzez 'proszenie' o zalogowanie wraz
z podaniem swojej nazwy, wtedy serwer będzie logował klientów lub jeszcze 
prosił ich o hasło w przypadnku zarejestrowanych kont.

Rozłączanie będzie na żądanie klienta, administratora lub przy odpowiednim
ustawieniu automatycznie po określonym czasie nieaktywności.

Użytkownicy po zalogowaniu się będą otrzymywać niepowtarzalny identyfikator
sesji, który będzie liczbą i będzie musiał być wysłany przy każdym połączeniu
z serwerem.