# Simple-Logic-Game---Burger-Starvation readme pl
wstęp:
Gra została stworzona przy pomocy Visual Studio 2015 Express, C++ i frameworke'a SFML. Była stworzona przeze mnie samego, zacząłem jątworzyć podczas Slavic Game Jam'u i skończyłem jakiś czas później. Celem było stworzenie działającej gry, spojrzenie z perspektywy programisty na tworzenie gier i nauka podstaw framework'u SFML. Kodowi daleko jest do perfekcji, ale spełnia założenia gry jame'owej - działa, choć po pewnym czasie oczywistym było, że bardziej abstrakcyjny styl pisaniabyłby użyteczny - zwłaszcza w przypadku funkcji bHandlePlayAgain albo vResetGame. Jedną z rzeczy, których się nauczyłem było to, że powinienem zawsze pakować jakąkolwiek funkcjonalność w oddzielne funkcje, nawet jeśli nie korzystam z zasad programowania obiektowego.

-----------------------------------------------------------------------

zasady:
Celem jest opróżnić wskaźnik "najedzenia", jednocześnie utrzymując wskaźniki "tur" i "zatrucia" powyżej 0. Można tego dokonać sprawdzając, który składnik jest "zatruty" - jeśli jest to opróżni wskaźnik "zatrucia" i "najedzenia" o 1 po dodaniu go i pokaże fioletową kropę obok składnika. Bezpiecznym jest użycie funkcji "reset" gdy wskaźnik zatrucia jest równy 1. Powtarzaj dopóki nie odkryjesz wszystkich "zatrutych" składników. Zawsze są 3 "zatrute" składniki i 3 nie "zatrute". Potem będziesz mógł bezpiecznie opróżniać wskaźnik "zatrucia" różnymi składnikami - powtarzanie tych samych czyni je 2 razy mniej efektywnymi. Ważnym jest by nie marnować "tur", gdyż nie ma sposobu by je odzyskać.

W grze są 3 wskaźniki:
fioletowy - wskaźnik "zatrucia", opróżnia się gdy dodajesz "zatruty" składnik do burgera. Kiedy dodajesz te same "zatrute" składniki wiele razy bez użycia funkcji "reset", jego wartość "zatrucia" maleje dwukrotnie za każdym razem. Wskaźnik resetuje się po użyciu funkcji "reset". Jeśli spadnie do 0, przegrałeś.

jasno niebieski - wskaźnik "tur", dodanie jakiegokolwiek składnika zawsze zmniejsza go o 1. On NIE resetuje się po użyciu funkcji "reset". Jeśli spadnie do 0, przegrałeś.

zielony - wskaźnik "najedzenia", opróźnia się gdy dodajesz jakikolwiek składnik, nawet "zatruty". Powtarzanie tego samego składnika bez użycia funkcji "reset" dzieli dwukrotnie jego wartość. Wskaźnik resetuje się po użyciu funkcji "reset". Jeśli spadnie do 0, wygrałeś.

Poza tym są tu przycisk resetu i kropki obok składników:
przycisk resetu - "resetuje" niektóre aspekty gry: wskaźnik "najedzenia", wskaźnik "zatrucia", wartości "zatrucia" i "najedzenia" do domyślnych (1) i wizualne składniki w burgerze. Nie resetuje wskaźnika "tur" ani kropek przy składnikach pokazujących czy są "zatrute" czy nie.

kropki przy składnikach - pokazują czy dany składnik jest "zatruty" czy nie. Fioletowa oznacza, że tak, jasno niebieski oznacza, że nie. Trzeba jednak najpierw je odkryć. Nie resetują się po użyciu funkcji "reset".

przycisk Escape - pauzuje grę i umożliwia ponowną grę, albo wyjście z gry. Jeśli gra jest zapauzowana to wznawia ją.

-----------------------------------------------------------------------

"fabuła":
Jesteś wygłodniałym naukowcem, który musi coś zjeść natychmiast. Lecz zatrułeś połowę składników, które masz w domu w celu eksperymentów i jako, że tematem jam'u było "nieznane" - zapomniałeś które składniki zatrułeś, więc musisz zjeść odpowiednio obfitego burgera, jednocześnie nie umierając z głodu albo zatrucia.
