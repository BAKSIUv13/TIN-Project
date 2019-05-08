## Użycie

```
server [port]
```

Jeśli port nie jest podany, serwer włącza się na domyślnym porcie 42000.

Zakończenie wejścia (Ctrl-D) kończy program, podobnie jak wpisanie `exit` na
wejściu.

Można usunąć zalogowaną sesję wpisując na wejściu

```
l <nazwa użytkownika>
```

## Co działa

### Można zająć zalogowaną sesję na serwerze

```
OwO!sess<ośmiobajtowy numer sesji>
```

Dodane numery sesji są ciągami ośmiu takich samych znakóœ w ASCII i można
znaleźć je w piku `app/server.cc` w funkcji
`void Server::SpecialHardcodeInit()`.

Servwer wtedy odsyła

```
OwO!okej
```

### Można po zalogowaniu napisać wiadomość

```
OwO!msg0<długość wiadomości w bajtach><wiadomość>
```

Serwer wtdy wyśle wszystkim zalogowanym

```
OwO!msg1<nazwa użytkownika><dłogość wiadomości><wiadomość>
```

### Można wysłać przesunięcoie myszki

```
OwO!mysz<int32 x><int32 y>
```

Serwer wyśle ewszystkim

```
OwO!maus<nazwa><int32 x><int32 y>
```
