2 pkt. ✓ wyświetlać znak zachęty w postaci [{path}] $, gdzie {path} jest ścieżką do bieżącego katalogu roboczego;

2 pkt. ✓ obsługiwać polecenie cd, działające analogicznie jak cd znane nam z powłoki bash;

2 pkt. ✓ obsługiwać polecenie exit, kończące działanie programu powłoki;

XXX 2 pkt. ✓ obsługiwać polecenie help, wyświetlające na ekranie informacje o autorze programu i oferowanych przez niego funkcjonalnościach;

XXX 6 pkt. obsługiwać dwa inne, dowolnie wybrane polecenia powłoki (chodzi tutaj np. o własną, samodzielną, średnio zaawansowaną implementację dwóch poleceń, np. cp, których kody nie były podane podczas laboratoriów, nie są rozwiązaniami zadań domowych, ani nie pochodzą ze strony Bartłomieja Przybylskiego); bardzo proste implementacje, takie jak np. echo przy pomocy printf() nie są akceptowane;

XXX 6 pkt. ✓ przyjmować polecenia odwołujące się przez nazwę do programów znajdujących się w katalogach opisanych wartością zmiennej środowiskowej PATH oraz umożliwiać wywołanie tych skryptów i programów z argumentami (czyt. fork() + exec*());

XXX 4 pkt ✓ wypisywać komunikat błędu, gdy niemożliwe jest poprawne zinterpretowanie polecenia;

XXX 6 pkt. posiadać tzw. dodatkowe bajery, np. wyświetlanie loginu aktualnie zalogowanego użytkownika, obsługę kolorów, obsługę argumentów w cudzysłowach, sensowną obsługę sygnałów (np. Ctrl+Z), obsługę historii poleceń poprzez strzałki, uzupełnianie składni, itp.; punkty są przyznawane w zależności od stopnia skomplikowania problemu.