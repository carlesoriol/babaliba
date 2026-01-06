# Projecte Babaliba <img src="data/babalibac/babalibapal8.png" width="24px"> - Atari ST Development

## To do

### Investigar

- Els 3 enemics aleatoris principals téene algun criteri de moviment diferent?
- La ubicació dels objectes especials (princesa, presoner, claus, tresor) no és aleatoria en la conversió. Buscar lògica de posicionament.

### Idees Extres

- Flors i plantes en els marcs de làrea de joc.
- Indicar el pis en el que ens trobem.
- Indicador de zona (jardí pis, massmorra...) i potser algun efecte visual. (color terra?)
- *Prefetch* de gràfics a la pantalla d'inici mentre no es prem cap tecla.
- Minimapa?
  - Funció `draw_pixel`.
  - `block_color` al *header*.
  - Es podria fer que sols es veuen les pantalles visitades
- Pot ser seria divertit de fer una versió ortigràfica per linux... o per l'st.

## Fet

### Àudio i Música
- Sons de pantalles de victòria, mort, *tic-tac* de la bomba (càlcul de temps) i explosió.
- Lògica per alentir la música al final.

### Lògica de Joc i Objectes
- Sistema de **Guardar/Carregar** partida (*save/load game*).
- **Objectes:** Princesa, clau lila, presoner, clau verda i tresor.
- IDs d'objectes especials i recollida de lletres de l'encanteri (*spells*) segons posició.
- Gestió d'objectes amb ubicació dinàmica canviant el mapa.
- Ubicacions especials amb teletransport.
- Habitacions amb moviment automàtic (escales).
- **Bomba:** - Temps d'explosió: 3,4 s. 
    - Només una bomba alhora. 
    - Mort per proximitat.
- **Portes:** Sistema de claus creuades (clau rosa obre porta verda i viceversa).

### Personatges i Enemics
- **Princesa:** Moviment aleatori, canvi de comportament segons distància (braços amunt/avall) i gestió com a enemic aleatori.
- **Enemics:** Serps (comencen a baix) i aranyes (comencen a dalt).
- Temps de reaparició de guardians (8 s) després d'explosió.
- Mort de la princesa o del presoner.

### Interfície i Gràfics
- Marcador de lletres: aconseguides (blanc/verd), pendents (cian/magenta).
- Pantalles: "Ya tienes el tesoro", "Ya tienes a la princesa", victòria, derrota (temps) i crèdits.
- Menú de trucs (*cheats*): vides, bombes, immunitat.
- Transicions *fade-in* i *fade-out*.
- Cartell dinàmic i dibuix d'elements a la barra de menú.

### Motor i Refactorització (C / 68000)
- Funció unificada de detecció (objecte/enemic/paret).
- Conversió de coordenades de joc a pantalla.
- Funció unificada de pintat de *sprites* i estructura de dades.
- Lectura de tecles per *scan codes* i canvi de `getchar` a `wait_key`.
- Solucionat bug de col·lisió per ubicació en pantalla.

### Altres
- Entrada i sortida del programa netes sense alterar cap paràmetre del sistema.
- **Idiomes:** Anglès i català.

## Notes on Atari ST, 68000 and C

### Documentació
- [TOS XBIOS Functions](https://freemint.github.io/tos.hyp/en/xbios_functions.html)
- [GEMDOS Reference](https://freemint.github.io/tos.hyp/en/gemdos_main.html)
- [Atari ST Programming Guide](https://www.bighole.nl/pub/mirror/homepage.ntlworld.com/kryten_droid/Atari/ST/spg/st_prog_guide.htm)

