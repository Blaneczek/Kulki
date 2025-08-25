Getting Started
Unreal Engine 5.5.4
Sklonować repozytorium lub pobrać ZIP, wygenerować pliki Visual Studio i skompilować przez IDE.

KULKI

Kulki to prosta gra zręcznęściowa w której zadaniem gracza jest połykanie słabszych i unikanie silniejszych przeciwników. 

Poruszanie się:
Poruszanie odbywa się za pomocą myszki. Gdy klikniemy prawy przycisk, kulka gracza podąża za kursorem. Dodatkowo, im mniejsza Szybkość, tym cięższe jest sterowanie (kulka ślizga się). Wartość tę można ustawić za pomocą float curve.

![BrakingCurve](https://github.com/user-attachments/assets/519af4bc-4f79-404a-a5d6-11dc5e65fcf8)
![PlayerCharacter](https://github.com/user-attachments/assets/1fe9901a-78e4-4891-aab5-05b1fbb56e09)

Przeciwnicy:
Przeciwnicy spawnowani są na początku gry. Im są dalej od gracza, tym ich wartość atrybutów jest większa. Wartości te mozna ustawić za pomocą float curve w której oś X jest odległością od gracza, a os Y siłą przeciwnika. 
Jeśli gracz zje czerwoną lub żółtą kulke, jego Siła/Szybkość wzrasta w zależności od wielkości przeciwnika (jego Siły). Jeśli zje fioletową kulke, jego Siła i Szybkość maleje.

![StrengthCurve](https://github.com/user-attachments/assets/68e998d7-bc25-4a8e-a3fd-e947d63d9f8c)

Informacje o spawnie przeciwników można ustawić w DataAsset: dla kazdego koloru wybieramy przedziały odległości (np. 5 żółtych kulek zespawnuje się w odległości między 1000 a 1500 od gracza).
![SpawnData](https://github.com/user-attachments/assets/f58c2307-3a02-495b-a8d7-ecef96c072a4)

Wartości Siły i Szybkości przeciwnkików skalują się z wybranym poziomem trudności. Mnożnik ustalamy w tym samym DataAsset, a poziom trudności po kliknięciu Start Game w menu.
![difficulty](https://github.com/user-attachments/assets/ced9ea94-7b2b-4d66-9a10-fa363cd3932d)
![difficultymenu](https://github.com/user-attachments/assets/b0ea5828-7dde-454d-bdd9-212976563b1e)

Gra posiada też menu pauzy po wscisnięciu klawisza P.
![pause](https://github.com/user-attachments/assets/0f2dfd5b-93ed-4e93-91a0-e4db3e458a6f)
