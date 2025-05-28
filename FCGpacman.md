**Obiettivo:**
Molto semplicemente realizzare una versione simil-identica di Pacman in C++ utilizzando la libreria grafica SFML.

# Pacman 00

La versione contiene la mappa di gioco completa disegnata a schermo.

E' da notare che il codice è molto grezzo siccome a questo punto del progetto stavo cercando di capire la fattibilità di realizzare pacman come progetto finale.

### **TileMap.cpp:**

- La mappa è rappresentata da una matrice di interi che corrispondono agli ID dei tile (che a loro volta corrispondono ai nomi di file per comodità).
- Gli sprite delle tile vengono creati e posizionati in base alla griglia di gioco.

Viene inoltre stabilita la dimensione della finestra che sarà in scala rispetto al gioco originale.

Gli altri componenti prenderanno davvero forma nella prossima versione ed è per questo che vengono spiegati di seguito.

# Pacman 01

Viene implementata una versione molto grezza del giocatore, pacman puo spostarsi con wasd all'interno della finestra senza la presenza di collisioni.

## Componenti principali

### **Funzioni di utilità (utils.cpp):**

- Funzioni per caricare specificata la directory tutte le immagini presenti in un vector o hashmap di texture.
- Funzione di conversione dei pixel neri in trasparenti, utilizzata dalle due precedenti.

### **Player (Player.cpp):**

- Gestione del movimento (WASD) con implementazione di un buffer molto rudimentale.
- Funzione `update` per aggiornare la posizione spostando il player di tot pixel per frame nella direzione che si sta guardando, e la rotazione in base alla direzione usando il metodo `setRotation`.

### **Main loop (main.cpp):**

- Stato del gioco gestito dallo struct state.
- Gestione degli eventi input tastiera e chiusura finestra.
- Funzione `doGraphics` che gestisce il disegnamento dei vari oggetti(per ora griglia di debug, la mappa e il player).
- Ciclo che aggiorna e ridisegna la scena a ogni frame.

### **Convenzioni generali**

- Da qui in poi quasi tutti gli oggetti principali del progetto avranno tutti 2 funzioni in comune:
  -  `update` che si occupa di aggiornare lo stato dell'oggetto ogni frame.
  - `draw` che disegna l'oggetto a schermo ogni frame.


------

# Pacman 02

Aggiunte collisioni e animazioni per il giocatore.

## player.cpp

### Animazioni Giocatore
  - Animazioni gestite da due `int`: `AniFrameMax` corrisponde alla fine del ciclo animativo mentre `AniFrame`  viene aumentato ogni frame.
  - Questo mi permette di cambiare lo Sprite del frame d'animazione corrente ogni tot frame

```cpp
    // Update animation frame
    if(direction == 0) {
        // Empty
    } else if (AniFrame >= AniFrameMax) {
        AniFrame = 0;
        sprite->setTexture(frames[0]);
    } else if (AniFrame < AniFrameMax / 3) {
        AniFrame++;
        sprite->setTexture(frames[1]);
    } else {
        AniFrame++;
        sprite->setTexture(frames[2]);
    }
```
### Collisioni Giocatore

  - Le collisioni sono l'algoritmo che mi ha dato più difficolta in assoluto, ho provato diverse soluzioni senza successo ma alla fine sono arrivato a questo:

  - Il player ha un currentTile sulla griglia di gioco che viene aggiornato solo quando è centrato nel tile

   ```cpp
    if (isCentered()) {
      currentTile = sf::Vector2i(
        static_cast<*int*>(sprite->getPosition().x) / TILE_SIZE,
        static_cast<*int*>(sprite->getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE
      );
    }
   ```
   ```cpp
   bool Player::isCentered() {
        return (int(sprite->getPosition().x) % TILE_SIZE == TILE_SIZE/2 && 
                int(sprite->getPosition().y) % TILE_SIZE == TILE_SIZE/2);
   }
   ```
   - Questo garantisce che Pacman rimanga nella griglia in quanto capace di cambiare direzione solo quando al centro del tile, gia dalla futura versione però verrà aggiornata (quella sottostante) per un margine di "errore" di 3 pixel in modo che sia impossibile che il giocatore in un frame salti la finestra per poter cambiare direzione.
   ```cpp
    bool Player::isCentered() {
        int center = TILE_SIZE / 2;
        int xOffset = std::abs(int(sprite->getPosition().x) % TILE_SIZE - center);
        int yOffset = std::abs(int(sprite->getPosition().y) % TILE_SIZE - center);
        return (xOffset <= 3 && yOffset <= 3);
    }
   ```

Viene inoltre aggiunto un "padding" in all'inizio della finestra che sara il luogo che ospiterà lo scoring, le vite la frutta bonus e il livello corrente.

# Pacman 03

Le feature aggiunte sono la possibilità di mangiare i pallini, la gestione del punteggio e una prima grezza gestione dei livelli.

## **score.cpp**

La classe più semplice del progetto, `getPoints` `addPoints` abbastanza auto-esplicative, usa il font originale di pacman.

## **gameManager.cpp**

- Questa classe `GameManager` si occupa principalmente di gestire lo stato generale del gioco.
- Tiene traccia del livello corrente (`currentLevel`) e aggiorna il gioco quando tutti i pallini vengono raccolti (`checkLevelProgression()`).
- Quando i pallini diventano 0 i metodi `loadNextLevel` resettano lo stato di player e tilemap.
- Gestisce testi come il livello attuale e il messaggio "Ready?" e disegna le vite del gicoatore.
- Implementa il timer di pausa (un `int` che blocca il gioco per tot frame) che ora è utilizzato solo all'inizio di un livello ma che poi verrà espanso anche ad altre meccaniche.
- La `draw` di questa classe si occupa di disegnare tutto ciò che riguarda lo scoreboard in alto

## Buffering dell'input

In questa versione viene anche ultimato il sistema di buffering dell'input del giocatore, nelle versioni precedenti usavo una versione innecessariamente più complessa che utilizzava un vector che si è evoluta in questa:
```cpp
if (checkCollision(tileMap, buffered_dir) && isCentered()) {
    direction = buffered_dir; // Update direction if no collision
}
```
```cpp
void Player::setDirection(int dir) {
	buffered_dir = dir;  // last pressed key
}
```

Quando c'è la possibilità di girare viene utilizzata l'ultima direzione premuta dal giocatore, questo permette al giocatore di avere una finestra più che generosa per premere la propria scelta agli incroci.

# Pacman 04

La versione aggiunge i frutti bonus nella loro interezza

## Gestione del frutto bonus

I frutti bonus seguiranno questa logica:

| **Level** | **Bonus Symbol (Fruit)** | **Bonus Points** |
| --------- | ------------------------ | ---------------- |
| 1         | Cherries                 | 100              |
| 2         | Strawberry               | 300              |
| 3         | Peach                    | 500              |
| 4         | Peach                    | 500              |
| 5         | Apple                    | 700              |
| 6         | Apple                    | 700              |
| 7         | Grapes                   | 1000             |
| 8         | Grapes                   | 1000             |
| 9         | Galaxian                 | 2000             |
| 10        | Galaxian                 | 2000             |
| 11        | Bell                     | 3000             |
| 12        | Bell                     | 3000             |
| 13+       | Key                      | 5000             |


### **gameManager.cpp**

Salva tutti i frutti collezionati in `std::vector<sf::Sprite> collectedFruits = {};` e si occupa della logica per stamparli (simile a quella delle vite del giocatore)

```cpp
void GameManager::spawnBonusIfNeeded() {
    if (tileMap.getPelletCount() == 170 or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
          // Spawn bonus 
          bonusFruit.spawn(currentLevel);
      }
      if(tileMap.getPelletCount() == 70 && !bonusFruit.isFruitCollected()) {
          // Spawn bonus 
          bonusFruit.spawn(currentLevel);
      }
}
```
Lo spawn del bonus avviene come nel gico originale quando mancano 170 e 70 pallini da mangiare, `sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M` è un cheat di debug.

### **bonusFruit.cpp**

La classe `BonusFruit` gestisce tutto ciò che riguarda i **frutti bonus** nel gioco.

####  Comparsa e scomparsa del frutto bonus

La funzione `spawn(int currentLevel)` seleziona **texture e punti** in base al livello corrente seguendo la logica originale del gioco (Andando avanti con i livelli spawnerà diversa frutta e i punti guadagnati ottenendola aumenteranno), rende poi il frutto visibile e avviando un timer di 10 secondi (`fruitTimer = 600`).

La funzione `update()` riduce `fruitTimer` e gestisce la **scomparsa automatica** del frutto dopo il tempo limite:

```cpp
if (isVisible && --fruitTimer <= 0) {
    isVisible = false;
}
```

#### Collisione con il giocatore

Il metodo `checkCollision()` verifica la **collisione tra il frutto e il giocatore** usando `findIntersection`, imposta il frutto come raccolto e mostra i punti per 3 secondi (`pointsTimer = 180`):

```cpp
bool BonusFruit::checkCollision(const sf::FloatRect& playerBounds) { 
    sf::FloatRect fruitBox = getCollisionBox();
    if (isVisible && fruitBox.findIntersection(playerBounds)) {
        fruitCollected = true;
        isVisible = false;
        pointsTimer = 180;  // Keep the points visible for a short duration (3 seconds)
        return true;
    }
    return false;  // No collision detected
}
```

#### Rendering frutto
`draw()` disegna il frutto **solo se  attualmente visibile**, e lo fa lampeggiare negli ultimi 2 secondi prima della scomparsa `fruitTimer > 120 || (fruitTimer / 10) % 2 == 0` (questa logica verrà utilizzata anche per il lampeggiamento dei fantasmi spaventanti più avanti).

Se `pointsTimer` > 0, viene disegnato anche lo sprite del **punteggio acquisito**.

```cpp
if (pointsTimer > 0) {
    window.draw(*pointsSprite);
}
```

## Collisioni tra entità

questa è la versione che aggiunge le collisioni fra entità vediamo come sono gestite.

Player e BonuFruit usando la seguente funzione:
```cpp
sf::FloatRect Player::getCollisionBox() {
    return sprite->getGlobalBounds();
}
```

Funzione che verrà modificata già dalla prossima versione (che verrà usata anche per la box dei fantasmi dopo):

```cpp
sf::FloatRect Player::getCollisionBox() {
    sf::FloatRect bounds = sprite->getGlobalBounds();
    sf::Vector2f center = bounds.getCenter();

    sf::Vector2f halfSize = bounds.size / 2.f;

    return sf::FloatRect(
        center - halfSize / 2.f,
        halfSize
    );
}
```

La box invece di avere le dimensioni dello Sprite ha le dimensioni di un quadrato di metà larghezza 	rispetto all'originale e l'origine è spostata in modo che questo quadrato ridimensionato sia centrato 	nello Sprite. 

Questo mimica meglio la versione originale del gioco dove le entità possono sovrapporsi un minimo  	per poi collidere se sono l'una dentro l'altra.

# Pacman 05

Aggiunta della classe Ghost!

## **ghost.cpp**

La classe gestisce la logica dei fantasmi nel gioco. Ogni fantasma è identificato da un nome (Blinky, Pinky, Inky, Clyde) e possiede caratteristiche visive e comportamentali specifiche (che verranno implementate completamente nelle versioni successive).

### **Texture multiple**
Ogni fantasma dispone di tre set di texture:

- `standardTextures`: animazioni normali (due frame per direzione).
- `frightenedTextures`: stato di paura (quando il giocatore mangia una super pallina).
- `deadTextures`: stato morto (dopo essere stato mangiato).

La versione corrente usa solo le standard.

Il metodo `updateSprite()` aggiorna la Texture del fantasma in base allo stato (`isFrightened`, `isDead`, `isChasing`) e alla direzione.

### **Collisioni con la mappa**

Viene usata la stessa logica che viene usata per il giocatore.

### **Comportamento**
I fantasmi alternano due modalità:

- `chase`: l'IA del gioco originale non è stata ancora implementata e per ora i fantasmi inseguono il giocatore aggiornando il `targetTile` in base alla posizione del player.
- `scatter`: tornano in un angolo prestabilito della mappa, stabilito da `targetTile` e diverso per ogni fantasma.

Le due modalità si alternano ogni tot secondi al termine di un timer.

### **Calcolo direzione più efficente**
Il metodo `chaseDecision()` calcola la direzione più efficiente verso il `targetTile`, questo è per me l'algoritmo più bello del progetto e che da "vita" hai fantasmi.

```cpp
    const std::vector<sf::Vector2i> directions = {
        {  0, -1 },  // 1 = up
        {  0,  1 },  // 2 = down
        { -1,  0 },  // 3 = left
        {  1,  0 }   // 4 = right
    };

    int bestDir = 0;
    float minDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < directions.size(); ++i) {
        if((i+1) == getOppositeDirection()) continue;

        sf::Vector2i neighbor = currentTile + directions[i];

        if (!tileMap.isTileSolid(neighbor.x, neighbor.y)) {
            float dx = static_cast<float>(targetTile.x - neighbor.x);
            float dy = static_cast<float>(targetTile.y - neighbor.y);
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < minDistance) {
                minDistance = distance;
                bestDir = i + 1; // direction start from 1
            }
        }
    }
```
Se il Tile non è un muro e la direzione che si sta controllando non è uguale a quella opposta, si calcola la **distanza euclidea** tra quel Tile e il `targetTile` e si tiene traccia della direzione che porta alla distanza più breve.

# Pacman 06

Aggiunta la logica per i fantasmi spaventati e finalmente l'effetto pacman!

## State machine

Da questa versione viene implementata una state machine:

```cpp
enum class GhostState {
    Scatter,
    Chase,
    Frightened,
    Dead
};
```

Che assieme alla funzione `setState(GhostState newState)` mi permette di aggiornare e trattare lo stato del fantasma.

## Stato Fantasmi spaventati

I fantasmi spaventati non fanno uso di nessuna logica complessa per scegliere la direzione in cui virare agli incroci, come nel gioco originale sceglieranno una direzione randomica tra le disponibili che non sia la direzione opposta a quella attuale.

Ancora non possono essere mangiati, questo verrà implementato nella prossima tappa.

Una meccanica importante per il gioco però è che appena mangiato una super pallina i fantasmi si gireranno in direzione opposta.

## Effetto Pacman

Molto semplice, raggiunti i Tile al di fuori della mappa lo Sprite e il Tile corrente vengono aggiornati.

```cpp
// Handle pacman effect (wrap around the screen)
if(currentTile.x == -1 && currentTile.y == 14 && direction == 3) {
    currentTile.x = 28; // Pacman effect
    sprite.setPosition(sf::Vector2f(TILE_SIZE*28, TILE_SIZE*14 + SCORE_BOARD_HEIGHT + TILE_SIZE/2)); 
}
if (currentTile.x == 28 && currentTile.y == 14 && direction == 4) {
    currentTile.x = -1; // Pacman effect
    sprite.setPosition(sf::Vector2f(TILE_SIZE*-1, TILE_SIZE*14 + SCORE_BOARD_HEIGHT + TILE_SIZE/2));
}
```



# Pacman 07

Questa è una tappa bella massiccia.

Aggiunti le collisioni e conseguentemente anche lo stato di morte del fantasma e il sistema di reset dopo aver perso una vita, inoltre sono state completamente implementate le ai dei fantasmi e la loro "uscita dalla casetta".

## AI dei fantasmi

L'ai dei fantasmi usa la logica spiegata precedentemente per il calcolo della direzione più efficiente, l'unica cosa che cambia è la funzione `updateTargetTile` che modifica il target da inseguire secondo la logica del gioco originale.

### Blinky (rosso)

Blinky è il più semplice dei 4 è il targetTile è sempre quello del giocatore.

### Pinky (rosa)

Pinky inseguirà il tile 4 spazi avanti rispetto alla direzione in cui guarda giocatore.
Questo significa che cerca di intercettare il giocatore anticipando i suoi movimenti. 

Da notare che nel gioco originale a causa di un bug nell'implementazione originale del gioco, quando il giocatore si muove verso l'alto, Pinky finisce per puntare a **4 spazi sopra e 4 spazi a sinistra**, cosa che ho deciso di "risolvere" nel mio progetto e non inlementarla come "feature".

### Inky (azzurro)

Inky ha la logica più complessa. Il suo comportamento dipende sia dalla posizione del giocatore sia da quella di Blinky.

1. Si calcola un punto **2 spazi avanti** alla direzione del giocatore (come Pinky, ma solo 2 spazi)(punto B).
2. Si traccia un vettore da questo punto a Blinky(punto A).
3. Si effettua una rotazione di 180° del vettore con centro in **B **(targetTile = 2 * B - A).

In pratica, Inky tenderà a lavorare assieme a Blinky per intrappolare il giocatore presentandosi dalla direzione opposta.

### Clyde (arancione)

Clyde ha un comportamento ibrido:

- Se la distanza tra Clyde e il giocatore è maggiore di **8 tile**, Clyde si comporta come Blinky e insegue il giocatore.
- Se la distanza è **8 tile o meno**, Clyde si dirige verso la sua **scatter tile** (Angolo in basso a sinistra della mappa), comportandosi come se stesse fuggendo.

Questo lo rende il più imprevedibile del gruppo.

## Collisioni tra fantasma e giocatore

Come con i frutti per controllare le collisioni uso la funzione `findIntersection` e i Rect "ristretti" dati da `getCollisionBox`, niente di nuovo.

## Logica di cattura dei fantasmi

Come nel gioco originale catturare più fantasmi di fila ti permette di ottenere i seguenti punti:

- 1 Fantasma: 200
- 2 Fantasmi: 400
- 3 Fantasmi: 800
- 4 Fantasmi: 1600

Dopo un pò i fantasmi torneranno al loro stato normale e il counter si resetterà ripartendo da 200.

Io non implemento un timer ma uso questa logica:

```cpp
anyGhostFrightened = false; // reset anyGhostFrightened flag
for (int i = 0; i < ghosts.size(); i++) {
    if (ghosts[i].getState() == GhostState::Frightened) {
        anyGhostFrightened = true;
        break;
    }
}
if (!anyGhostFrightened) {
    eatenGhostsInARow = 0;
}
```

Finche è presente anche solo un fantasma spaventato è possibile continuare la combo per ottenere più punti

Altri dettagli implementati:

- Ora i fantasmi vanno a metà della loro velocita quando sono spaventati

- Quando un fantasma viene mangiato il gioco si ferma per un tot e il suo sprite viene manualmente settato, per la durata della pausa, allo sprite dei punteggi ottenuti.

 ```cpp
if(eatenGhostsInARow >= 4) {
	ghosts[i].setPointSprite(3); // set ghost sprite to points sprite
} else {
	ghosts[i].setPointSprite(eatenGhostsInARow - 1);
}   
 ```



## Stato fantasmi morti

Il fantasma mangiato avrà la sua velocità aumentata e il suo `targetTile` posto al Tile sopra la casetta dei fantasmi, una volta raggiunta la casa entreranno e respawneranno con Stato normale, in questo stato non sono presenti collisioni con il giocatore.

# Pacman 08

Aggiunti gli effetti sonori, l'animazione di morte e di completamento del livello e il GameOver (oltre a dei bilanciamenti alle velocità che diverranno obsoleti con la prossima tappa).

## Suoni

Gestiti dalla classe `GameManager.cpp`

```cpp
// Sound effects
sf::SoundBuffer introSoundBuffer;
sf::SoundBuffer deathSoundBuffer;
sf::SoundBuffer eatGhostSoundBuffer;
sf::SoundBuffer eatFruitSoundBuffer;
sf::SoundBuffer extraPacmanSoundBuffer;
sf::Sound soundEffect;
sf::SoundBuffer chomp1SoundBuffer;
sf::SoundBuffer chomp2SoundBuffer;
sf::Sound chompSoundEffect;
sf::SoundBuffer ghostWanderingSoundBuffer;
sf::SoundBuffer ghostFrightenedSoundBuffer;
sf::SoundBuffer ghostEatenSoundBuffer;
sf::Sound ghostSoundEffect;
```

Uso solo gli oggetti `Sound` e `SoundBuffer` siccome Pacman non utilizza musica nel suo gioco.

La loro gestione non è niente di che il suono dei fantasmi cambia in base al loro stato, se è presente almeno un fantasma morto verrà riprodotto in loop `ghostEatenSoundBuffer`, se è presente almeno un fantasma spaventato allora `ghostFrightenedSoundBuffer`, altrimenti `ghostWanderingSoundBuffer`

Gli altri sono tutti effetti riprodotti in determinate particolari situazioni.

## Completamento del livello

Ora mangiati tutti i pallini la griglia di gioco lampeggerà di bianco, effetto ottenuto mettento in pausa con `pauseTimer` e i metodi `convertBlueToWhite` e `convertWhiteToBlue`, usando la stessa logica usata per cambiare i pixel neri in trasparenti in utils.cpp.

## Gameover

Viene renderizzata la scritta del gameover e si smette di aggiornare e disegnare il giocatore.

# Pacman 09

Versione finale dedicata al bilanciamento del gioco.

## Bilanciamento velocità 

Ora quando viene superato un livello verrà passato a `loadNextLevel` il livello corrente. Inoltre è stato introdotto una costante `FULL_SPEED` che per comodita sarà uguale a `WINDOW_RESIZE`, che aggiornerà la velocita corrente in base alle regole qua sotto.

| **LEVEL** | **PAC-MAN SPEED** |            | **GHOST SPEED** |        |
| --------- | ----------------- | ---------- | --------------- | ------ |
|           | NORM              | **FRIGHT** | NORM            | FRIGHT |
| 1         | 80%               | 90%        | 75%             | 50%    |
| 2 – 4     | 90%               | 95%        | 85%             | 55%    |
| 5 – 20    | 100%              | 100%       | 95%             | 60%    |
| 21+       | 90%               | -          | 95%             | -      |

Con l'andare avanti dei livelli la velocità aumenterà sia per il giocatore che per i fantasmi rendendo il gioco più difficile.

Pacman sarà sempre più veloce dei fantasmi soprattutto dopo aver mangiato una super pallina, tuttavia dopo il livello 21 i fantasmi saranno un poco più veloci per difficoltà aggiuntiva.

## Bilanciamento fantasmi spaventati

Ora il tempo in cui i fantasmi rimarranno spaventati seguirà questa logica.

| **Level** | **Fright Time (sec)** |
| --------- | --------------------- |
| 1         | 6                     |
| 2         | 5                     |
| 3         | 4                     |
| 4         | 3                     |
| 5         | 2                     |
| 6         | 5                     |
| 7         | 2                     |
| 8         | 2                     |
| 9         | 1                     |
| 10        | 5                     |
| 11        | 2                     |
| 12        | 1                     |
| 13        | 1                     |
| 14        | 3                     |
| 15        | 1                     |
| 16        | 1                     |
| 17        | 0                     |
| 18        | 1                     |
| 18+       | 0                     |



# Risorse

## Funzionamento ai dei fantasmi

https://www.youtube.com/watch?v=ataGotQ7ir8&t=854s

## Varie specifiche

https://pacman.holenet.info/#LvlSpecs
